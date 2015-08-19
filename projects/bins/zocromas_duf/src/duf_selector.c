#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* for DUF_ACT_FLAG( progress ) !ONLY! */
#include "duf_option_defs.h"    /* for DUF_ACT_FLAG( progress ) !ONLY! */

#include "duf_utils.h"          /* duf_percent */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_pdi.h"
#include "duf_sccb_def.h"
#include "duf_sccb.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"

#include "duf_sql2.h"
#include "duf_ufilter_bind.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccb_eval_dirs.h" /* for assert  */

#include "duf_leaf_scan2.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

/* ###################################################################### */
#include "duf_selector.h"
/* ###################################################################### */


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


