#include "qstd_defs.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include <mastar/masxfs/masxfs_types.h>
#include <mastar/masxfs/masxfs_structs.h>

#include <mastar/masxfs/masxfs_levinfo_ref.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/mysqlpfs/mysqlpfs_query.h>

#include "qstd_structs.h"
#include "qstd_mstmt_base.h"
#include "qstd_mstmt_parents.h"
#include "qstd_mstmt_names.h"
#include "qstd_mstmt_sizes.h"
#include "qstd_mstmt_datas.h"

static int _uUu_
test7cb( masxfs_levinfo_t * li, unsigned long flags, void *qstdv )
{
  mas_qstd_t *qstd _uUu_ = ( mas_qstd_t * ) qstdv;

  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  {
    mysqlpfs_s_ulonglong_t theid = 0;
    mysqlpfs_s_ulonglong_t parent_id = masxfs_levinfo_parent_id( li );
    masxfs_entry_type_t detype = masxfs_levinfo_detype( li );
    mysqlpfs_s_ulonglong_t as_parent_id = 0;
      mysqlpfs_s_ulonglong_t dataid _uUu_ = 0;

    const char *sdetypes[] = {
      [MASXFS_ENTRY_BLK_NUM] = "BLK",
      [MASXFS_ENTRY_CHR_NUM] = "CHR",
      [MASXFS_ENTRY_DIR_NUM] = "DIR",
      [MASXFS_ENTRY_REG_NUM] = "REG",
      [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
      [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
      [MASXFS_ENTRY_LNK_NUM] = "LNK"
    };
    {
      const masxfs_stat_t *stat = masxfs_levinfo_stat_ref( li, flags );

      if ( stat )
        dataid = mas_qstd_mstmt_selinsget_datas_id( qstd, stat );
    }
    if ( masxfs_levinfo_depth_ref( li, flags ) != 0 )
    {
#if 0
      theid = mas_qstd_mstmt_insget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#endif
#if 1
      theid = mas_qstd_mstmt_selinsget_names_id( qstd, ename, parent_id, dataid, sdetypes[detype] );
#endif
#if 0
      theid = mas_qstd_mstmt_insselget_names_id( qstd, ename, parent_id, dataid, sdetypes[detype] );
#endif
    }
    if ( detype == MASXFS_ENTRY_DIR_NUM )
    {
      as_parent_id = mas_qstd_mstmt_selinsget_parents_id( qstd, theid );
      masxfs_levinfo_set_id( li, as_parent_id );
    }
    else if ( detype == MASXFS_ENTRY_REG_NUM )
    {
      size_t size = masxfs_levinfo_size_ref( li, flags );
      size_t thesize _uUu_ = mas_qstd_mstmt_insget_sizes_id( qstd, size );

    /* WARN( "SIZE: %ld / %ld", size, thesize ); */
    }
    if ( !theid || 0 == strcmp( ename, "home" ) || as_parent_id == 66 || as_parent_id == 1 )
      MARK( "(T6)", " %ld. '%s' ID: %llu => %llu; as_parent_id:%llu", depth, ename, ( unsigned long long ) theid, ( unsigned long long ) parent_id,
            ( unsigned long long ) as_parent_id );
  }
  return 0;
}

int
test7( void )
{
  rDECL( 0 );
/* int r = 0; */

  masxfs_entry_callback_t callbacks[] = {
    {MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, test7cb,
     .flags = 0 | MASXFS_CB_NAME /* | MASXFS_CB_PATH */  | MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ }
    , {0, NULL}
  };

/* mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 ); */
  mas_qstd_t *qstd = mas_qstd_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( qstd->pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 /* depth limit */  );

    rC( mas_mysqlpfs_query( qstd->pfs, "START TRANSACTION" ) );

    rC( masxfs_pathinfo_scan_depth( pi, test7cb, qstd, MASXFS_CB_NAME | MASXFS_CB_STAT /* flags */  ) );
#if 1
    rC( masxfs_pathinfo_scan( pi, callbacks, qstd, MASXFS_CB_RECURSIVE /* | MASXFS_CB_MULTIPLE_CBS */ , 1000 /* maxdepth */  ) );
#else
    masxfs_scanpath_real( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest", callbacks,
                          MASXFS_CB_RECURSIVE /* | MASXFS_CB_MULTIPLE_CBS */ , 255 );
#endif
  /* rC( mas_mysqlpfs_query( qstd->pfs, "UPDATE parents AS upar SET nchilds=(SELECT COUNT(*) FROM filenames AS fn LEFT JOIN parents AS p ON (fn.parent_id=p.id) WHERE upar.id==fn.parent_id)" ) ); */
    {
      const char *usql[] = {
        "UPDATE parents AS p "
                " LEFT JOIN (SELECT filenames.parent_id, COUNT(*) AS nchilds "
                " FROM filenames "
                " LEFT JOIN parents ON (filenames.parent_id=parents.id) "
                " GROUP BY filenames.parent_id) AS fnx ON (p.id=fnx.parent_id) SET p.nchilds=fnx.nchilds",
/* "UPDATE filesizes" */
      };
      for ( size_t i = 0; i < sizeof( usql ) / sizeof( usql[0] ); i++ )
        rC( mas_mysqlpfs_query( qstd->pfs, usql[i] ) );
    }
    rC( mas_mysqlpfs_query( qstd->pfs, "COMMIT" ) );

    masxfs_pathinfo_delete( pi );
  }
/* mysqlpfs_delete( pfs ); */
  mas_qstd_delete( qstd );
  rRET;
/* return r; */
}
