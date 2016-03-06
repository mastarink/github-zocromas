/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "std_fieldsets.h"
#include "std_selectors.h"
/* ###################################################################### */
#include "duf_evsql_selector_new.h"
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

static
/*   const char *                                                            */
/* duf_unref_fieldset( const char *fieldset, duf_node_type_t type, int *pr ) */
SRP( SQL, const char *, fieldset, fiel_dset, unref_fieldset, const char *fiel_dset, duf_node_type_t type )
{
  if ( fieldset )
  {
    if ( *fieldset == '#' )
    {
      const char *fsn;

      fsn = fieldset + 1;
      fieldset = duf_find_std_fieldset( fsn, type );
      if ( !fieldset )
        ERRMAKE( PTR );
    }
  }
/* return fieldset; */
  ERP( SQL, const char *, fieldset, fiel_dset, unref_fieldset, const char *fieldset, duf_node_type_t type );
}

/* static const char *                                                       */
/* duf_unref_selector( const char *selector, duf_node_type_t type, int *pr ) */
SRP( SQL, const char *, selector, selector_, unref_selector, const char *selector_, duf_node_type_t type )
{
  if ( selector )
  {
    if ( *selector == '#' )
    {
      const char *sln;

      sln = selector + 1;
      selector = duf_find_std_selector( sln, type );
      if ( !selector )
        ERRMAKE( PTR );
    }
  }
/* return selector; */
  ERP( SQL, const char *, selector, selector_, unref_selector, const char *selector_, duf_node_type_t type );
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
    str = mas_strdup( DUF_CONFIGGSP( db.main.name_x ) );
  }
  return str;
}

/* TODO : move to common place with general substitutions */
char *
duf_expand_sql( const char *sql, const char *dbname )
{
  char *nsql;

  nsql = mas_expand_string_cb_arg_alloc( sql, duf_expand_sql_xsdb_getvar, dbname );
/* MAST_TRACE( temp, 0, "@@@SQL:%s => %s", sql, nsql ); */
  return nsql;
}

