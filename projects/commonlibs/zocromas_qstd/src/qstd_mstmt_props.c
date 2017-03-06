#define RGEMSG mysql_error(mas_qstd_mysql(qstd))
#include "qstd_defs.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include <mastar/masxfs/masxfs_types.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"
#include "qstd_mstmt.h"
#include "qstd_mstmt_base.h"

#include "qstd_mstmt_props.h"

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id )
{
  rDECL( 0 );
  mysqlpfs_s_ulonglong_t theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_PROPS_ID );

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 0, data_id, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );
  /* QRGS( rCODE ); */

    if ( rCODE != MYSQL_NO_DATA )
    {
      mysqlpfs_s_bool_t is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &theid, &is_null ) );
      QRGS( rCODE );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype, const masxfs_stat_t * stat _uUu_ )
{
  rDECL( 0 );
  QRGP( qstd );
  mysqlpfs_s_ulonglong_t theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_PROPS );
    /* const schar *s = "INSERT  INTO fileprops(data_id,detype,mode,uid,gid,atim,mtim,ctim,size,rdev,blksize,blocks) " */
    /*         "VALUES (?,?,?,?,?,FROM_UNIXTIME(?),FROM_UNIXTIME(?),FROM_UNIXTIME(?),?,?,?,?)";                        */

/* data_id,detype,mode,uid,gid,atim,mtim,ctim,size,rdev,blksize,blocks */
    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 0, data_id, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, 1, sdetype ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 2, stat->st_mode, stat->st_mode ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 3, stat->st_uid, stat->st_uid ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 4, stat->st_gid, stat->st_gid ? FALSE : TRUE ) );
    QRGS( rCODE );
  /*
     struct timespec {
     time_t tv_sec;
     long tv_nsec;
     };
   */
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 5, stat->st_atim.tv_sec, stat->st_atim.tv_sec ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 6, stat->st_mtim.tv_sec, stat->st_mtim.tv_sec ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 7, stat->st_ctim.tv_sec, stat->st_ctim.tv_sec ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 8, stat->st_size, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 9, stat->st_rdev, stat->st_rdev ? FALSE : TRUE ) );
    QRGS( rCODE );
    if ( rCODE )
      WARN( "ERR: %s", mas_mysqlpfs_mstmt_error( mstmt ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 10, stat->st_blksize, stat->st_blksize ? FALSE : TRUE ) );
    QRGS( rCODE );
    if ( rCODE )
      WARN( "ERR: %s", mas_mysqlpfs_mstmt_error( mstmt ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 11, stat->st_blocks, stat->st_blocks ? FALSE : TRUE ) );
    if ( rCODE )
      WARN( "ERR: %s", mas_mysqlpfs_mstmt_error( mstmt ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    QRGS( rCODE );
    if ( rCODE )
      WARN( "ERR: %s", mas_mysqlpfs_mstmt_error( mstmt ) );

    if ( !rCODE && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      theid = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
/* QRG( theid >= 0 ? 0 : -1 ); */
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selinsget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype, const masxfs_stat_t * stat )
{
/* rDECL( 0 ); */
  QRGP( qstd );

  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_selget_props_id( qstd, data_id );

  if ( !theid )
    theid = mas_qstd_mstmt_insget_props_id( qstd, data_id, sdetype, stat );

/* QRG( theid >= 0 ? 0 : -1 ); */
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insselget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype, const masxfs_stat_t * stat )
{
/* rDECL( 0 ); */
  QRGP( qstd );
  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_insget_props_id( qstd, data_id, sdetype, stat );

  if ( !theid )
    theid = mas_qstd_mstmt_selget_props_id( qstd, data_id );

/* QRG( theid >= 0 ? 0 : -1 ); */
  return theid;
}
