/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stdlib.h>                                                  /* free */

#include <dirent.h>                                                  /* struct dirent  */
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_debug_defs.h"                                          (* DUF_WRAPSTATIC; DUF_WRAPPED ...  ✗ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_dirent.h"

#include "duf_sccb_scanstage.h"
#include "duf_sccb_structs.h"
#include "duf_sccbh_shortcuts.h"


#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_sccbh_eval_fs.h"
/* ###################################################################### */

/* 20151027.144614 */
static
SR( SCCBH, sccbh_eval_fs_w_scanner_here, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED,
    duf_scanner_t scanner )
{
/* DUF_STARTR( r ); */

  CR( levinfo_statat_dh, H_PDI );

  if ( QNOERR )
  {
  /* call hook frmo mod_ */
  /* if ( scanner ) */
    {
      /* sccbh->current_scanner = scanner; */
      MAST_TRACE( scan, 2, "@@@@@@@%s +%d :: %s | %s", duf_nodetype_name( duf_levinfo_node_type( H_PDI ) ), duf_pdi_depth( H_PDI ),
                  duf_levinfo_relpath( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
      CRV( ( scanner ), NULL /* pstmt */ , H_PDI, sccbh );
      {
        duf_levinfo_t *pli = NULL;

        pli = duf_levinfo_ptr_up( H_PDI );
        if ( pli )
        {
          if ( duf_levinfo_node_type( H_PDI ) == DUF_NODE_NODE )
            pli->scanned_childs.nodes++;
          else if ( duf_levinfo_node_type( H_PDI ) == DUF_NODE_LEAF )
            pli->scanned_childs.leaves++;
          else
          {
            assert( 0 );
          }
        }
      }
      assert( sccbh->assert__current_node_type == DUF_NODE_FS );
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, NULL /* pstmt */ , scanstage, scanner, DUF_NODE_FS, QERRIND );
    }
  }
  else if ( QISERR_N( STATAT_ENOENT ) )
  {
  /* DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) ); */
    ERRMAKE_M( STAT, "No such entry %s/%s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_fs_w_scanner_here, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED,
      duf_scanner_t scanner );
}

/* 20151027.104729 */
SR( SCCBH, sccbh_eval_fs_direntry, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, struct dirent *de,
    duf_scanstage_t scanstage MAS_UNUSED )
{
/* DUF_STARTR( r ); */
  duf_node_type_t nt;

  nt = ( de->d_type == DT_DIR ) ? DUF_NODE_NODE : DUF_NODE_LEAF;
/* --> */
  CR( levinfo_godown, H_PDI, de->d_name, nt );
  {
    duf_scanner_t scanner;

    scanner = duf_scanstage_scanner( H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt );
  /* assert(duf_scanstage_scanner( H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt )); */
  /* QT( "SCANNER: %d : %s", duf_scanstage_scanner( H_SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt ) ? 1 : 0, duf_uni_scan_action_title( H_SCCB ) ); */
    if ( scanner )
    {
      MAST_TRACE( scan, 2, "@@@@%s +%d :: %s | %-17s", duf_nodetype_name( duf_levinfo_node_type( H_PDI ) ), duf_pdi_depth( H_PDI ),
                  duf_levinfo_relpath( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
      CR( sccbh_eval_fs_w_scanner_here, sccbh, pstmt_unused, scanstage, scanner );
    }
  }
/* <-- */
  CR( levinfo_goup, H_PDI );

/* DUF_ENDR( r ); */

  ER( SCCBH, sccbh_eval_fs_direntry, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, struct dirent *de, duf_scanstage_t scanstage );
}

/* 20151013.130021 */
SR( SCCBH, sccbh_eval_fs_w2scanners_sd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh )
{
/* DUF_STARTR( r ); */
  int ry = 0;
  struct dirent **list = NULL;

/* QT( "@@@@%s", duf_levinfo_path( H_PDI ) ); */
  ry = scandirat( duf_levinfo_dfd( H_PDI ), ".", &list, duf_direntry_filter, NULL /* alphasort */  );
  if ( ry >= 0 )
  {
    int nlist = ry;

    for ( int il = 0; il < nlist; il++ )
    {
      MAST_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      CR( sccbh_eval_fs_direntry, sccbh, pstmt_unused, list[il], scanstage );

      if ( list[il] )
        free( list[il] );
    }
    MAST_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemshowname( H_PDI ) );
    if ( list )
      free( list );
  }
  else
  {
  /* system level: scandirat - register errors */
    int errorno = errno;

    if ( !duf_levinfo_path( H_PDI ) )
      ERRMAKE( PATH );
    else if ( errorno != EACCES )
    {
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) );
    }
    else
    {
    /* ??? */
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) );
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_fs_w2scanners_sd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh );
}

