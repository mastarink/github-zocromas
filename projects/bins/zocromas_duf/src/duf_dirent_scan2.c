#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

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
 *      for directory                - scan_dirent_dir2
 *      for other (~ regular) entry  - scan_dirent_reg2
 * 3. up one level
 *
 * */
static int
duf_scan_direntry2_here( duf_depthinfo_t * pdi, duf_scan_hook2_dirent_t scan_dirent_reg2, duf_scan_hook2_dirent_t scan_dirent_dir2 )
{
  DEBUG_STARTR( r );

  r = duf_statat_dh( duf_levinfo_pdh( pdi ), duf_levinfo_pdh_up( pdi ), duf_levinfo_itemname( pdi ) );

  if ( r >= 0 )
  {
    duf_scan_hook2_dirent_t scanner;

    scanner = duf_levinfo_is_leaf( pdi ) ? scan_dirent_reg2 : scan_dirent_dir2;
    /* sccb->dirent_file_scan_before2 -- duf_scan_hook2_dirent_t */
    /* sccb->dirent_dir_scan_before2 -- duf_scan_hook2_dirent_t */
    if ( scanner )
      r = ( scanner ) (  /* pstmt, */ duf_levinfo_itemname( pdi ), duf_levinfo_stat( pdi ), /* duf_levinfo_dirid_up( pdi ) , */ pdi );
  }
  else if ( r == DUF_ERROR_STATAT_ENOENT )
  {
    DUF_SHOW_ERROR( "No such entry %s/%s", duf_levinfo_path( pdi ), duf_levinfo_itemname( pdi ) );
    r = DUF_ERROR_STAT;
  }
  DEBUG_ENDR( r );
}

/*
 * called for each direntry at dir from current pdi level
 *
 * 1. down 1 level
 * 2. call. get stat
 * 3. call
 *      for directory                - scan_dirent_dir2
 *      for other (~ regular) entry  - scan_dirent_reg2
 * 4. up one level
 *
 * */
static int
duf_scan_direntry2_lower( struct dirent *de, duf_depthinfo_t * pdi,
                          duf_scan_hook2_dirent_t scan_dirent_reg2, duf_scan_hook2_dirent_t scan_dirent_dir2 )
{
  DEBUG_STARTR( r );

  r = duf_levinfo_godown( pdi, 0, de->d_name, 0 /* ndirs */ , 0 /* nfiles */ , de->d_type != DT_DIR /* is_leaf */  );
  if ( r >= 0 )
    r = duf_scan_direntry2_here( pdi, scan_dirent_reg2, scan_dirent_dir2 );
  duf_levinfo_goup( pdi );

  DEBUG_ENDR( r );
}

static int
_duf_scan_dirents2( duf_depthinfo_t * pdi, duf_scan_hook2_dirent_t scan_dirent_reg2, duf_scan_hook2_dirent_t scan_dirent_dir2 )
{
  DEBUG_STARTR( r );

  int nlist = 0;
  struct dirent **list = NULL;

  DUF_TRACE( scan, 2, "dirID=%llu; scandir dfname:[%s :: %s]", duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemname( pdi ) );
  nlist = scandirat( duf_levinfo_dfd( pdi ), ".", &list, duf_direntry_filter, alphasort );
  DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), duf_levinfo_itemname_q( pdi, "nil" ), nlist,
             duf_levinfo_dfd( pdi ) );

  if ( nlist >= 0 )
  {
    for ( int il = 0; il < nlist; il++ )
    {
      /*
       * call for each direntry
       *   for directory                - scan_dirent_dir2
       *   for other (~ regular) entry  - scan_dirent_reg2
       * */
      DOR(r , duf_scan_direntry2_lower( list[il], pdi, scan_dirent_reg2, scan_dirent_dir2 ));

      if ( list[il] )
        free( list[il] );
    }
    DUF_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemname( pdi ) );
    if ( list )
      free( list );
    DUF_TEST_R( r );
  }
  else
  {
    int errorno = errno;

    if ( !duf_levinfo_path( pdi ) )
      r = DUF_ERROR_PATH;
    else if ( errorno == EACCES )
      r = 0;
    else
    {
      DUF_ERRSYSE( errorno, "path '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemname( pdi ) );
      r = DUF_ERROR_SCANDIR;
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
 *               - for directory                - scan_dirent_dir2
 *               - for other (~ regular) entry  - scan_dirent_reg2
 *
 * pdi
 * scan_dirent_reg2 - dir entry scanner function
 * scan_dirent_dir2 - reg (file) entry scanner function
 * */
int
duf_scan_dirents2(  /* duf_sqlite_stmt_t * pstmt, */ duf_depthinfo_t * pdi,
                   duf_scan_hook2_dirent_t scan_dirent_reg2, duf_scan_hook2_dirent_t scan_dirent_dir2 )
{
  DEBUG_STARTR( r );
  const struct stat *pst_parent;

  pst_parent = duf_levinfo_stat( pdi );

/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_parent || !( S_ISDIR( pst_parent->st_mode ) ) )
  {
/* no such entry */
    DUF_SHOW_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemname( pdi ) );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemname( pdi ) );
    r = DUF_ERROR_STAT;
  }
  else
    r = _duf_scan_dirents2( pdi, scan_dirent_reg2, scan_dirent_dir2 );
  DEBUG_ENDR( r );
}
