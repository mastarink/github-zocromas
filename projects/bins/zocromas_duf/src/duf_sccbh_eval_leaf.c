/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb.h"
#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"

#include "duf_sccbh_eval.h"
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
SR( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */

#if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
#endif
  {
    ERRLOWER( FS_DISABLED );
    CR( levinfo_if_openat_dh, H_PDI );
    {
      ERRLOWER( STATAT_ENOENT );
      CR( levinfo_if_statat_dh, H_PDI );
      ERRUPPER( STATAT_ENOENT );
    }
    ERRUPPER( FS_DISABLED );
  }
/* DOR_LOWERE( r, duf_levinfo_if_openat_dh( H_PDI ), DUF_ERROR_FS_DISABLED );                          */
/* DOR_LOWERE( r, duf_levinfo_if_statat_dh( H_PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT ); */
#if 1
  if ( duf_levinfo_deleted( H_PDI ) )
  {
    ERRCLEAR( OPENAT_ENOENT );
    ERRCLEAR( STATAT_ENOENT );
  }
#endif
  for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
  {
    duf_scanner_t scanner = NULL;

    scanner = duf_scanstage_scanner( H_SCCB, scanstage, duf_levinfo_deleted( H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );
#if 0
    /* sccbh->current_scanner = scanner; */
      if ( scanner )
      {
        CRV( scanner, pstmt, H_PDI );
        assert( sccbh->assert__current_node_type == DUF_NODE_LEAF );
        if ( sccbh->atom_cb )                                        /* atom is fs-direntry(dir or reg) or item(node or leaf) */
          sccbh->atom_cb( sccbh, pstmt, scanstage, scanner, DUF_NODE_LEAF, QERRIND );
      }
      else
      {
# if 0
        DUF_SHOW_ERRORO( "no scanner at %s; deleted:%d for <path>/%s%s... ...", duf_uni_scan_action_title( H_SCCB ), duf_levinfo_deleted( H_PDI ),
                         duf_levinfo_relpath( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );
# else
        ERRMAKE_M( UNKNOWN, "no scanner at %s; deleted:%d for <path>/%s%s... ...", duf_uni_scan_action_title( H_SCCB ),
                   duf_levinfo_deleted( H_PDI ), duf_levinfo_relpath( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );
# endif
      }
#else
      CR( sccbh_call_scanner, sccbh, pstmt, scanstage, scanner, DUF_NODE_LEAF );
#endif
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBI = 0;
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage );
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
SR( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */

#if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
#endif
  for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
  {
    duf_scanner_t scanner = NULL;

    scanner = duf_scanstage_scanner( H_SCCB, scanstage, duf_levinfo_deleted( H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );
#if 0
      if ( scanner )
        CRV( scanner, pstmt, H_PDI );
#else
      CR( sccbh_call_scanner, sccbh, pstmt, scanstage, scanner, DUF_NODE_LEAF );
#endif
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBI = 0;
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage );
}
