/* 20150819.134056 */
#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"

#include "duf_expandable.h"

#include "duf_config_ref.h"
#include "duf_config_defs.h"
#include "duf_config_util.h"

#include "std_fieldsets.h"
#include "std_selectors.h"
/* ###################################################################### */
#include "evsql_selector.h"
/* ###################################################################### */

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

/* TODO : move to common place with general substitutions */
static const char *
duf_expand_sql_xsdb_getvar( const char *name, const char *arg )
{
  char *str = NULL;
  char buf[1024];

  if ( 0 == strcmp( name, "PDI_NAME" ) )
    str = mas_strdup( arg );
  else if ( 0 == strcmp( name, "DB_PATH" ) )
  {
#if 0
    str = mas_strdup( DUF_CONFIGGSP( db.dir ) );
    str = duf_config_db_path_add_subdir( str, ( int * ) NULL /* &r */  );
#else
    str = mas_normalize_path( DUF_CONFIGGS( db.path ) );
#endif
  }
  else if ( 0 == strcmp( name, "PID" ) )
  {
    snprintf( buf, sizeof( buf ), "%u", getpid(  ) );
    str = mas_strdup( buf );
  }
  else if ( 0 == strcmp( name, "DB_NAME" ) )
  {
    str = mas_strdup( DUF_CONFIGGSP( db.main.name ) );
  }
  return str;
}

char *
duf_expand_sql_new( const char *sql, const char *dbname )
{
  char *nsql;

  nsql = mas_expand_string_cb_arg_alloc( sql, duf_expand_sql_xsdb_getvar, dbname );
  /* DUF_TRACE( temp, 0, "@@@SQL:%s => %s", sql, nsql ); */
  return nsql;
}

static char *
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

char *
duf_selector2sql_vcat_many_new( char *sql, const char *jfirst, const char *jrest, const char *delim, unsigned parenthesis, unsigned *phas,
                                va_list args )
{
  const char *quant = NULL;
  unsigned cnt = 0;

  assert( phas );
  while ( ( quant = va_arg( args, const char * ) ) )
  {
    if ( quant )
    {
      const char *j;

      j = *phas ? jrest : jfirst;
      if ( cnt > 0 && delim )
        j = delim;
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, j );
      sql = mas_strcat_x( sql, " " );
      if ( cnt == 0 && parenthesis )
        sql = mas_strcat_x( sql, "( " );
      sql = mas_strcat_x( sql, "( " );
      sql = mas_strcat_x( sql, quant );
      /* T( "@quant=%s; %s", quant, sql ); */
      sql = mas_strcat_x( sql, " )" );
      ( *phas )++;
      cnt++;
    }
  }
  if ( cnt > 0 && parenthesis )
    sql = mas_strcat_x( sql, " )" );
  return sql;
}

char *
duf_selector2sql_cat_many_new( char *sql, const char *jfirst, const char *jrest, const char *delim, unsigned parenthesis, unsigned *phas, ... )
{
  va_list args;

  va_start( args, phas );
  sql = duf_selector2sql_vcat_many_new( sql, jfirst, jrest, delim, parenthesis, phas, args );
  va_end( args );
  return sql;
}

char *
duf_selector2sql_cat_new( char *sql, const char *jfirst, const char *jrest, unsigned parenthesis, unsigned *phas, const char *quant )
{
  return duf_selector2sql_cat_many_new( sql, jfirst, jrest, jrest, parenthesis, phas, quant, NULL );
}

char *
duf_selector2sql_vfiltercat_new( char *sql, const char *delim, unsigned *phas_where, va_list args )
{
  sql = duf_selector2sql_vcat_many_new( sql, "WHERE", "AND", delim, 1, phas_where, args );
  return sql;
}

char *
duf_selector2sql_filtercat_many_new( char *sql, const char *delim, unsigned *phas_where, ... )
{
  va_list args;

  va_start( args, phas_where );
  sql = duf_selector2sql_vfiltercat_new( sql, delim, phas_where, args );
  va_end( args );
  return sql;
}

char *
duf_selector2sql_filtercat_new( char *sql, unsigned *phas_where, const char *filter )
{
  return duf_selector2sql_filtercat_many_new( sql, "AND", phas_where, filter, NULL );
}

