#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

/* #include "duf_config_ref.h" */

#include "duf_pdi.h"
/* #include "duf_levinfo.h" */

/* #include "duf_sql.h" */
/* #include "duf_sql2.h" */

/* #include "duf_sccb.h" */
#include "duf_option_names.h"

#include "duf_dir_scan2.h"

/* ###################################################################### */
#include "duf_uni_scan2.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/


/* duf_str_cb(1?)_uni_scan_dir:
 * this is callback of type: duf_str_cb_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid(2?) + pdi (built from str_cb_udata) with duf_str_cb(1?)_uni_scan_dir
 *       for each <dir> record by dirID (i.e. children of dirID) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb(1?)_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
int
duf_scan_dirs_by_pdi_maxdepth( duf_sqlite_stmt_t * pstmt_selector, DSCCBX )
{
  DEBUG_STARTR( r );

  assert( PDI );

  /* if ( duf_pdi_is_good_depth( PDI ) ) */
  {
    DUF_TRACE( explain, duf_pdi_reldepth( PDI ) <= 1 ? 3 : 4, "cb2; option %s value %d; depth %d", DUF_OPT_NAME( MAXRELDEPTH ),
               PDI->pu->max_rel_depth, duf_pdi_reldepth( PDI ) );

    /* recursively calls this `duf_scan_dirs_by_pdi_maxdepth` */
    DOR( r, duf_scan_dirs_by_pdi_wrap( pstmt_selector, SCCBX ) );
  }
  /* else                                                                                                               */
  /* {                                                                                                                  */
  /*   DUF_TRACE( explain, 0, "no depth condition by %s value %d", DUF_OPT_NAME( MAXRELDEPTH ), PDI->u.max_rel_depth ); */
  /* }                                                                                                                  */

  DEBUG_ENDR( r );
}
