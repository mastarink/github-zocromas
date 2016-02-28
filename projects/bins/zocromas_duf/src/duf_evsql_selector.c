/* #undef MAS_TRACING */
#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_expandable.h>                              
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "std_fieldsets.h"
#include "std_selectors.h"
/* ###################################################################### */
#include "duf_evsql_selector.h"
/* ###################################################################### */

#if 0
static const char *
duf_find_std_fieldset( const char *fieldset_name, duf_node_type_t type )
{
  const char *set = NULL;
  duf_fieldset_t *fs;

  fs = all_fieldsets;
  while ( fs && fs->name )
  {
    if ( type == fs->type && 0 == strcmp( fieldset_name, fs->name ) )
    {
      set = fs->set;
      break;
    }
    fs++;
  }
  return set;
}

static const char *
duf_find_std_selector( const char *selector_name, duf_node_type_t type )
{
  const char *selector = NULL;
  duf_selector_t *sl;

  sl = all_selectors;
  while ( sl && sl->name )
  {
    if ( type == sl->type && 0 == strcmp( selector_name, sl->name ) )
    {
      selector = sl->selector2;
      break;
    }
    sl++;
  }
  return selector;
}

static const char *
duf_unref_fieldset( const char *fieldset, duf_node_type_t type, int *pr )
{
  if ( fieldset )
  {
    if ( *fieldset == '#' )
    {
      const char *fsn;

      fsn = fieldset + 1;
      fieldset = duf_find_std_fieldset( fsn, type );
      if ( !fieldset && pr )
        DUF_MAKE_ERROR( *pr, DUF_ERROR_PTR );
    }
  }
  return fieldset;
}

static const char *
duf_unref_selector( const char *selector, duf_node_type_t type, int *pr )
{
  if ( selector )
  {
    if ( *selector == '#' )
    {
      const char *sln;

      sln = selector + 1;
      selector = duf_find_std_selector( sln, type );
      if ( !selector && pr )
        DUF_MAKE_ERROR( *pr, DUF_ERROR_PTR );
    }
  }
  return selector;
}

char *
duf_fieldset2sql( const duf_sql_set_t * sql_set, int *pr )
{
  int rpr = 0;
  char *fieldset = NULL;

  const char *const *pfs;

  pfs = sql_set->fieldsets;
  if ( *pfs )
  {
    while ( DUF_NOERROR( rpr ) && pfs && *pfs )
    {
      const char *fs;

      fs = duf_unref_fieldset( *pfs, sql_set->type, &rpr );
      if ( DUF_NOERROR( rpr ) )
      {
        assert( fs );
        if ( fieldset )
        {
          fieldset = mas_strcat_x( fieldset, "," );
          fieldset = mas_strcat_x( fieldset, fs );
        }
        else
        {
          fieldset = mas_strdup( fs );
        }
      }
      pfs++;
    }
  }
  else
  {
    const char *fs;

    fs = duf_unref_fieldset( sql_set->fieldset, sql_set->type, &rpr );
    if ( DUF_NOERROR( rpr ) )
      fieldset = mas_strdup( fs );
  }
  if ( pr )
    *pr = rpr;

  return fieldset;
}