char *
duf_selector2sql_filtercat_list_new( char *sql, unsigned *phas_where, const char *const *afilter )
{
  for ( unsigned i = 0; afilter[i]; i++ )
    sql = duf_selector2sql_filtercat_new( sql, phas_where, afilter[i] );
  return sql;
}

char *
duf_selector2sql_filtercat2or_new( char *sql, unsigned *phas_where, const char *cfilter, const char *filter )
{
  return ( cfilter && filter ) ? duf_selector2sql_filtercat_many_new( sql, "OR", phas_where, cfilter, filter, NULL ) : sql;
}


/* 20150819.133515 */
char *
duf_selector2sql_2new( const duf_sql_set_t * sql_set, const duf_sql_set_t * sql_set2, const char *selected_db, unsigned total, int *pr )
{
  int rpr = 0;
  char *sql = NULL;
  const char *selector2 = NULL;
  unsigned cte_mode = 0;

  assert( sql_set );




  if ( total )
    cte_mode = sql_set->cte && sql_set->selector2_cte; /* ???????????????????? */
  else
    cte_mode = ( sql_set->selector2 ? 0 : ( ( sql_set->cte && sql_set->selector2_cte ) ? 1 : 0 ) );

  selector2 = cte_mode ? sql_set->selector2_cte : sql_set->selector2;

  assert( selector2 );
  if (  /* sql_set->fieldset && */ selector2 )
  {
    unsigned has_where = 0;
    unsigned has_group = 0;
    unsigned has_order = 0;
    const char *selector = NULL;
    char *fieldset = NULL;

    if ( total )
    {
      fieldset = mas_strdup( "COUNT(" );
      fieldset = mas_strcat_x( fieldset, sql_set->count_aggregate ? sql_set->count_aggregate : "*" );
      fieldset = mas_strcat_x( fieldset, ") AS nf" );
    }
    else
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

          tsql = duf_expand_sql_new( selector, selected_db );
          sql = mas_strcat_x( sql, tsql );
          mas_free( tsql );
        }
        else
        {
          sql = mas_strcat_x( sql, selector );
        }

        sql = duf_selector2sql_filtercat_new( sql, &has_where, sql_set->filter );
        {
          unsigned t DUF_UNUSED = has_where;

#if 0
          sql = duf_selector2sql_filtercat_list_new( sql, &has_where, sql_set->filter_fresh );
#else
          sql = duf_selector2sql_filtercat_new( sql, &has_where, sql_set2 ? sql_set2->filter_fresh : sql_set->filter_fresh );
#endif
          /* if ( t != has_where ) */
          /*   T( "@%s", sql );    */
        }

#if 0
        if ( sql_set->filter_fast )
          sql = duf_selector2sql_filtercat_many_new( sql, "OR", &has_where, ":fFast  IS NULL", sql_set->filter_fast, NULL );
#else
        {
          unsigned t DUF_UNUSED = has_where;

          sql = duf_selector2sql_filtercat2or_new( sql, &has_where, ":fFast  IS NULL", sql_set2 ? sql_set2->filter_fast : sql_set->filter_fast );
          /* if ( t != has_where ) */
          /*   T( "@%s", sql );    */
        }
#endif
        if ( !total )
          sql = duf_selector2sql_filtercat_new( sql, &has_where, sql_set->matcher );
#if 0
        if ( sql_set->group )
        {
          if ( has_group )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " GROUP BY " );
          has_group = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#else
        sql = duf_selector2sql_cat_new( sql, "GROUP BY", ",", 0, &has_group, sql_set->group );
#endif
#if 0
        if ( sql_set->order )
        {
          if ( has_order )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " ORDER BY " );
          has_order = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#else
        sql = duf_selector2sql_cat_new( sql, "ORDER BY", ",", 0, &has_order, sql_set->group );
#endif
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
  /* T("@%s", sql); */
  return sql;
}

char *
duf_selector2sql_new( const duf_sql_set_t * sql_set, const char *selected_db, unsigned total, int *pr )
{
  return duf_selector2sql_2new( sql_set, NULL, selected_db, total, pr );
}
