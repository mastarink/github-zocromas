#define R_GOOD(_r) (_r>=0)
#include <mastar/qstd/qstd_defs.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include <mastar/masxfs/masxfs_types.h>
#include <mastar/masxfs/masxfs_structs.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/qstd/qstd_structs.h>
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_parents.h>
#include <mastar/qstd/qstd_mstmt_names.h>
#include <mastar/qstd/qstd_mstmt_sizes.h>
#include <mastar/qstd/qstd_mstmt_datas.h>
#include <mastar/qstd/qstd_mstmt_props.h>

int
test0( void )
{
  rDECL( 0 );

  {
    mas_qstd_t *qstd = mas_qstd_instance_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

    mas_qstd_drop_tables( qstd );
    mas_qstd_create_tables( qstd );

    if ( qstd->pfs )
    {
      const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 /* depth limit */  );

      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    }
    mas_qstd_instance_delete(  );
  }
  rRET;
}
