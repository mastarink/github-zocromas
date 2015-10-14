/* 20150819.134056 */
#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_utils_path.h"

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
    str = duf_normalize_path( DUF_CONFIGGS( db.path ) );
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
duf_expand_sql( const char *sql, const char *dbname )
{
  char *nsql;

  nsql = mas_expand_string_cb_arg_alloc( sql, duf_expand_sql_xsdb_getvar, dbname );
  /* DUF_TRACE( temp, 0, "@@@SQL:%s => %s", sql, nsql ); */
  return nsql;
}

/* 20150819.133515 */
char *
duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr )
{
  int rpr = 0;
  char *sql = NULL;

  if ( sql_set->fieldset && sql_set->selector2 )
  {
    /* if ( 0 == strncmp( sql_set->selector2, "SELECT", 6 ) )             */
    /* {                                                                  */
    /*   char *sql3;                                                      */
    /*                                                                    */
    /*   assert( 0 );                                                     */
    /*   sql3 = duf_sql_mprintf( sql_set->selector2, sql_set->fieldset ); */
    /*   sql = mas_strdup( sql3 );                                        */
    /*   mas_free( sql3 );                                                */
    /* }                                                                  */
    /* else                                                               */
    {
      int has_where = 0;
      const char *selector = NULL;
      char *fieldset = NULL;

      {
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
      }
      if ( DUF_NOERROR( rpr ) )
      {
        selector = duf_unref_selector( sql_set->selector2, sql_set->type, &rpr );
        if ( DUF_NOERROR( rpr ) && selector && fieldset )
        {
          sql = mas_strdup( "SELECT " );
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
#if 1
          if ( sql_set->filter )
          {
            if ( has_where )
              sql = mas_strcat_x( sql, " aND " );
            else
              sql = mas_strcat_x( sql, " wHERE " );
            has_where = 1;
            sql = mas_strcat_x( sql, sql_set->filter );
          }
#endif
#if 1
          if ( sql_set->matcher )
          {
            if ( has_where )
              sql = mas_strcat_x( sql, " AND " );
            else
              sql = mas_strcat_x( sql, " WHERE " );
            has_where = 1;
            sql = mas_strcat_x( sql, sql_set->matcher );
          }
#endif
        }
      }
      mas_free( fieldset );
    }
  }
  else
  {
    DUF_MAKE_ERROR( rpr, DUF_ERROR_PTR );
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

  selector2 = ( sql_set->cte ) ? sql_set->selector2_cte : sql_set->selector2;
  assert( selector2 );
  if ( selector2 )
  {
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


#if 1
        if ( sql_set->filter )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AND " );
          else
            sql = mas_strcat_x( sql, " WHERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->filter );
        }
#endif
#if 0
        if ( sql_set->matcher )
        {
          if ( has_where )
            sql = mas_strcat_x( sql, " AnD " );
          else
            sql = mas_strcat_x( sql, " WhERE " );
          has_where = 1;
          sql = mas_strcat_x( sql, sql_set->matcher );
        }
#endif
#if 1
        if ( sql_set->group )
        {
          if ( has_group )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " GROUP BY " );
          has_group = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#endif
#if 1
        if ( sql_set->order )
        {
          if ( has_order )
            sql = mas_strcat_x( sql, "," );
          else
            sql = mas_strcat_x( sql, " ORDER BY " );
          has_order = 1;
          sql = mas_strcat_x( sql, sql_set->order );
        }
#endif
      }

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
