#define R_GOOD(_r) (_r>=0)
#include "qstd_defs.h"
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

#include <mastar/mysqlpfs/mysqlpfs_defs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>

#include "qstd_structs.h"
#include "qstd_mstmt_base.h"
#include "qstd_mstmt_parents.h"
#include "qstd_mstmt_names.h"
#include "qstd_mstmt_sizes.h"
#include "qstd_mstmt_datas.h"
#include "qstd_mstmt_props.h"

static int
test7cb( masxfs_levinfo_t * li, unsigned long flags, void *qstdv, masxfs_depth_t reldepth _uUu_ )
{
  mas_qstd_t *qstd = ( mas_qstd_t * ) qstdv;

  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );

  {
    unsigned long long theid = 0;
    unsigned long long parent_id = masxfs_levinfo_parent_id( li );
    masxfs_entry_type_t detype = masxfs_levinfo_detype( li, ( masxfs_levinfo_flags_t ) MASXFS_CB_MODE_FS );
    unsigned long long as_parent_id = 0;
    unsigned long long dataid = 0;

  /* unsigned long long propid _uUu_ = 0; */

    const char *sdetypes[] = {
      [MASXFS_ENTRY_BLK_NUM] = "BLK",
      [MASXFS_ENTRY_CHR_NUM] = "CHR",
      [MASXFS_ENTRY_DIR_NUM] = "DIR",
      [MASXFS_ENTRY_REG_NUM] = "REG",
      [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
      [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
      [MASXFS_ENTRY_LNK_NUM] = "LNK"
    };
  /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
    {
      size_t size = masxfs_levinfo_size_ref( li, flags );
      size_t thesize _uUu_ = mas_qstd_mstmt_insget_sizes_id( qstd, size );

    /* WARN( "SIZE: %ld / %ld", size, thesize ); */
    }
    {
      const masxfs_stat_t *stat = masxfs_levinfo_stat_ref( li, flags );

      if ( stat )
      {
        dataid = mas_qstd_mstmt_selinsget_datas_id( qstd, stat );
      /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
        mas_qstd_mstmt_selinsget_props_id( qstd, dataid, sdetypes[detype], stat );
      }
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
      masxfs_levinfo_set_node_id( li, as_parent_id );
    }
    if ( !theid || 0 == strcmp( ename, "home" ) || as_parent_id == 66 || as_parent_id == 1 )
      MARK( "(T6)", " %ld. '%s' ID: %llu => %llu; as_parent_id:%llu", ( long ) depth, ename, ( unsigned long long ) theid,
            ( unsigned long long ) parent_id, ( unsigned long long ) as_parent_id );
  }
  return 0;
}

int
test7( void )
{
  rDECL( 0 );

  masxfs_entry_callback_t callbacks[] = {
    { /* MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, */ test7cb,
     .flags = 0 | MASXFS_CB_NAME /* | MASXFS_CB_PATH */  | MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ }
    , {NULL}
  };

  mas_qstd_t *qstd = mas_qstd_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  mas_qstd_drop_tables( qstd );
  mas_qstd_create_tables( qstd );
  if ( qstd->pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 /* depth limit */ , 0);
    masxfs_levinfo_flags_t flagsfs = MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_SINGLE_CB;
    masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;

    rC( mas_mysqlpfs_query( qstd->pfs, "START TRANSACTION" ) );
  /*20170319.113014 rC( masxfs_pathinfo_scan_depth_cbf( pi, test7cb, qstd, MASXFS_CB_NAME | MASXFS_CB_STAT | MASXFS_CB_MODE_FS (* flags *)  ) ); */
    rC( masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, qstd, flagsfs, 1000 /* maxdepth */  ) );
    rC( mas_mysqlpfs_query( qstd->pfs, "COMMIT" ) );
    {
      const char *updop[] = {
        "START TRANSACTION",
        "UPDATE " QSTD_TABLE_PARENTS " AS p "                        /* */
                " LEFT JOIN "                                        /* */
                "   (SELECT " QSTD_TABLE_NAMES ".parent_id, COUNT(*) AS nchilds " /* */
                "     FROM " QSTD_TABLE_NAMES " "                    /* */
                "     LEFT JOIN " QSTD_TABLE_PARENTS " ON (" QSTD_TABLE_NAMES ".parent_id=" QSTD_TABLE_PARENTS ".id) " /* */
                "     GROUP BY " QSTD_TABLE_NAMES ".parent_id) "     /* */
                "    AS fx ON (p.id=fx.parent_id) SET p.nchilds=fx.nchilds",
        "UPDATE " QSTD_TABLE_DATAS " AS fd "                         /* */
                " LEFT JOIN "                                        /* */
                "   (SELECT ifd.id AS data_id, COUNT(*) AS nlinkdb " /* */
                "     FROM " QSTD_TABLE_DATAS " AS ifd "             /* */
                "     LEFT JOIN " QSTD_TABLE_NAMES " AS ifn ON (ifn.data_id=ifd.id) " /* */
                "     GROUP BY ifn.data_id) "                        /* */
                "    AS fx ON (fd.id=fx.data_id) SET fd.nlinkdb=fx.nlinkdb",
        "UPDATE " QSTD_TABLE_SIZES " AS fs "                         /* */
                " LEFT JOIN "                                        /* */
                "   (SELECT ifs.size AS size, COUNT(*) AS nsame "    /* */
                "     FROM " QSTD_TABLE_SIZES " AS ifs "             /* */
                "     LEFT JOIN " QSTD_TABLE_PROPS " AS ifp ON (ifs.size=ifp.size) " /* */
                "     GROUP BY ifp.size) "                           /* */
                "    AS fx ON (fs.size=fx.size) SET fs.nsame=fx.nsame",
        "COMMIT"
      };
      for ( size_t i = 0; i < sizeof( updop ) / sizeof( updop[0] ); i++ )
        rC( mas_mysqlpfs_query( qstd->pfs, updop[i] ) );
    }

    masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
  }
  mas_qstd_delete( qstd );
  rRET;
}
