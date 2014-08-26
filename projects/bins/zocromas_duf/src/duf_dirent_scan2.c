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

#include "duf_hook_types.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql_field.h"
/* #include "duf_sql_field1.h" */


#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_dirent_scan2.h"
/* ###################################################################### */


static int
duf_scan_direntry2( duf_sqlite_stmt_t * pstmt, struct dirent *de, unsigned long long pathid, duf_depthinfo_t * pdi,
                    duf_scan_hook2_dirent_reg_t scan_dirent_reg2, duf_scan_hook2_dirent_dir_t scan_dirent_dir2 )
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
      if ( scan_dirent_reg2 )
      {
        DUF_TRACE( scan_de_reg, 11, "pass   > regfile='.../%s'", de->d_name );
        r = ( *scan_dirent_reg2 ) ( pstmt, de->d_name, &st, pathid, pdi );
        DUF_TRACE( scan_de_reg, 11, "passed < regfile='.../%s'", de->d_name );
      }
      else
      {
        DUF_TRACE( scan_de_reg, 11, "missing scan_dirent_reg2; regfile='.../%s'", de->d_name );
      }
      break;
    case DT_DIR:
      if ( scan_dirent_dir2 )
      {
        DUF_TRACE( scan_de_dir, 10, "pass   > dir='.../%s'", de->d_name );
        r = ( *scan_dirent_dir2 ) ( pstmt, de->d_name, &st, pathid, pdi );
        DUF_TRACE( scan_de_dir, 10, "passed < dir='.../%s'", de->d_name );
      }
      else
      {
        DUF_TRACE( scan_de_dir, 10, "missing scan_dirent_dir2; dir='.../%s'", de->d_name );
      }
      break;
    }
  duf_levinfo_up( pdi );
  DEBUG_ENDR( r );
  return r;
}


static int
duf_scan_dirents_by_pdi_and_dfname2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, const char *dfname,
                                     duf_scan_hook2_dirent_reg_t scan_dirent_reg2, duf_scan_hook2_dirent_dir_t scan_dirent_dir2 )
{
  int r = 0;
  const struct stat *pst_parent;

  const char *real_path_parent = NULL;
  unsigned long long dirid;

  DEBUG_START(  );

  dirid = duf_levinfo_dirid( pdi );
  /* pst_parent = &pdhi->st; */
  pst_parent = duf_levinfo_stat( pdi );

/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_parent || !( S_ISDIR( pst_parent->st_mode ) ) )
  {
    if ( !real_path_parent )
      real_path_parent = duf_levinfo_path( pdi );
/* no such entry */
    DUF_ERROR( "No such entry '%s'/'%s'", real_path_parent ? real_path_parent : "?", dfname );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( scan, 0, "No such entry %s", dfname );
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    if ( !real_path_parent )
      real_path_parent = duf_levinfo_path( pdi );
    DUF_TRACE( scan, 0, "@@@@@@@@@@@@@@@@@@@@@@@ [%s :: %s] ", real_path_parent, dfname );

    DUF_TRACE( scan, 0, "dirid=%llu; scandir dfname:[%s]", dirid, dfname );
    {
#if 1
      nlist = scandirat( duf_levinfo_dfd( pdi ), ".", &list, duf_direntry_filter, alphasort );
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), dfname ? dfname : "nil", nlist,
                 duf_levinfo_dfd( pdi ) );
#else
      const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

      assert( pdhi );
      nlist = scandirat( pdhi->dfd, ".", &list, duf_direntry_filter, alphasort );
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), dfname ? dfname : "nil", nlist,
                 pdhi->dfd );
#endif
    }
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
        DUF_TRACE( scan, 11, "dirid=%llu; > entry='%s'", dirid, list[il]->d_name );

        r = duf_scan_direntry2( pstmt, list[il], dirid, pdi, scan_dirent_reg2, scan_dirent_dir2 );

        DUF_TRACE( scan, 11, "dirid=%llu; < entry='%s'", dirid, list[il]->d_name );

        DUF_TEST_R( r );
        if ( list[il] )
          free( list[il] );
      }
      DUF_TRACE( scan, 10, "passed scandirat='.../%s'", dfname );
      if ( list )
        free( list );
      DUF_TEST_R( r );
    }
  }

  DEBUG_ENDR( r );
  return r;
}

int
duf_scan_dirents2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi,
                   duf_scan_hook2_dirent_reg_t scan_dirent_reg2, duf_scan_hook2_dirent_dir_t scan_dirent_dir2 )
{
  int r = 0;

  DEBUG_START(  );

  {
    const char *real_path_parent = NULL;

    if ( DUF_IF_TRACE( scan ) )
    {
      if ( !real_path_parent )
        real_path_parent = duf_levinfo_path( pdi );
      DUF_TRACE( scan, 11, "real_path_parent=%s", real_path_parent );
    }
    DUF_SFIELD2( dfname );
    /* assert( dfname ); */
    {
      unsigned long long dirid;

      dirid = duf_levinfo_dirid( pdi );
      /* dirid needless? */
      assert( dirid == duf_levinfo_dirid( pdi ) );
      DUF_TRACE( scan, 0, "dirid=%llu; scandir dfname:[%s]", dirid, dfname );
    }
    r = duf_scan_dirents_by_pdi_and_dfname2( pstmt, pdi, dfname, scan_dirent_reg2, scan_dirent_dir2 );
  }
  DEBUG_ENDR( r );
  return r;
}
