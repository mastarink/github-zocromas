#undef MAS_TRACING
#include <string.h>

#include <dirent.h>
#include <errno.h>

#include "duf_maintenance.h"

#include "duf_config.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_dirent.h"

#include "duf_sccb_scanstage.h"
#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval_fs.h"
/* ###################################################################### */

/* 20151027.144614 */
static int
duf_sccbh_eval_fs_w_scanner_here( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh,
                                  duf_scanner_t scanner )
{
  DUF_STARTR( r );

  DOR( r, duf_levinfo_statat_dh( PDI ) );

  if ( DUF_NOERROR( r ) )
  {
    /* call hook frmo mod_ */
    /* if ( scanner ) */
    {
      sccbh->current_scanner = scanner;
      DUF_TRACE( scan, 2, "@@@@@@@%s +%d :: %s | %s", duf_nodetype_name( duf_levinfo_node_type( PDI ) ), duf_pdi_depth( PDI ),
                 duf_levinfo_relpath( PDI ), duf_levinfo_itemtruename( PDI ) );
      DOR( r, ( scanner ) ( NULL /* pstmt */ , PDI ) );
      {
        duf_levinfo_t *pli = NULL;

        pli = duf_levinfo_ptr_up( PDI );
        if ( pli )
        {
          if ( duf_levinfo_node_type( PDI ) == DUF_NODE_NODE )
            pli->scanned_childs.nodes++;
          else if ( duf_levinfo_node_type( PDI ) == DUF_NODE_LEAF )
            pli->scanned_childs.leaves++;
          else
          {
            assert( 0 );
          }
        }
      }
      assert( sccbh->current_node_type == DUF_NODE_FS );
      if ( sccbh->atom_cb )     /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, NULL /* pstmt */ , scanner, DUF_NODE_FS, r );
    }
  }
  else if ( DUF_IS_ERROR_N( r, DUF_ERROR_STATAT_ENOENT ) )
  {
    /* DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( PDI ), duf_levinfo_itemshowname( PDI ) ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, "No such entry %s/%s", duf_levinfo_path( PDI ), duf_levinfo_itemshowname( PDI ) );
  }
  DUF_ENDR( r );
}

/* 20151027.104729 */
static int
duf_sccbh_eval_fs_direntry( struct dirent *de, duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED,
                            duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  duf_node_type_t nt;

  nt = ( de->d_type == DT_DIR ) ? DUF_NODE_NODE : DUF_NODE_LEAF;
/* --> */
  DOR( r, duf_levinfo_godown( PDI, de->d_name, nt ) );
  {
    duf_scanner_t scanner;


    scanner = duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt );
    /* assert(duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt )); */
    /* T( "SCANNER: %d : %s", duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt ) ? 1 : 0, duf_uni_scan_action_title( SCCB ) ); */
    if ( scanner )
    {
      DUF_TRACE( scan, 2, "@@@@%s +%d :: %s | %-17s", duf_nodetype_name( duf_levinfo_node_type( PDI ) ), duf_pdi_depth( PDI ),
                 duf_levinfo_relpath( PDI ), duf_levinfo_itemtruename( PDI ) );
      DOR( r, duf_sccbh_eval_fs_w_scanner_here( scanstage, pstmt_unused, sccbh, scanner ) );
    }
  }
/* <-- */
  DOR( r, duf_levinfo_goup( PDI ) );

  DUF_ENDR( r );
}

/* 20151013.130021 */
static int DUF_UNUSED
duf_sccbh_eval_fs_w2scanners_sd( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  int ry = 0;
  struct dirent **list = NULL;

  /* T( "@@@@%s", duf_levinfo_path( PDI ) ); */
  ry = scandirat( duf_levinfo_dfd( PDI ), ".", &list, duf_direntry_filter, NULL /* alphasort */  );
  if ( ry >= 0 )
  {
    int nlist = ry;

    for ( int il = 0; il < nlist; il++ )
    {
      DUF_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      DOR( r, duf_sccbh_eval_fs_direntry( list[il], scanstage, pstmt_unused, sccbh ) );

      if ( list[il] )
        free( list[il] );
    }
    DUF_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemshowname( PDI ) );
    if ( list )
      free( list );
  }
  else
  {
    /* system level: scandirat - register errors */
    int errorno = errno;

    if ( !duf_levinfo_path( PDI ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    else if ( errorno != EACCES )
    {
      /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) ); */
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
    else
    {
      /* ??? */
      /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) ); */
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
  }
  DUF_ENDR( r );
}

