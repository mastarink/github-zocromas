/* #undef MAS_TRACING */
#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"

#include "duf_sccb.h"
#include "evsql_begfin.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_maindb.h"

/* ###################################################################### */
#include "duf_sccb_begfin.h"
/* ###################################################################### */

int
duf_sccb_eval_sqlsq( const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db )
{
  DUF_STARTR( r );

  DUF_TRACE( sql, 6, "pu->md5id:%llu:%llu", pu ? pu->md5id.min : 99999, pu ? pu->md5id.min : 99999 );
  DUF_TRACE( sql, 6, "beginning_sql '%s'",
             ( sccb && sccb->beginning_sql_seq && sccb->beginning_sql_seq->sql ) ? *sccb->beginning_sql_seq->sql : "?" );

  DOR( r, duf_eval_sqlsq( sccb->beginning_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */ ,
                          pu, py, selected_db ) );

  DUF_ENDR( r );
}

int
duf_sccb_eval_final_sqlsq( const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py )
{
  DUF_STARTR( r );

  DOR( r, duf_eval_sqlsq( sccb->final_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */ , pu, py,
                          NULL /* selected.db */  ) );
  DUF_ENDR( r );
}
