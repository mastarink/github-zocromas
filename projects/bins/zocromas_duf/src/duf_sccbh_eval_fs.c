#include <string.h>

#include <dirent.h>
#include <errno.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_dirent.h"

#include "duf_sccb_scanstage.h"
#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval_fs.h"
/* ###################################################################### */

/* 20151013.130012 */
static int
duf_sccbh_eval_fs_with_scanner_here( duf_sccb_handle_t * sccbh, duf_scanner_t scanner )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_statat_dh( PDI ) );

  if ( DUF_NOERROR( r ) )
  {
    /* call hook frmo mod_ */
    if ( scanner )
      DOR( r, ( scanner ) ( NULL /* pstmt */ , PDI ) );
  }
  else if ( DUF_IS_ERROR_N( r, DUF_ERROR_STATAT_ENOENT ) )
  {
    /* DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( PDI ), duf_levinfo_itemshowname( PDI ) ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, "No such entry %s/%s", duf_levinfo_path( PDI ), duf_levinfo_itemshowname( PDI ) );
  }
  DEBUG_ENDR( r );
}

/* 20151013.130015 */
static int
duf_sccbh_eval_fs_direntry( struct dirent *de,
                            duf_sccb_handle_t * sccbh /*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 */  )
{
  DEBUG_STARTR( r );
  duf_node_type_t nt;

  /* int is_leaf; */

  nt = ( de->d_type == DT_DIR ) ? DUF_NODE_NODE : DUF_NODE_LEAF;
  /* is_leaf = ( de->d_type != DT_DIR ); */
/* --> */
  DOR( r, duf_levinfo_godown( PDI, de->d_name, nt == DUF_NODE_LEAF /* is_leaf */  ) );
  {
#if 0
    assert( duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, DUF_NODE_LEAF ) == scanner_dirent_reg2 );
    assert( duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, DUF_NODE_NODE ) == scanner_dirent_dir2 );    
    /* assert( duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt ) == ( is_leaf ? scanner_dirent_reg2 : scanner_dirent_dir2 ) ); */
#endif
#if 0
    DOR( r, duf_sccbh_eval_fs_with_scanner_here( sccbh, is_leaf ? scanner_dirent_reg2 : scanner_dirent_dir2 ) );
#else
    DOR( r, duf_sccbh_eval_fs_with_scanner_here( sccbh, duf_scanstage_scanner( SCCB, DUF_SCANSTAGE_FS_ITEMS, 0, nt ) ) );
#endif
  }
/* <-- */
  DOR( r, duf_levinfo_goup( PDI ) );

  DEBUG_ENDR( r );
}

/* 20151013.130021 */
static int
duf_sccbh_eval_fs_dirat_with2scanners( duf_sccb_handle_t * sccbh /*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 */  )
{
  DEBUG_STARTR( r );
  int ry = 0;
  struct dirent **list = NULL;

  ry = scandirat( duf_levinfo_dfd( PDI ), ".", &list, duf_direntry_filter, alphasort );
  if ( ry >= 0 )
  {
    int nlist = ry;

    for ( int il = 0; il < nlist; il++ )
    {
      DUF_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      DOR( r, duf_sccbh_eval_fs_direntry( list[il], sccbh /*, scanner_dirent_reg2, scanner_dirent_dir2 */  ) );

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
      DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
    else
    {
      /* ??? */
      DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( PDI, "?" ), duf_levinfo_itemshowname( PDI ) );
    }
  }
  DEBUG_ENDR( r );
}

/* 20151013.130028 */
static int
_duf_sccbh_eval_fs_with2scanners( duf_sccb_handle_t * sccbh /*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 */  )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_if_openat_dh( PDI ) );
  DOR( r, duf_sccbh_eval_fs_dirat_with2scanners( sccbh /*, scanner_dirent_reg2, scanner_dirent_dir2 */  ) );

  DEBUG_ENDR( r );
}

/* 20151013.130037 */
int
duf_sccbh_eval_fs_with2scanners( duf_sccb_handle_t * sccbh /*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 */  )
{
  DEBUG_STARTR( r );

  assert( PDI );

  /* TODO */
  DOR_LOWERE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_STATAT_ENOENT );

#if 0
  DUF_TRACE( scan, 4, "scan dirent hooks d:%d; r:%d", scanner_dirent_dir2 ? 1 : 0, scanner_dirent_reg2 ? 1 : 0 );
#endif
/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( S_ISDIR( duf_levinfo_stat_mode( PDI ) ) )
  {
    DOR( r, _duf_sccbh_eval_fs_with2scanners( sccbh /*, scanner_dirent_reg2, scanner_dirent_dir2 */  ) );
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
  DEBUG_ENDR( r );
}
