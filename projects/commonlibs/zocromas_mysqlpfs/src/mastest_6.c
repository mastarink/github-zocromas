#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>

#include <mastar/masxfs/masxfs_structs.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>
#include <mastar/masxfs/masxfs_levinfo_ref.h>
#include <mastar/masxfs/masxfs_enums.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt_std.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

static int _uUu_
test6icb( masxfs_levinfo_t * li, unsigned long flags _uUu_, void *qstdv )
{
  mas_qstd_t *qstd = ( mas_qstd_t * ) qstdv;

  mysqlpfs_s_ulonglong_t theid = 0;
  mysqlpfs_s_ulonglong_t parent_id = masxfs_levinfo_parent_id( li );
  masxfs_entry_type_t detype = masxfs_levinfo_detype( li );

  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  const char *sdetypes[] = {
    [MASXFS_ENTRY_BLK_NUM] = "BLK",
    [MASXFS_ENTRY_CHR_NUM] = "CHR",
    [MASXFS_ENTRY_DIR_NUM] = "DIR",
    [MASXFS_ENTRY_REG_NUM] = "REG",
    [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
    [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
    [MASXFS_ENTRY_LNK_NUM] = "LNK"
  };
  if ( !ename )
    DIE( "NAME is NULL; %s - %ld", li->name, li->lidepth );
#if 0
  theid = mas_qstd_mstmt_selinsget_names_id( pfs, ename, parent_id, sdetypes[detype] );
#else
  theid = mas_qstd_mstmt_insselget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#endif
  masxfs_levinfo_set_id( li, theid );
  MARK( "(T6)", " %ld. %s ID: %llu", depth, ename, ( unsigned long long ) theid );

  return 0;
}

int
test6( void )
{
/* mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 ); */
  mas_qstd_t *qstd = mas_qstd_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( qstd->pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      rSET( 0 );
      WARN( "%p %ld", pi, pi->pidepth );
      for ( size_t i = 0; i < pi->pidepth; i++ )
      {
        WARN( "%ld. %s", i, pi->levinfo[i].name );
      }
      rC( masxfs_pathinfo_scan_depth( pi, test6icb, qstd, MASXFS_CB_NAME /* flags */  ) );

      {
        mysqlpfs_s_ulonglong_t theid = 0;

        theid = mas_qstd_mstmt_selget_names_id( qstd, "zocromas_xfs", 12 );
        MARK( "RESULT", ": theid:%lld", theid );
      }

      {
        char *path = masxfs_pathinfo_pi2path( pi );

        EXAMS( path, path0, "%s : %s" );
        WARN( "restored path:%s", path );
        mas_free( path );
      }
      masxfs_pathinfo_delete( pi );
    }
  /* mysqlpfs_delete( pfs ); */
    mas_qstd_delete( qstd );
  }

  return 0;
}
