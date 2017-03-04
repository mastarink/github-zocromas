#include "mysqlpfs_defs.h"
/* #include <mastar/masxfs/masxfs_defs.h> */
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include <mastar/masxfs/masxfs_structs.h>

/* #include <mastar/masxfs/masxfs_levinfo_base.h> */
#include <mastar/masxfs/masxfs_levinfo_io.h>
#include <mastar/masxfs/masxfs_levinfo_path.h>
#include <mastar/masxfs/masxfs_levinfo_ref.h>
#include <mastar/masxfs/masxfs_levinfo.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/masxfs/masxfs_scan.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt_std.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

static int _uUu_
test7cb( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *qstdv )
{
  mas_qstd_t *qstd _uUu_ = ( mas_qstd_t * ) qstdv;

  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  {
    mysqlpfs_s_ulonglong_t theid = 0;
    mysqlpfs_s_ulonglong_t parent_id = masxfs_levinfo_parent_id( li );
    masxfs_entry_type_t detype = masxfs_levinfo_detype( li );

    const char *sdetypes[] = {
      [MASXFS_ENTRY_BLK_NUM] = "BLK",
      [MASXFS_ENTRY_CHR_NUM] = "CHR",
      [MASXFS_ENTRY_DIR_NUM] = "DIR",
      [MASXFS_ENTRY_REG_NUM] = "REG",
      [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
      [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
      [MASXFS_ENTRY_LNK_NUM] = "LNK"
    };
#if 0
    theid = mysqlpfs_mstmt_std_insget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#elif 1
    theid = mysqlpfs_mstmt_std_selinsget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#elif 1
    theid = mysqlpfs_mstmt_std_insselget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#endif
    masxfs_levinfo_set_id( li, theid );
    MARK( "(T6)", " %ld. %s ID: %llu", depth, ename, ( unsigned long long ) theid );
  }
  return 0;
}

int
test7( void )
{
  int r = 0;

  masxfs_entry_callback_t callbacks[] = {
    {MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, test7cb,
     .flags = 0 | MASXFS_CB_NAME /* | MASXFS_CB_PATH */  | MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ }
    , {0, NULL}
  };

/* mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 ); */
  mas_qstd_t *qstd = mysqlpfs_qstd_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( qstd->pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    r = mas_mysqlpfs_query( qstd->pfs, "START TRANSACTION" );

    if ( !r )
      r = masxfs_pathinfo_scan_depth( pi, test7cb, qstd, MASXFS_CB_NAME /* flags */  );
#if 1
    r = masxfs_pathinfo_scan( pi, callbacks, qstd, MASXFS_CB_RECURSIVE /* | MASXFS_CB_MULTIPLE_CBS */ , 1000 /* maxdepth */  );
#else
    masxfs_scanpath_real( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest", callbacks,
                          MASXFS_CB_RECURSIVE /* | MASXFS_CB_MULTIPLE_CBS */ , 255 );
#endif
    r = mas_mysqlpfs_query( qstd->pfs, "COMMIT" );

    masxfs_pathinfo_delete( pi );
  }
/* mysqlpfs_delete( pfs ); */
  mysqlpfs_qstd_delete( qstd );
  return r;
}
