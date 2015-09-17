/* 20150820.085357 */
#include <string.h>
#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"


#include "duf_sccbh_shortcuts.h"

/* ###################################################################### */
#include "duf_leaf_scan2.h"
/* ###################################################################### */



/*20150820.085324
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 *
 *  - pstmt - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
int
duf_eval_sccbh_db_leaf_fd_str_cb( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  PDI->items.total++;
  PDI->items.files++;

  DUF_TRACE(sccbh, 0, "@@@@T");
  DOR_NOE( r, duf_levinfo_if_openat_dh( PDI ), DUF_ERROR_FS_DISABLED );
  assert( r < 0 || duf_levinfo_opened_dh( PDI ) > 0 || duf_levinfo_item_deleted( PDI ) );

  DOR_NOE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_FS_DISABLED );
  assert( r < 0 || duf_levinfo_stat( PDI ) || duf_levinfo_item_deleted( PDI ) );
  {
    duf_scan_hook2_file_t scanner = duf_levinfo_item_deleted( PDI ) ? SCCB->leaf_scan_fd2_deleted : SCCB->leaf_scan_fd2;

    if ( scanner )
      DOR_NOE( r, scanner( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
  }
  DEBUG_ENDR( r );
}

/*20150820.085324
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *
 *  - pstmt - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
int
duf_eval_sccbh_db_leaf_str_cb( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  PDI->items.total++;
  PDI->items.files++;

  DUF_TRACE( fs, 0, "@ %d:%s ", duf_levinfo_item_deleted( PDI ), duf_levinfo_itemshowname( PDI ) );
  {
    duf_scan_hook2_file_t scanner = duf_levinfo_item_deleted( PDI ) ? SCCB->leaf_scan2_deleted : SCCB->leaf_scan2;

    if ( scanner )
      DOR_NOE( r, scanner( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
  }
  DEBUG_ENDR( r );
}
