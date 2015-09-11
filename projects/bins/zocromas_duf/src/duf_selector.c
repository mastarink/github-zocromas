/* 20150819.134056 */
#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_config_ref.h"

#include "duf_maintenance.h"

#include "duf_sql.h"

#include "duf_hook_types.h"
#include "duf_scan_types.h"

#include "std_fieldsets.h"
/* ###################################################################### */
#include "duf_selector.h"
/* ###################################################################### */

const char *
duf_find_fieldset( const char *fieldset_name )
{
  const char *set = NULL;
  duf_fieldset_t *fs;

  fs = all_fieldsets;
  while ( fs && fs->name )
  {
    if ( 0 == strcmp( fieldset_name, fs->name ) )
    {
      set = fs->set;
      break;
    }
    fs++;
  }
  return set;
}

const char *
duf_unref_fieldset( const char *fieldset )
{
  if ( fieldset )
  {
    if ( *fieldset == '#' )
    {
      const char *fsn;

      fsn = fieldset + 1;
      fieldset = duf_find_fieldset( fsn );
    }
  }
  return fieldset;
}

static char *
duf_getvar( const char *name, const char *arg )
{
  char *str = NULL;

  if ( 0 == strcmp( name, "SELECTED_DB" ) )
    str = ( char * ) arg;
  else if ( 0 == strcmp( name, "DB_PATH" ) )
    str = ( char * ) duf_config->db.dir;
  DUF_TRACE( temp, 10, "@@%s :: %s => %s", name, arg, str );
  return str;
}

char *
duf_expand_selected_db( const char *sql, const char *dbname )
{
  char *nsql;

  nsql = mas_expand_string_cb_arg( sql, duf_getvar, dbname );
  DUF_TRACE( temp, 10, "@@@SQL:%s => %s", sql, nsql );
  return nsql;
}

/* 20150819.133515 */
char *
duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db )
{
#define DUF_SELECTOR selector2
  char *sql = NULL;

  if ( sql_set->fieldset && sql_set->DUF_SELECTOR )
  {
    /* if ( 0 == strncmp( sql_set->DUF_SELECTOR, "SELECT", 6 ) )             */
    /* {                                                                  */
    /*   char *sql3;                                                      */
    /*                                                                    */
    /*   assert( 0 );                                                     */
    /*   sql3 = duf_sql_mprintf( sql_set->DUF_SELECTOR, sql_set->fieldset ); */
    /*   sql = mas_strdup( sql3 );                                        */
    /*   mas_free( sql3 );                                                */
    /* }                                                                  */
    /* else                                                               */
    {
      int has_where = 0;

      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, duf_unref_fieldset( sql_set->fieldset ) );
      sql = mas_strcat_x( sql, " " );
      if ( sql_set->set_selected_db )
      {
        char *tsql;

        tsql = duf_expand_selected_db( sql_set->DUF_SELECTOR, selected_db );
        sql = mas_strcat_x( sql, tsql );
        mas_free( tsql );
      }
      else
      {
        sql = mas_strcat_x( sql, sql_set->DUF_SELECTOR );
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
  else
    DUF_SHOW_ERROR( "Bad arg" );
  return sql;
#undef  DUF_SELECTOR
}

/* 20150819.133525 */
char *
duf_selector_total2sql( const duf_sql_set_t * sql_set, const char *selected_db )
{
/* #define DUF_SELECTOR selector_total2 */
#define DUF_SELECTOR selector2
  char *sql = NULL;

  assert( sql_set );
  assert( sql_set->DUF_SELECTOR );
  if ( sql_set->DUF_SELECTOR )
  {
    /* if ( 0 == strncmp( sql_set->DUF_SELECTOR, "SELECT", 6 ) )             */
    /* {                                                                           */
    /*   char *sql3;                                                               */
    /*                                                                             */
    /*   assert( 0 );                                                              */
    /*   sql3 = duf_sql_mprintf( sql_set->DUF_SELECTOR, sql_set->fieldset ); */
    /*   sql = mas_strdup( sql3 );                                                 */
    /*   mas_free( sql3 );                                                         */
    /* }                                                                           */
    /* else                                                                        */
    {
      int has_where = 0;
      int has_group = 0;
      int has_order = 0;

      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, "COUNT(" );
      sql = mas_strcat_x( sql, sql_set->count_aggregate ? sql_set->count_aggregate : "*" );
      sql = mas_strcat_x( sql, ") AS nf" );
      sql = mas_strcat_x( sql, " " );

      if ( sql_set->set_selected_db )
      {
        char *tsql;

        tsql = duf_expand_selected_db( sql_set->DUF_SELECTOR, selected_db );
        sql = mas_strcat_x( sql, tsql );
        mas_free( tsql );
      }
      else
      {
        sql = mas_strcat_x( sql, sql_set->DUF_SELECTOR );
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
    DUF_TRACE( select, 0, "TOTAL: %s", sql );
  }
  else
  {
    DUF_SHOW_ERROR( "Bad arg" );
  }
  return sql;
#undef  DUF_SELECTOR
}
