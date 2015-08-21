/* 20150819.134056 */
#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_sql.h"

/* ###################################################################### */
#include "duf_selector.h"
/* ###################################################################### */


/* 20150819.133515 */
char *
duf_selector2sql( const duf_sql_set_t * sql_set )
{
  char *sql = NULL;

  if ( sql_set->fieldset && sql_set->selector2 )
  {
    if ( 0 == strncmp( sql_set->selector2, "SELECT", 6 ) )
    {
      char *sql3;

      assert( 0 );
      sql3 = duf_sql_mprintf( sql_set->selector2, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      int has_where = 0;

      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, sql_set->fieldset );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->selector2 );
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
}

/*
SELECT COUNT(*) AS nf  FROM main.paths AS pt  LEFT JOIN common_pathtot_dirs  AS td ON (td.Pathid=pt.rowid)  LEFT JOIN common_pathtot_files AS tf ON (tf.Pathid=pt.rowid)  WHERE pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName )
SELECT COUNT(*) AS nf  FROM paths AS p 

*/

/* 20150819.133525 */
char *
duf_selector_total2sql( const duf_sql_set_t * sql_set )
{
  char *sql = NULL;

/* #define DUF_SELECTOR_TOTAL selector_total2 */
#define DUF_SELECTOR_TOTAL selector2
  assert( sql_set );
  assert( sql_set->DUF_SELECTOR_TOTAL );
  if ( sql_set->DUF_SELECTOR_TOTAL )
  {
    if ( 0 == strncmp( sql_set->DUF_SELECTOR_TOTAL, "SELECT", 6 ) )
    {
      char *sql3;

      assert( 0 );
      sql3 = duf_sql_mprintf( sql_set->DUF_SELECTOR_TOTAL, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      int has_where = 0;
      int has_group = 0;
      int has_order = 0;

      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, "COUNT(" );
      sql = mas_strcat_x( sql, sql_set->count_aggregate ? sql_set->count_aggregate : "*" );
      sql = mas_strcat_x( sql, ") AS nf" );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->DUF_SELECTOR_TOTAL );

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
}
