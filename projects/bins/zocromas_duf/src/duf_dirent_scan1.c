#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_config_ref.h"

#include "duf_levinfo.h"
#include "duf_dirent.h"


#include "duf_sql_defs.h"
#include "duf_sql.h"

#include "duf_sql_field.h"
#include "duf_sql_field1.h"


#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_dirent_scan1.h"
/* ###################################################################### */


static int
duf_scan_direntry1( struct dirent *de, unsigned long long pathid, duf_depthinfo_t * pdi, duf_record_t * precord,
                    duf_scan_hook_dirent_reg_t scan_dirent_reg, duf_scan_hook_dirent_dir_t scan_dirent_dir )
{
  int r = 0;

  DEBUG_START(  );
  const duf_dirhandle_t *pdhi_parent = duf_levinfo_pdh( pdi );
  struct stat st;

  r = fstatat( pdhi_parent->dfd, de->d_name, &st, 0 );
  if ( r < 0 )
  {
    const char *real_path_parent = NULL;

    if ( !real_path_parent )
      real_path_parent = duf_levinfo_path( pdi );
    DUF_ERROR( "No such entry %s/%s", real_path_parent, de->d_name );
    r = DUF_ERROR_STAT;
  }

  if ( r >= 0 )
    r = duf_levinfo_down( pdi, 0, de->d_name, 0 /* ndirs */ , 0 /* nfiles */ , 0 /* is_leaf */  );
  if ( r >= 0 )
    switch ( de->d_type )
    {
    case DT_REG:
      r = ( *scan_dirent_reg ) ( de->d_name, &st, pathid, pdi, precord );
      DUF_TRACE( scan_de_reg, 0, "regfile='.../%s'", de->d_name );
      break;
    case DT_DIR:
      r = ( *scan_dirent_dir ) ( de->d_name, &st, pathid, pdi, precord );
      DUF_TRACE( scan_de_dir, 0, "dir='.../%s'", de->d_name );
      break;
    }
  duf_levinfo_up( pdi );
  DEBUG_ENDR( r );
  return r;
}


/* TODO scan for removed files; mark as absent or remove from db */
static int
duf_scan_dirents_by_pathid_and_dfname( unsigned long long pathid, duf_depthinfo_t * pdi, duf_record_t * precord, const char *dfname,
                                       duf_scan_hook_dirent_reg_t scan_dirent_reg, duf_scan_hook_dirent_dir_t scan_dirent_dir )
{
  int r = 0;
  const struct stat *pst_parent;

  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );
  const char *real_path_parent = NULL;

  assert( pdhi );
  pst_parent = &pdhi->st;

/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_parent || !( S_ISDIR( pst_parent->st_mode ) ) )
  {
    if ( !real_path_parent )
      real_path_parent = duf_levinfo_path( pdi );
/* no such entry */
    DUF_ERROR( "No such entry '%s'/'%s'", real_path_parent ? real_path_parent : "?", dfname );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( scan, 11, "No such entry %s", dfname );
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    DEBUG_START(  );
    DUF_TRACE( scan, 11, "pathid=%llu; scandir dfname:[%s]", pathid, dfname );
    nlist = scandirat( pdhi->dfd, ".", &list, duf_direntry_filter, alphasort );
    DUF_TRACE( scan, 11, "pathid=%llu; nlist=%d", pathid, nlist );
    if ( nlist < 0 )
    {
      int errorno = errno;

      if ( !real_path_parent )
        real_path_parent = duf_levinfo_path( pdi );

      if ( !real_path_parent )
        r = DUF_ERROR_PATH;
      else if ( errorno == EACCES )
        r = 0;
      else
      {
        DUF_ERRSYSE( errorno, "path '%s'/'%s'", real_path_parent ? real_path_parent : "?", dfname );
        r = DUF_ERROR_SCANDIR;
      }
      DUF_TEST_R( r );
    }
    else
    {
      for ( int il = 0; il < nlist; il++ )
      {
        DUF_TRACE( scan, 11, "pathid=%llu; entry='%s'", pathid, list[il]->d_name );

        r = duf_scan_direntry1( list[il], pathid, pdi, precord, scan_dirent_reg, scan_dirent_dir );
        /* if(r<0) break; */

        DUF_TEST_R( r );
        if ( list[il] )
          free( list[il] );
      }
      if ( list )
        free( list );
      DUF_TEST_R( r );
    }
  }

  DEBUG_ENDR( r );
  return r;
}

int
duf_scan_dirents1( duf_depthinfo_t * pdi, duf_record_t * precord,
                   duf_scan_hook_dirent_reg_t scan_dirent_reg, duf_scan_hook_dirent_dir_t scan_dirent_dir )
{
  int r = 0;
  unsigned long long dirid;

  DEBUG_START(  );

  dirid = duf_levinfo_dirid( pdi );
  /* dirid needless? */
  assert( dirid == duf_levinfo_dirid( pdi ) );
  {
    const char *real_path_parent = NULL;

    if ( DUF_IF_TRACE( scan ) )
    {
      if ( !real_path_parent )
        real_path_parent = duf_levinfo_path( pdi );
      DUF_TRACE( scan, 11, "real_path_parent=%s", real_path_parent );
    }
    {
      DUF_SFIELD( dfname );
      assert( dfname );
      DUF_TRACE( scan, 11, "dirid=%llu; scandir dfname:[%s]", dirid, dfname );
      r = duf_scan_dirents_by_pathid_and_dfname( dirid, pdi, precord, dfname, scan_dirent_reg, scan_dirent_dir );
    }

    DUF_TRACE( scan, 11, "real_path_parent=%s", real_path_parent );
  }
  DEBUG_ENDR( r );
  return r;
}
