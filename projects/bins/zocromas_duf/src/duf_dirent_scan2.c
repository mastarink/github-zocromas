#include <string.h>

#include <dirent.h>
#include <errno.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_dh.h"
#include "duf_dirent.h"

/* ###################################################################### */
#include "duf_dirent_scan2.h"
/* ###################################################################### */

/* 20150901.121041 */
static int
duf_scan_fs_with_scanner_here( duf_depthinfo_t * pdi, duf_scanner_t scanner )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_statat_dh( pdi ) );

  if ( DUF_NOERROR( r ) )
  {
    /* call hook frmo mod_ */
    if ( scanner )
      DOR( r, ( scanner ) ( NULL /* pstmt */ , pdi ) );
  }
  else if ( DUF_IS_ERROR_N( r, DUF_ERROR_STATAT_ENOENT ) )
  {
    /* DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_STAT , "No such entry %s/%s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ));
  }
  DEBUG_ENDR( r );
}

static int
duf_scan_fs_with2scanners_lower( struct dirent *de, duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );
  int is_leaf;

  is_leaf = ( de->d_type != DT_DIR );
/* --> */
  DOR( r, duf_levinfo_godown( pdi, de->d_name, is_leaf ) );
  {
    DOR( r, duf_scan_fs_with_scanner_here( pdi, is_leaf ? scanner_dirent_reg2 : scanner_dirent_dir2 ) );
  }
/* <-- */
  DOR( r, duf_levinfo_goup( pdi ) );

  DEBUG_ENDR( r );
}

/* 20150901.120422 */
static int
duf_scandirat_with2scanners( duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );
  int ry = 0;
  struct dirent **list = NULL;

  ry = scandirat( duf_levinfo_dfd( pdi ), ".", &list, duf_direntry_filter, alphasort );
  if ( ry >= 0 )
  {
    int nlist = ry;

    for ( int il = 0; il < nlist; il++ )
    {
      DUF_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      DOR( r, duf_scan_fs_with2scanners_lower( list[il], pdi, scanner_dirent_reg2, scanner_dirent_dir2 ) );

      if ( list[il] )
        free( list[il] );
    }
    DUF_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemshowname( pdi ) );
    if ( list )
      free( list );
  }
  else
  {
    /* system level: scandirat - register errors */
    int errorno = errno;

    if ( !duf_levinfo_path( pdi ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    else if ( errorno != EACCES )
    {
      DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
    }
    else
    {
      /* ??? */
      DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", r, duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCANDIR ,"(%d) path '%s'/'%s'", r, duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ));
    }
  }
  DEBUG_ENDR( r );
}

/* 20150901.120412 */
static int
_duf_scan_fs_with2scanners( duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_if_openat_dh( pdi ) );
  DOR( r, duf_scandirat_with2scanners( pdi, scanner_dirent_reg2, scanner_dirent_dir2 ) );

  DEBUG_ENDR( r );
}

/* 20150901.120449 */
int
duf_scan_fs_with2scanners( duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );

  assert( pdi );

  /* TODO */
  DOR_LOWERE( r, duf_levinfo_if_statat_dh( pdi ) ,DUF_ERROR_STATAT_ENOENT);

  DUF_TRACE( scan, 4, "scan dirent hooks d:%d; r:%d", scanner_dirent_dir2 ? 1 : 0, scanner_dirent_reg2 ? 1 : 0 );
/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( S_ISDIR( duf_levinfo_stat_mode( pdi ) ) )
  {
    DOR( r, _duf_scan_fs_with2scanners( pdi, scanner_dirent_reg2, scanner_dirent_dir2 ) );
  }
  else
  {
    /* system level: S_ISDIR - register errors */
/* no such entry */
    /* DUF_SHOW_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) ); */
    /* TODO mark as absent or remove from db */

    /* DUF_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemshowname( pdi ) ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, "No such entry '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
  }
  DEBUG_ENDR( r );
}
