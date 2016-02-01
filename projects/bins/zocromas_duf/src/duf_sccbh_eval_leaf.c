/* 20150820.085357 */
#include <string.h>
#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"

/* ###################################################################### */
#include "duf_sccbh_eval_leaf.h"
/* ###################################################################### */


/* 20151027.144543 */
/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 *
 *  - pstmt - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
int
duf_eval_sccbh_db_leaf_fd_str_cb( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

#if 0
  PDI->items.total++;
  PDI->items.files++;
#endif

  DOR_LOWERE( r, duf_levinfo_if_openat_dh( PDI ), DUF_ERROR_FS_DISABLED );
  DOR_LOWERE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT );
#if 1
  if ( duf_levinfo_deleted( PDI ) )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT, DUF_ERROR_STATAT_ENOENT );
#endif
  {
    duf_scan_hook2_file_func_t scanner = NULL;

#if 0
    scanner = SCCB->leaf_scan_fd2;
    if ( duf_levinfo_deleted( PDI ) )
    {
      scanner = SCCB->leaf_scan_fd2_deleted ? SCCB->leaf_scan_fd2_deleted : NULL;
    }
    assert( scanner == duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_deleted( PDI ), DUF_NODE_LEAF ) );
#else
    scanner = duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_deleted( PDI ), DUF_NODE_LEAF );
#endif
    if ( scanner )
    {
      sccbh->current_scanner = scanner;
      DOR_LOWERE( r, ( scanner ) ( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
      assert( sccbh->current_node_type == DUF_NODE_LEAF );
      if ( sccbh->atom_cb )     /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, pstmt, scanner, DUF_NODE_LEAF, r );
    }
    else
    {
      DUF_SHOW_ERRORO( "no scanner at %s; deleted:%d for <path>/%s%s... ...", duf_uni_scan_action_title( SCCB ), duf_levinfo_deleted( PDI ),
                       duf_levinfo_relpath( PDI ), duf_levinfo_itemshowname( PDI ) );
    }
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
duf_eval_sccbh_db_leaf_str_cb( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  PDI->items.total++;
  PDI->items.files++;

  {
    duf_scan_hook2_file_func_t scanner = NULL;

#if 0
    scanner = SCCB->leaf_scan2;
    if ( SCCB->leaf_scan2_deleted && duf_levinfo_if_deleted( PDI ) )
    {
      scanner = SCCB->leaf_scan2_deleted;
      DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT, DUF_ERROR_STATAT_ENOENT );
    }
    assert( scanner == duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_deleted( PDI ), DUF_NODE_LEAF ) );
#else
    scanner = duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_deleted( PDI ), DUF_NODE_LEAF );
#endif
    if ( scanner )
      DOR_LOWERE( r, ( scanner ) ( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
  }
  DEBUG_ENDR( r );
}