static
/*   char *                                                   */
/* duf_fieldset2sql( const duf_sql_set_t * sql_set, int *pr ) */
SRP( SQL, char *, fieldset, NULL, fieldset2sql, const duf_sql_set_t * sql_set )
{
/* int rpr = 0; */
/* char *fieldset = NULL; */

  const char *const *pfs;

  pfs = sql_set->fieldsets;
  if ( *pfs )
  {
    while ( QNOERR && pfs && *pfs )
    {
      const char *fs;

      fs = duf_unref_fieldset( *pfs, sql_set->type, QPERRIND );
      if ( QNOERR )
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

    fs = duf_unref_fieldset( sql_set->fieldset, sql_set->type, QPERRIND );
    if ( QNOERR )
      fieldset = mas_strdup( fs );
  }
/* if ( pr ) */
/* *pr = rpr; */

/* return fieldset; */
  ERP( SQL, char *, fieldset, NULL, fieldset2sql, const duf_sql_set_t * sql_set );
}

char *
duf_selector2sql_vcat_many_frag( char *sql, unsigned with_pref, const char *jfirst, const char *jrest, const char *delim, unsigned parenthesis,
                                 unsigned *phas, va_list args )
{
  const char *quant = NULL;
  unsigned cnt = 0;

  assert( phas );
  while ( ( quant = va_arg( args, const char * ) ) )
  {
    if ( quant )
    {
      const char *j = " ";

      if ( with_pref )
        j = *phas ? jrest : jfirst;
      if ( cnt > 0 && delim )
        j = delim;
      if ( j )
      {
        sql = mas_strcat_x( sql, " " );
        sql = mas_strcat_x( sql, j );
        sql = mas_strcat_x( sql, " " );
      }
      if ( cnt == 0 && parenthesis )
      {
        sql = mas_strcat_x( sql, "( " );
        if ( 0 )
          sql = mas_strcat_x( sql, "/*4*/ " );
        sql = mas_strcat_x( sql, " " );
      }
      if ( 0 == strcmp( jfirst, "WHERE" ) )
      {
        sql = mas_strcat_x( sql, "( " );
        if ( 0 )
        {
          sql = mas_strcat_x( sql, "/*3 " );
          sql = mas_strcat_x( sql, jfirst );
          sql = mas_strcat_x( sql, " */ " );
        }
      }
      sql = mas_strcat_x( sql, quant );
      if ( 0 == strcmp( jfirst, "WHERE" ) )
      {
        if ( 0 )
          sql = mas_strcat_x( sql, " /*3*/" );
        sql = mas_strcat_x( sql, " )" );
      }
    /* T( "[%u:%u] quant=%s; %s", cnt, ( *phas ), quant, sql ); */
      ( *phas )++;
      cnt++;
    }
  }
  if ( cnt > 0 && parenthesis )
  {
    if ( 0 )
      sql = mas_strcat_x( sql, " /*4*/" );
    sql = mas_strcat_x( sql, " )" );
  }
  return sql;
}

char *
duf_selector2sql_cat_many_frag( char *sql, unsigned with_pref, const char *jfirst, const char *jrest, const char *delim, unsigned parenthesis,
                                unsigned *phas, ... )
{
  va_list args;

  va_start( args, phas );
  sql = duf_selector2sql_vcat_many_frag( sql, with_pref, jfirst, jrest, delim, parenthesis, phas, args );
  va_end( args );
  return sql;
}

char *
duf_selector2sql_cat_frag( char *sql, unsigned with_pref, const char *jfirst, const char *jrest, unsigned parenthesis, unsigned *phas,
                           const char *quant )
{
  return duf_selector2sql_cat_many_frag( sql, with_pref, jfirst, jrest, jrest, parenthesis, phas, quant, NULL );
}

char *
duf_selector2sql_vfiltercat_where_and( char *sql, unsigned with_pref, const char *delim, unsigned parenthesis, unsigned *phas_where, va_list args )
{
  sql = duf_selector2sql_vcat_many_frag( sql, with_pref, "WHERE", "AND", delim, parenthesis, phas_where, args );

  return sql;
}

char *
duf_selector2sql_filtercat_many_where_and( char *sql, unsigned with_pref, const char *delim, unsigned parenthesis, unsigned *phas_where, ... )
{
  va_list args;

  va_start( args, phas_where );
  sql = duf_selector2sql_vfiltercat_where_and( sql, with_pref, delim, parenthesis, phas_where, args );
  va_end( args );
  return sql;
}

char *
duf_selector2sql_filtercat_where_and( char *sql, unsigned with_pref, unsigned parenthesis, unsigned *phas_where, const char *filter )
{
  return duf_selector2sql_filtercat_many_where_and( sql, with_pref, "AND", parenthesis, phas_where, filter, NULL );
}

char *
duf_selector2sql_filtercat_list_where_and( char *sql, unsigned with_pref, unsigned *phas_where, const char *const flagname,
                                           const char *const *afilter, size_t n )
{
  char *sqln = NULL;
  unsigned tw = *phas_where;

  for ( unsigned i = 0; i < n && afilter && afilter[i]; i++ )
  {
    sqln = duf_selector2sql_filtercat_where_and( sqln, i > 0, 0 /* parenthesis */ , phas_where, afilter[i] );
  }
  if ( tw != *phas_where )
  {
    const char *j = NULL;

    if ( with_pref )
      j = tw ? "AND" : "WHERE";
    if ( j )
    {
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, j );
      sql = mas_strcat_x( sql, " " );
    }
    sql = mas_strcat_x( sql, "( " );
    if ( 0 )
      sql = mas_strcat_x( sql, "/*2*/ " );
    if ( flagname )
    {
      sql = mas_strcat_x( sql, ":" );
      sql = mas_strcat_x( sql, flagname );
      sql = mas_strcat_x( sql, " IS NULL OR " );
    }
    sql = mas_strcat_x( sql, "( " );
    if ( 0 )
      sql = mas_strcat_x( sql, "/*1*/ " );
    assert( sqln );
    sql = mas_strcat_x( sql, sqln );
    if ( 0 )
      sql = mas_strcat_x( sql, " /*1*/" );
    sql = mas_strcat_x( sql, " )" );
    if ( 0 )
      sql = mas_strcat_x( sql, " /*2*/" );
    sql = mas_strcat_x( sql, " )" );
    mas_free( sqln );
  }
  return sql;
}