static
SR( SCCBH, sccbh_eval_fs_w2scanners_rd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh )
{
/* DUF_STARTR( r ); */
  DIR *dh = NULL;

  dh = opendir( duf_levinfo_path( H_PDI ) );
  if ( dh )
  {
    struct dirent *de;
    int il = 0;

    while ( ( de = readdir( dh ) ) )
    {
      if ( duf_direntry_filter( de ) )
      {
        MAST_TRACE( scan, 2, "@@@@@@dirent %s", de->d_name );
        CR( sccbh_eval_fs_direntry, sccbh, pstmt_unused, de, scanstage );
        il++;
      }
    }
    closedir( dh );
  }
  else
  {
    int errorno = errno;

    if ( !duf_levinfo_path( H_PDI ) )
      ERRMAKE( PATH );
    else if ( errorno != EACCES )
    {
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) );
    }
    else
    {
    /* ??? */
    /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) ); */
      ERRMAKE_M( SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) );
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_fs_w2scanners_rd, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_sccb_handle_t * sccbh );
}

/* 20151013.130037 */
static
SR( SCCBH, sccbh_eval_fs_in, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  if ( H_SCCB->dirent_dir_scan_before2 || H_SCCB->dirent_file_scan_before2 )
  {
  /* assert( duf_pdi_opendir(H_PDI) == 1 ); */
    duf_pdi_set_opendir( H_PDI, 1 );                                 /* TODO */
    MAST_TRACE( scan, 2, "@@@@@@@´%s´ : %s to scan FS at %s : %s", duf_scanstage_name( scanstage ),
                duf_nodetype_name( duf_levinfo_node_type( H_PDI ) ), duf_levinfo_relpath( H_PDI ), duf_levinfo_itemtruename( H_PDI ) );
    {
      {
	ERRLOWER(STATAT_ENOENT);
        CR( levinfo_if_statat_dh, H_PDI );
	ERRUPPER(STATAT_ENOENT);
      }
      assert( QISERR || duf_levinfo_stat( H_PDI ) );

/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
      if ( S_ISDIR( duf_levinfo_stat_mode( H_PDI ) ) )
      {
        CR( levinfo_if_openat_dh, H_PDI );
        sccbh->assert__current_node_type = DUF_NODE_FS;
#if 0
        DOR( r, duf_sccbh_eval_fs_w2scanners_sd( scanstage, pstmt_unused, sccbh ) );
#else
        CR( sccbh_eval_fs_w2scanners_rd, scanstage, pstmt_unused, sccbh );
#endif
      }
      else
      {
      /* system level: S_ISDIR - register errors */
/* no such entry */
      /* DUF_SHOW_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) ); */
      /* TODO mark as absent or remove from db */

      /* MAST_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemshowname( H_PDI ) ); */
        ERRMAKE_M( STAT, "No such entry '%s'/'%s'", duf_levinfo_path_q( H_PDI, "?" ), duf_levinfo_itemshowname( H_PDI ) );
      }
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_fs_in, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED );
}

/* 20151027.114354 */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 *
 * call from duf_sccbh_eval_all(_wrap)
 *
 * fn of type: duf_str_cb2_t
 * */
SR( SCCBH, sccbh_eval_fs, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED )
{
/* DUF_STARTR( r ); */
  assert( H_PDI );
  if ( H_SCCB->dirent_dir_scan_before2 || H_SCCB->dirent_file_scan_before2 )
  {
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>>q +dirent" );
    MAST_TRACE( scan, 4, "@scan dirent by %5llu:%s; %s", duf_levinfo_dirid( H_PDI ), duf_uni_scan_action_title( H_SCCB ), duf_levinfo_path( H_PDI ) );

    MAST_TRACE( sccbh, 2, "(%s) stat (%s) %s", QERRNAME, duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );

  /* assert( duf_levinfo_dfd( H_PDI ) ); */
  /*
   *   -- call for each direntry
   *      - for directory                - sccb->dirent_dir_scan_before2
   *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
   * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */

    CR( sccbh_eval_fs_in, sccbh, pstmt_unused, scanstage );
    MAST_TRACE( sccbh, 2, "(%s) stat (%s) %s", QERRNAME, duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_fs, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED );
}
