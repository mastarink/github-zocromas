#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"

#include "duf_dh.h"
#include "duf_dirent.h"

/* ###################################################################### */
#include "duf_dirent_scan2.h"
/* ###################################################################### */

/*
 * called for each direntry at dir from current pdi level
 *
 * 1. call. get stat
 * 2. call
 *      for directory                - scanner_dirent_dir2
 *      for other (~ regular) entry  - scanner_dirent_reg2
 * 3. up one level
 *
 * */
static int
duf_scan_with_scanner_here( duf_depthinfo_t * pdi, duf_scanner_t scanner )
{
  DEBUG_STARTR( r );

#if 0
  /* TODO - remove from here */
  /* DOR( r, duf_statat_dh( duf_levinfo_pdh( pdi ), duf_levinfo_pdh_up( pdi ), duf_levinfo_itemshowname( pdi ) ) ); */
#else
  DOR( r, duf_levinfo_statat_dh( pdi ) );
#endif

  if ( r >= 0 )
  {
    /* call hook frmo mod_ */
    if ( scanner )
      DOR( r, ( scanner ) ( NULL /* pstmt */ , pdi ) );
  }
  else if ( r == DUF_ERROR_STATAT_ENOENT )
  {
    DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }
  DEBUG_ENDR( r );
}

/*
 * called for each direntry at dir from current pdi level
 *
 * 1. down 1 level
 * 2. call. get stat
 * 3. call
 *      for directory                - scanner_dirent_dir2
 *      for other (~ regular) entry  - scanner_dirent_reg2
 * 4. up one level
 *
 * */
static int
duf_scan_fs_with2scanners_lower( struct dirent *de, duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );
  int is_leaf;

  is_leaf = de->d_type != DT_DIR;
  DOR( r, duf_levinfo_godown( pdi, 0, de->d_name, 0 /* ndirs */ , 0 /* nfiles */ , is_leaf ) );
  DOR( r, duf_scan_with_scanner_here( pdi, is_leaf ? scanner_dirent_reg2 : scanner_dirent_dir2 ) );
  DOR( r, duf_levinfo_goup( pdi ) );

  DEBUG_ENDR( r );
}

static int
_duf_scan_fs_with2scanners( duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );

  int nlist = 0;
  struct dirent **list = NULL;

  DUF_TRACE( scan, 2, "dirID=%llu; scandir dfname:[%s :: %s]", duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  if ( !duf_levinfo_dfd( pdi ) )
    DOR( r, duf_levinfo_openat_dh( pdi ) );
  assert( duf_levinfo_dfd( pdi ) );
  nlist = scandirat( duf_levinfo_dfd( pdi ), ".", &list, duf_direntry_filter, alphasort );

#ifdef MAS_TRACING
  {
    extern duf_scan_callbacks_t duf_dummy_callbacks;

    DUF_TRACE( scan, 0, "scan dirent (nlist:%d) hooks d:%p; r:%p; %p !!", nlist, scanner_dirent_dir2, scanner_dirent_reg2,
               duf_dummy_callbacks.dirent_dir_scan_before2 );
  }
#endif

  DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), duf_levinfo_itemshowname_q( pdi, "nil" ), nlist,
             duf_levinfo_dfd( pdi ) );

  if ( nlist >= 0 )
  {
    for ( int il = 0; il < nlist; il++ )
    {
      /*
       * call for each direntry
       *   for directory                - scanner_dirent_dir2
       *   for other (~ regular) entry  - scanner_dirent_reg2
       * */
      DUF_TRACE( scan, 2, "scan dirent %d: %s", il, list[il]->d_name );
      DOR( r, duf_scan_fs_with2scanners_lower( list[il], pdi, scanner_dirent_reg2, scanner_dirent_dir2 ) );

      if ( list[il] )
        free( list[il] );
    }
    DUF_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemshowname( pdi ) );
    if ( list )
      free( list );
    DUF_TEST_R( r );
  }
  else
  {
    int errorno = errno;

    if ( !duf_levinfo_path( pdi ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    else if ( errorno == EACCES )
      r = 0;
    else
    {
      DUF_ERRSYSE( errorno, "(%d) path '%s'/'%s'", nlist, duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
      DUF_MAKE_ERROR( r, DUF_ERROR_SCANDIR );
    }
    DUF_TEST_R( r );
  }
  DEBUG_ENDR( r );
}

/*
 * 1. get entries from filesystem at dir from current pdi level
 * 2. filter them with duf_direntry_filter
 * 3. call duf_scan_direntry2 for each:
 *             -- call for each direntry
 *               - for directory                - scanner_dirent_dir2
 *               - for other (~ regular) entry  - scanner_dirent_reg2
 *
 * pdi
 * scanner_dirent_reg2 - dir entry scanner function
 * scanner_dirent_dir2 - reg (file) entry scanner function
 * */
int
duf_scan_fs_with2scanners( duf_depthinfo_t * pdi, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 )
{
  DEBUG_STARTR( r );
  /* const struct stat *pst_parent; */

  assert( pdi );

  /* pst_parent = duf_levinfo_stat( pdi ); */
  /* assert( pst_parent ); */

  if ( !duf_levinfo_stat_dev( pdi ) )
  {
    DOR( r, duf_levinfo_statat_dh( pdi ) );
  }

  assert( duf_levinfo_stat_dev( pdi ) );

  DUF_TRACE( scan, 0, "scan dirent hooks d:%d; r:%d", scanner_dirent_dir2 ? 1 : 0, scanner_dirent_reg2 ? 1 : 0 );
/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if (  /* r || !pst_parent || */ !( S_ISDIR( duf_levinfo_stat_mode( pdi ) ) ) )
  {


/* no such entry */
    DUF_SHOW_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemshowname( pdi ) );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemshowname( pdi ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }
  else
  {
    r = _duf_scan_fs_with2scanners( pdi, scanner_dirent_reg2, scanner_dirent_dir2 );
  }
  DEBUG_ENDR( r );
}