/* 20150819.133515 */
/* char *                                                                                                                                   */
/* duf_selector2sql_2new( const duf_sql_set_t * sql_set, const duf_sql_set_t * sql_set2, const char *selected_db, unsigned total, int *pr ) */
SRP(SQL, char *, sql, NULL, selector2sql_2new, const duf_sql_set_t * sql_set, const duf_sql_set_t * sql_set2, const char *selected_db, unsigned total)
{
  /* int rpr = 0; */
  /* char *sql = NULL; */
  const char *selector2 = NULL;
  unsigned cte_mode = 0;
  const duf_sql_set_t *sql_set_uni;

  assert( sql_set );
/* T( "@%s : %p", sql_set->name, sql_set2 ); */

  sql_set_uni = ( sql_set2 ? sql_set2 : sql_set );
  if ( total )
    cte_mode = sql_set->cte && sql_set->selector2_cte;               /* ???????????????????? */
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
      fieldset = mas_strcat_x( fieldset, sql_set_uni->count_aggregate ? sql_set_uni->count_aggregate : "*" );
      fieldset = mas_strcat_x( fieldset, ") AS nf" );
    }
    else
      fieldset = duf_fieldset2sql( ( sql_set_uni->fieldset ? sql_set_uni : sql_set ), QPERRIND );

    if ( QNOERR && fieldset )
    {
      selector = duf_unref_selector( selector2, sql_set->type, QPERRIND );
      if ( QNOERR && selector && fieldset )
      {
        if ( cte_mode )
          sql = mas_strcat_x( sql, sql_set->cte );

        sql = mas_strcat_x( sql, "SELECT " );
        if ( 0 && sql_set )
        {
          sql = mas_strcat_x( sql, "/* " );
          sql = mas_strcat_x( sql, sql_set->name );
          sql = mas_strcat_x( sql, " */" );
        }
        if ( 0 && sql_set2 )
        {
          sql = mas_strcat_x( sql, "/* " );
          sql = mas_strcat_x( sql, sql_set2->name );
          sql = mas_strcat_x( sql, " */" );
        }
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
        sql = duf_selector2sql_filtercat_where_and( sql, 1, 1, &has_where, sql_set->filter );
        if ( sql_set2 )
          sql = duf_selector2sql_filtercat_where_and( sql, 1, 1, &has_where, sql_set2->filter );
        sql = duf_selector2sql_filtercat_list_where_and( sql, 1, &has_where, NULL, sql_set->afilter,
                                                         sizeof( sql_set->afilter ) / sizeof( sql_set->afilter[0] ) );
        {
          unsigned t MAS_UNUSED = has_where;

          if ( sql_set2 )
            sql = duf_selector2sql_filtercat_list_where_and( sql, 1, &has_where, NULL, sql_set2->afilter,
                                                             sizeof( sql_set2->afilter ) / sizeof( sql_set2->afilter[0] ) );
        /* if ( t != has_where )                         */
        /*   T( "@%s : %s", sql, sql_set2->afilter[0] ); */
        }
        {
          unsigned t MAS_UNUSED = has_where;

          sql = duf_selector2sql_filtercat_list_where_and( sql, 1, &has_where, "fFresh",
                                                           ( sql_set_uni->afilter_fresh ? sql_set_uni : sql_set )->afilter_fresh,
                                                           sizeof( sql_set_uni->afilter_fresh ) / sizeof( sql_set_uni->afilter_fresh[0] ) );
          sql = duf_selector2sql_filtercat_where_and( sql, 1, 1, &has_where, ( sql_set_uni->filter_fresh ? sql_set_uni : sql_set )->filter_fresh );
        /* if ( t != has_where ) */
        /*   T( "@%s", sql );    */
        }

#if 0
        if ( sql_set->filter_fast )
          sql = duf_selector2sql_filtercat_many_where_and( sql, "OR", &has_where, ":fFast  IS NULL", sql_set->filter_fast, NULL );
#else
        {
          unsigned t MAS_UNUSED = has_where;

          sql = duf_selector2sql_filtercat_list_where_and( sql, 1, &has_where, "fFast",
                                                           ( sql_set_uni->afilter_fast ? sql_set_uni : sql_set )->afilter_fast,
                                                           sizeof( sql_set_uni->afilter_fast ) / sizeof( sql_set_uni->afilter_fast[0] ) );
          sql = duf_selector2sql_filtercat_where_and( sql, 1, 1, &has_where, ( sql_set_uni->filter_fast ? sql_set_uni : sql_set )->filter_fast );
        /* if ( t != has_where ) */
        /*   T( "@%s", sql );    */
        }
#endif
        if ( !total )
        {
          sql = duf_selector2sql_filtercat_list_where_and( sql, 1, &has_where, NULL, ( sql_set_uni->amatcher ? sql_set_uni : sql_set )->amatcher,
                                                           sizeof( sql_set_uni->amatcher ) / sizeof( sql_set_uni->amatcher[0] ) );
          sql = duf_selector2sql_filtercat_where_and( sql, 1, 1, &has_where, ( sql_set_uni->matcher ? sql_set_uni : sql_set )->matcher );
        }
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
        sql = duf_selector2sql_cat_frag( sql, 1, "GROUP BY", ",", 0, &has_group, ( sql_set_uni->group ? sql_set_uni : sql_set )->group );
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
        sql = duf_selector2sql_cat_frag( sql, 1, "ORDER BY", ",", 0, &has_order, ( sql_set_uni->order ? sql_set_uni : sql_set )->order );
#endif
      }
    }
    mas_free( fieldset );
  }
  else
  {
    ERRMAKE( PTR );
    assert( 0 );
  }
  if ( QISERR && sql )
  {
    mas_free( sql );
    sql = NULL;
  }
  /* if ( pr ) */
    /* *pr = rpr; */
/* T( "@%s", sql ); */
  /* return sql; */
ERP(SQL, char *, sql, NULL, selector2sql_2new, const duf_sql_set_t * sql_set, const duf_sql_set_t * sql_set2, const char *selected_db, unsigned total);
}

char *
duf_selector2sql_new( const duf_sql_set_t * sql_set, const char *selected_db, unsigned total, int *pr )
{
  return duf_selector2sql_2new( sql_set, NULL, selected_db, total, pr );
}
