#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */

#include "duf_config_ref.h"
#include "duf_option_defs.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
#include "duf_context.h"
#include "duf_path2dirid.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_pathinfo_ref.h"
#include "duf_pathinfo_credel.h"

#include "duf_ufilter_ref.h"


#include "duf_sql2.h"
#include "duf_maindb.h"
#include "duf_status_ref.h"

#include "duf_path2db.h"
#include "sql_tables_defs.h"
#include "evsql_begfin.h"

#include "evsql_selector.h"


/* ###################################################################### */
#include "duf_pdi_attach.h"
/* ###################################################################### */

/* 20150912.094647 */
#ifdef DUF_ATTACH_SELECTED_PATTERN
int
duf_pdi_attach_selected( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
  DUF_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
  /* assert( global_status.db_attached_selected == NULL ); */
  if ( !pdi->db_attached_selected )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_SELECTED_PATTERN "' AS duf${PDI_NAME}";
    int changes = 0;

    pdi->db_attached_selected = mas_strdup( pdi->pdi_name );
    T( "%p attach %s", pdi, pdi->db_attached_selected );

    DOR( r, duf_eval_sql_one( sql, pdi->db_attached_selected, &changes ) );
  }
  DEBUG_ENDR( r );
}
#endif

/* 20150904.090641 */
int DUF_UNUSED
duf_pdi_detach_selected( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  static const char *sql1 = "DETACH DATABASE 'duf${PDI_NAME}'";

  int changes = 0;


  DOR( r, duf_eval_sql_one( sql1, pdi->pdi_name, &changes ) );
  T( "(%d) DETACH changes:%d", r, changes );

  DEBUG_ENDR( r );
}
