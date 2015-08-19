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

      sql3 = duf_sql_mprintf( sql_set->selector2, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, sql_set->fieldset );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->selector2 );
    }
  }
  else
    DUF_SHOW_ERROR( "Bad arg" );
  return sql;
}

/* 20150819.133525 */
char *
duf_selector_total2sql( const duf_sql_set_t * sql_set )
{
  char *sql = NULL;

  assert( sql_set );
  assert( sql_set->selector_total2 );
  if ( sql_set->selector_total2 )
  {
    if ( 0 == strncmp( sql_set->selector_total2, "SELECT", 6 ) )
    {
      char *sql3;

      sql3 = duf_sql_mprintf( sql_set->selector_total2, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, "COUNT(*) AS nf" );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->selector_total2 );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Bad arg" );
  }
  return sql;
}