/* 20150819.133515 */
char *
duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr )
{
  int rpr = 0;
  char *sql = NULL;
  const char *selector2 = NULL;
  unsigned cte_mode = 0;

  assert( sql_set );

  cte_mode = ( sql_set->selector2 ? 0 : ( ( sql_set->cte && sql_set->selector2_cte ) ? 1 : 0 ) );
  selector2 = cte_mode ? sql_set->selector2_cte : sql_set->selector2;
  assert( selector2 );
  if (  /* sql_set->fieldset && */ selector2 )
  {
    int has_where = 0;
    int has_group = 0;
    int has_order = 0;
    const char *selector = NULL;
    char *fieldset = NULL;

    fieldset = duf_fieldset2sql( sql_set, &rpr );
    if ( DUF_NOERROR( rpr ) && fieldset )
    {
      selector = duf_unref_selector( selector2, sql_set->type, &rpr );
      if ( DUF_NOERROR( rpr ) && selector && fieldset )
      {
        if ( cte_mode )
          sql = mas_strcat_x( sql, sql_set->cte );

        sql = mas_strcat_x( sql, "SELECT " );
        sql = mas_strcat_x( sql, fieldset );
        sql = mas_strcat_x( sql, " " );

        if ( sql_set->expand_sql )
        {
          char *tsql;

          tsql = duf_expand_sql( selector, selected_db );
          sql = mas_strcat_x( sql, tsql );
          mas_free( tsql );
        }
        else
        {
          sql = mas_strcat_x( sql, selector );
        }
# if 1
        if ( sql_set->filter )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AND " );
          else
            sql = mas_strcat_x( sql, " WHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->filter );
        }
# endif
# if 1
        if ( sql_set->matcher )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AND " );
          else
            sql = mas_strcat_x( sql, " WHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->matcher );
        }
# endif
# if 1
        if ( sql_set->group )
        {
          if ( has_group )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " GROUP BY " );
          has_group = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
# endif
# if 1
        if ( sql_set->order )
        {
          if ( has_order )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " ORDER BY " );
          has_order = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
# endif
      }
    }
    mas_free( fieldset );
  }
  else
  {
    DUF_MAKE_ERROR( rpr, DUF_ERROR_PTR );
    assert( 0 );
  }
  if ( DUF_IS_ERROR( rpr ) && sql )
  {
    mas_free( sql );
    sql = NULL;
  }
  if ( pr )
    *pr = rpr;
  return sql;
}

/* 20150819.133525 */
char *
duf_selector_total2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr )
{
  int rpr = 0;
  char *sql = NULL;
  const char *selector2 = NULL;

  assert( sql_set );

  selector2 = ( sql_set->cte && sql_set->selector2_cte ) ? sql_set->selector2_cte : sql_set->selector2;
  assert( selector2 );
  if ( selector2 )
  {
    int has_where = 0;
    int has_group = 0;
    int has_order = 0;
    const char *selector = NULL;

    selector = duf_unref_selector( selector2, sql_set->type, &rpr );
    if ( selector )
    {
      if ( sql_set->cte )
        sql = mas_strcat_x( sql, sql_set->cte );

      sql = mas_strcat_x( sql, "SELECT " );
      sql = mas_strcat_x( sql, "COUNT(" );
      sql = mas_strcat_x( sql, sql_set->count_aggregate ? sql_set->count_aggregate : "*" );
      sql = mas_strcat_x( sql, ") AS nf" );
      sql = mas_strcat_x( sql, " " );
      if ( sql_set->expand_sql )
      {
        char *tsql;

        tsql = duf_expand_sql( selector, selected_db );
        sql = mas_strcat_x( sql, tsql );
        mas_free( tsql );
      }
      else
      {
        sql = mas_strcat_x( sql, selector );
      }

# if 1
      if ( sql_set->filter )
      {
        if ( has_where )
          sql = mas_strcat_x( sql, " AND " );
        else
          sql = mas_strcat_x( sql, " WHERE " );
        has_where = 1;
        sql = mas_strcat_x( sql, sql_set->filter );
      }
# endif
# if 0
      if ( sql_set->matcher )
      {
        if ( has_where )
          sql = mas_strcat_x( sql, " AnD " );
        else
          sql = mas_strcat_x( sql, " WhERE " );
        has_where = 1;
        sql = mas_strcat_x( sql, sql_set->matcher );
      }
# endif
# if 1
      if ( sql_set->group )
      {
        if ( has_group )
          sql = mas_strcat_x( sql, "," );
        else
          sql = mas_strcat_x( sql, " GROUP BY " );
        has_group = 1;
        sql = mas_strcat_x( sql, sql_set->order );
      }
# endif
# if 1
      if ( sql_set->order )
      {
        if ( has_order )
          sql = mas_strcat_x( sql, "," );
        else
          sql = mas_strcat_x( sql, " ORDER BY " );
        has_order = 1;
        sql = mas_strcat_x( sql, sql_set->order );
      }
# endif
    }
    DUF_TRACE( select, 0, "TOTAL: %s", sql );
  }
  else
  {
    DUF_MAKE_ERROR( rpr, DUF_ERROR_PTR );
  }
  if ( pr )
    *pr = rpr;
  return sql;
}
#endif
