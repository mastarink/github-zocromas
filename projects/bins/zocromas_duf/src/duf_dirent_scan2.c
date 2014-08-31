#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>



/* #include "duf_types.h" */
#include "duf_maintenance.h"


#include "duf_utils.h"
#include "duf_config_ref.h"

#include "duf_levinfo.h"
#include "duf_dh.h"
#include "duf_dirent.h"

#include "duf_hook_types.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql_field.h"
/* #include "duf_sql_field1.h" */



/* ###################################################################### */
#include "duf_dirent_scan2.h"
/* ###################################################################### */


static int
duf_scan_direntry2(  /* duf_sqlite_stmt_t * pstmt, */ struct dirent *de, duf_depthinfo_t * pdi,
                    duf_scan_hook2_dirent_reg_t scan_dirent_reg2, duf_scan_hook2_dirent_dir_t scan_dirent_dir2 )
{
  int r = 0;

  DEBUG_START(  );
  duf_dirhandle_t *pdhi;
  const duf_dirhandle_t *pdhi_parent;

  /* struct stat st; */

  if ( r >= 0 )
    r = duf_levinfo_godown( pdi, 0, de->d_name, 0 /* ndirs */ , 0 /* nfiles */ , de->d_type != DT_DIR /* is_leaf */  );

  if ( r >= 0 )
  {
    pdhi = duf_levinfo_pdh( pdi );
    pdhi_parent = duf_levinfo_pdh_up( pdi );
  }
  /* r = fstatat( pdhi_parent->dfd, de->d_name, &st, 0 ); */


  r = duf_statat_dh( pdhi, pdhi_parent, duf_levinfo_itemname( pdi ) );

  if ( r < 0 )
  {
    if ( errno == ENOENT )
      DUF_ERROR( "No such entry %s/%s", duf_levinfo_path( pdi ), duf_levinfo_itemname( pdi ) );
    r = DUF_ERROR_STAT;
  }
  if ( r >= 0 )
  {
#if 1
    if ( duf_levinfo_is_leaf( pdi ) )
    {
      /* sccb->dirent_file_scan_before2 -- duf_scan_hook2_dirent_reg_t */
      if ( scan_dirent_reg2 )
      {
        r = scan_dirent_reg2(  /* pstmt, */ duf_levinfo_itemname( pdi ), duf_levinfo_stat( pdi ), duf_levinfo_dirid_up( pdi ), pdi );
      }
      else
      {
        DUF_TRACE( scan_de_reg, 11, "missing scan_dirent_reg2; regfile='.../%s'", duf_levinfo_itemname( pdi ) );
      }
    }
    else
    {
      /* sccb->dirent_dir_scan_before2 -- duf_scan_hook2_dirent_dir_t */
      if ( scan_dirent_dir2 )
      {
        r = scan_dirent_dir2(  /* pstmt, */ duf_levinfo_itemname( pdi ), duf_levinfo_stat( pdi ), duf_levinfo_dirid_up( pdi ), pdi );
      }
      else
      {
        DUF_TRACE( scan_de_dir, 10, "missing scan_dirent_dir2; dir='.../%s'", duf_levinfo_itemname( pdi ) );
      }
    }
    DUF_TRACE( scan_de_reg, 11, "passed < regfile='.../%s'", duf_levinfo_itemname( pdi ) );
#else
    switch ( de->d_type )
    {
    case DT_REG:
      if ( scan_dirent_reg2 )
      {
        DUF_TRACE( scan_de_reg, 11, "pass   > regfile='.../%s'", duf_levinfo_itemname( pdi ) );
        r = scan_dirent_reg2( pstmt, duf_levinfo_itemname( pdi ), duf_levinfo_stat( pdi ), duf_levinfo_dirid_up( pdi ), pdi );
        DUF_TRACE( scan_de_reg, 11, "passed < regfile='.../%s'", duf_levinfo_itemname( pdi ) );
      }
      else
      {
        DUF_TRACE( scan_de_reg, 11, "missing scan_dirent_reg2; regfile='.../%s'", duf_levinfo_itemname( pdi ) );
      }
      break;
    case DT_DIR:
      if ( scan_dirent_dir2 )
      {
        DUF_TRACE( scan_de_dir, 10, "pass   > dir='.../%s'", duf_levinfo_itemname( pdi ) );
        r = scan_dirent_dir2( pstmt, duf_levinfo_itemname( pdi ), duf_levinfo_stat( pdi ), duf_levinfo_dirid_up( pdi ), pdi );
        DUF_TRACE( scan_de_dir, 10, "passed < dir='.../%s'", duf_levinfo_itemname( pdi ) );
      }
      else
      {
        DUF_TRACE( scan_de_dir, 10, "missing scan_dirent_dir2; dir='.../%s'", duf_levinfo_itemname( pdi ) );
      }
      break;
    }
#endif
  }
  duf_levinfo_goup( pdi );
  DEBUG_ENDR( r );
  return r;
}

/*
 * pdi
 * scan_dirent_reg2 - dir entry scanner function
 * scan_dirent_dir2 - reg (file) entry scanner function
 * */
int
duf_scan_dirents2(  /* duf_sqlite_stmt_t * pstmt, */ duf_depthinfo_t * pdi,
                   duf_scan_hook2_dirent_reg_t scan_dirent_reg2, duf_scan_hook2_dirent_dir_t scan_dirent_dir2 )
{
  int r = 0;
  const struct stat *pst_parent;

  DEBUG_START(  );

  pst_parent = duf_levinfo_stat( pdi );

/* check if parent really existing directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_parent || !( S_ISDIR( pst_parent->st_mode ) ) )
  {
/* no such entry */
    DUF_ERROR( "No such entry '%s'/'%s'", duf_levinfo_path_q( pdi, "?" ), duf_levinfo_itemname( pdi ) );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( scan, 0, "No such entry %s", duf_levinfo_itemname( pdi ) );
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    DUF_TRACE( scan, 0, "dirID=%llu; scandir dfname:[%s :: %s]", duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemname( pdi ) );
    {
#if 1
      nlist = scandirat( duf_levinfo_dfd( pdi ), ".", &list, duf_direntry_filter, alphasort );
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), duf_levinfo_itemname_q( pdi, "nil" ), nlist,
                 duf_levinfo_dfd( pdi ) );
#else
      const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

      assert( pdhi );
      nlist = scandirat( pdhi->dfd, ".", &list, duf_direntry_filter, alphasort );
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu - %s; nlist=%d; (dfd:%d)", duf_levinfo_dirid( pdi ), duf_levinfo_itemname_q( pdi, "nil" ), nlist,
                 pdhi->dfd );
#endif
    }
    if ( nlist < 0 )
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
    else
    {
      for ( int il = 0; il < nlist; il++ )
      {
        r = duf_scan_direntry2(  /* pstmt, */ list[il], pdi, scan_dirent_reg2, scan_dirent_dir2 );

        DUF_TEST_R( r );
        if ( list[il] )
          free( list[il] );
      }
      DUF_TRACE( scan, 10, "passed scandirat='.../%s'", duf_levinfo_itemname( pdi ) );
      if ( list )
        free( list );
      DUF_TEST_R( r );
    }
  }

  DEBUG_ENDR( r );
  return r;
}
