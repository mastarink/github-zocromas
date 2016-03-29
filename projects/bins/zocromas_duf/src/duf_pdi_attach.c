/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_evsql_begfin.h"                                        /* duf_eval_sqlsq() ✗ */

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */
#include "duf_status_ref.h"
#include "sql_selected_defs.h"

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi_attach.h"
/* ###################################################################### */

/* 20150912.094647 */
#ifdef DUF_ATTACH_SELECTED_PATTERN
static
SR( PDI, pdi_attach_selected_i, duf_depthinfo_t * pdi )
{
  CR( main_db_open, pdi );
  MAST_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
  MAST_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
/* assert( global_status.db_attached_selected == NULL ); */
  if ( !pdi->db_attached_selected )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_SELECTED_PATTERN "' AS " DUF_DBSELECTEDALIAS;
    static const char *sql1 = "DROP  TABLE IF EXISTS " DUF_DBSELECTEDALIAS "." DUF_SQL_SELECTED_TMP_FILENAMES;
    int changes = 0;

    QT( "(%d) to attach (%p:%s)", r, pdi, pdi->pdi_name );
    pdi->db_attached_selected = mas_strdup( pdi->pdi_name );
  /* MAST_TRACE( sql, 0, "%p ATTACH %s : %s", pdi, pdi->db_attached_selected, sql ); */
    MAST_TRACE( db, 0, "%p ATTACH %s : %s", pdi, pdi->db_attached_selected, sql );

    CR( eval_sql_one, sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, pdi->db_attached_selected, &changes );
    CR( eval_sql_one, sql1, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, pdi->db_attached_selected, &changes );
  }
  MAST_TRACE( sql, 0, "%p post ATTACH %s", pdi, pdi->db_attached_selected );

  ER( PDI, pdi_attach_selected_i, duf_depthinfo_t * pdi );
}

SR( PDI, pdi_attach_selected, duf_depthinfo_t * pdi )
{
  if ( !pdi->attached_copy )
  {
    CR( pdi_attach_selected_i, pdi );
  }
  ER( PDI, pdi_attach_selected, duf_depthinfo_t * pdi );
}
#endif

/* 20150904.090641 */
SR( PDI, pdi_detach_selected, duf_depthinfo_t * pdi )
{
  static const char *sql1 = "DETACH DATABASE 'duf${PDI_NAME}'";

  int changes = 0;

  CR( eval_sql_one, sql1, pdi->pup, ( duf_yfilter_t * ) NULL, pdi->pdi_name, &changes );
  QT( "(%d) DETACH changes:%d", QERRIND, changes );

  ER( PDI, pdi_detach_selected, duf_depthinfo_t * pdi );
}
