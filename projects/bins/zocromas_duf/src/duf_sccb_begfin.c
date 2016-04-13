/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccb.h"
#include "duf_sccb_structs.h"

#include "duf_ufilter_structs.h"

#include "duf_evsql_begfin.h"                                        /* duf_eval_sqlsq() ✗ */

#include "duf_sccbh_shortcuts.h"
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */

/* ###################################################################### */
#include "duf_sccb_begfin.h"
/* ###################################################################### */

SR( SCCBH, sccb_eval_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db )
{
  MAST_TRACE( sql, 6, "pu->md5id:%llu:%llu", pu ? pu->md5id.min : 99999, pu ? pu->md5id.min : 99999 );
  MAST_TRACE( sql, 6, "beginning_sql '%s'",
              ( sccb && sccb->beginning_sql_seq && sccb->beginning_sql_seq->sql ) ? *sccb->beginning_sql_seq->sql : "?" );

  CR( eval_sqlsq, sccb->beginning_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */ ,
      pu, py, selected_db );

  ER( SCCBH, sccb_eval_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db );
}

SR( SCCBH, sccb_eval_final_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py )
{
  CR( eval_sqlsq, sccb->final_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */ , pu, py,
      NULL /* selected.db */  );
  ER( SCCBH, sccb_eval_final_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py );
}
