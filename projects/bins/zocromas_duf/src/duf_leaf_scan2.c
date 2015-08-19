#include <assert.h>
#include <string.h>
#include <stddef.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* DUF_TRACE */

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_item_scan2.h"
#include "duf_dirent_scan2.h"


#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_levinfo_openclose.h"

/* #include "duf_uni_scan2.h" */
#include "duf_sccb_eval_dirs.h"


#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_leaf_scan2.h"
/* ###################################################################### */



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
duf_str_cb2_leaf_scan_fd( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  /* int dfd; */
  PDI->items.total++;
  PDI->items.files++;
  /* dfd = duf_levinfo_dfd( PDI ); */
#if 1
  if ( !duf_levinfo_opened_dh( PDI ) )
#else
  if ( !duf_levinfo_dfd( PDI ) )
#endif
  {
    DOR_NOE( r, duf_levinfo_openat_dh( PDI ), DUF_ERROR_FS_DISABLED );
  }

  assert( r < 0 || duf_levinfo_opened_dh( PDI ) || duf_levinfo_item_deleted( PDI ) );
  {
    duf_scan_hook2_file_t scanner = duf_levinfo_item_deleted( PDI ) ? SCCB->leaf_scan_fd2_deleted : SCCB->leaf_scan_fd2;

    if ( scanner )
      DOR_NOE( r, scanner( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
  }                             /* DUF_CLEAR_ERROR( r, DUF_ERROR_FS_DISABLED ); */
  DEBUG_ENDR( r );
}

/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *
 *  - pstmt - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
int
duf_str_cb2_leaf_scan( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
#endif
  PDI->items.total++;
  PDI->items.files++;
  DUF_TRACE( fs, 0, "@ %d:%s ", duf_levinfo_item_deleted( PDI ), duf_levinfo_itemshowname( PDI ) );
#if 0
  if ( duf_levinfo_item_deleted( PDI ) )
  {
    DUF_SFIELD2( filename );
    if ( SCCB->leaf_scan2_deleted )
      DOR( r, SCCB->leaf_scan2_deleted( pstmt, PDI ) );
    DUF_TRACE( deleted, 0, "DELETED '%s%s'", duf_levinfo_path( PDI ), filename );
  }
  else
  {
    if ( SCCB->leaf_scan2 )
      DOR( r, SCCB->leaf_scan2( pstmt, PDI ) );
  }
#else
  {
    duf_scan_hook2_file_t scanner = duf_levinfo_item_deleted( PDI ) ? SCCB->leaf_scan2_deleted : SCCB->leaf_scan2;

    if ( scanner )
      DOR_NOE( r, scanner( pstmt, PDI ), DUF_ERROR_FS_DISABLED );
  }
#endif
  DEBUG_ENDR( r );
}