static int DUF_UNUSED
duf_sccbh_eval_fs_w2scanners_rd( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  DIR *dh = NULL;

  dh = opendir( duf_levinfo_path( PDI ) );
  if ( dh )
  {
    struct dirent *de;
    int il = 0;

    while ( ( de = readdir( dh ) ) )
    {
      if ( duf_direntry_filter( de ) )
      {
        DUF_TRACE( scan, 2, "@@@@@@dirent %s", de->d_name );
        DOR( r, duf_sccbh_eval_fs_direntry( de, scanstage, pstmt_unused, sccbh ) );
        il++;
      }
    }
    closedir( dh );
  }
  else
  {
    int errorno = errno;

    if ( !duf_levinfo_path( PDI ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    else if ( errorno != EACCES )
    {
      /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) ); */
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
    else
    {
      /* ??? */
      /* DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) ); */
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", errorno, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
  }
  DUF_ENDR( r );
}

/* 20151013.130037 */
DUF_WRAPSTATIC int
duf_sccbh_eval_fs( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );

  if ( SCCB->dirent_dir_scan_before2 || SCCB->dirent_file_scan_before2 )
  {
    /* assert( PDI->opendir == 1 ); */
    duf_pdi_set_opendir( PDI, 1 ); /* TODO */
    DUF_TRACE( scan, 2, "@@@@@@@´%s´ : %s to scan FS at %s : %s", duf_scanstage_name( scanstage ),
               duf_nodetype_name( duf_levinfo_node_type( PDI ) ), duf_levinfo_relpath( PDI ), duf_levinfo_itemtruename( PDI ) );
    {
      DOR_LOWERE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_STATAT_ENOENT );
      assert( DUF_IS_ERROR( r ) || duf_levinfo_stat( PDI ) );

/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
      if ( S_ISDIR( duf_levinfo_stat_mode( PDI ) ) )
      {
        DOR( r, duf_levinfo_if_openat_dh( PDI ) );
        sccbh->current_node_type = DUF_NODE_FS;
#if 0
        DOR( r, duf_sccbh_eval_fs_w2scanners_sd( scanstage, pstmt_unused, sccbh ) );
#else
        DOR( r, duf_sccbh_eval_fs_w2scanners_rd( scanstage, pstmt_unused, sccbh ) );
#endif
      }
      else
      {
        /* system level: S_ISDIR - register errors */
/* no such entry */
        /* DUF_SHOW_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) ); */
        /* TODO mark as absent or remove from db */

        /* DUF_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemshowname( PDI ) ); */
        DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, "No such entry '%s'/'%s'", duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
      }
    }
  }
  DUF_ENDR( r );
}

/* 20151027.114354 */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 *
 * call from duf_eval_sccbh_all(_wrap)
 *
 * fn of type: duf_str_cb2_t
 * */
int DUF_WRAPPED( duf_sccbh_eval_fs ) ( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  assert( PDI );
  if ( SCCB->dirent_dir_scan_before2 || SCCB->dirent_file_scan_before2 )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>>q +dirent" );
    DUF_TRACE( scan, 4, "@scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

    DUF_TRACE( sccbh, 2, "(%s) stat (%s) %s", mas_error_name_i( r ), duf_uni_scan_action_title( SCCB ), SCCB->name );

    /* assert( duf_levinfo_dfd( PDI ) ); */
    /*
     *   -- call for each direntry
     *      - for directory                - sccb->dirent_dir_scan_before2
     *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
     * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */

    DOR( r, duf_sccbh_eval_fs( scanstage, pstmt_unused, sccbh ) );
    DUF_TRACE( sccbh, 2, "(%s) stat (%s) %s", mas_error_name_i( r ), duf_uni_scan_action_title( SCCB ), SCCB->name );
  }
  DUF_ENDR( r );
}
