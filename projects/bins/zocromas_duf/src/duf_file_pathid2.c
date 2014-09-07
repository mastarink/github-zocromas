#include <stdio.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>



#include "duf_maintenance.h"
#include "duf_scan_types.h"

#include "duf_config_ref.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_options.h"


#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_item_scan1.h" */
#include "duf_item_scan2.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_file_pathid2.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirID (i.e. children of dirID) with corresponding args
 *
 * known str_cb for duf_scan_files_by_di:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd
 *
 * str_cb2 (sub-item scanner):
 *       duf_scan_dirs_by_pdi_maxdepth
 *       duf_str_cb2_leaf_scan
 *       duf_str_cb2_scan_file_fd
 * */
/* int                                                                                                                      */
/* duf_scan_files_by_dirid2( duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )                    */
/* {                                                                                                                        */
/*   DEBUG_STARTR( r );                                                                                                     */
/*   if ( sccb && sccb->leaf.selector2 )                                                                                    */
/*   {                                                                                                                      */
/*     DOR( r, duf_scan_db_items2( DUF_NODE_LEAF, str_cb2, pdi, sccb (*, sccb->leaf.selector2, sccb->leaf.fieldset *)  ) ); */
/*   }                                                                                                                      */
/*   else                                                                                                                   */
/*   {                                                                                                                      */
/*     DUF_ERROR( "sccb->leaf.selector2 must be set for %s", sccb->title );                                                 */
/*     r = DUF_ERROR_PTR;                                                                                                   */
/*   }                                                                                                                      */
/*                                                                                                                          */
/*   DEBUG_ENDR( r );                                                                                                       */
/* }                                                                                                                        */
