#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
/* #include <unistd.h> */

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

/* #include <mastar/levinfo/masxfs_levinfo_types.h> */
/* #include <mastar/masxfs/masxfs_types.h> */

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include "qstd_structs.h"
#include "qstd_enums.h"
/* #include "qstd_mstmt.h" */
#include "qstd_mstmt_base.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_props.h"

unsigned long long
mas_qstd_mstmt_selget_props_id( mas_qstd_t * qstd, unsigned long long data_id )
{
  rDECLBAD;
  unsigned long long theid = 0;

  {
    int np = 0;
    int nr = 0;
    int has_data = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_PROPS_ID );

    QRGP( mstmt_s );
    rC( mas_qstd_ret_code( qstd ) );
  /* rC( mas_qstd_mstmt_ret_code( mstmt_s ) ); */

    rC( mas_qstd_mstmt_set_param_longlong( mstmt_s, np++, data_id, FALSE ) );
    rC( mas_qstd_mstmt_execute_store( mstmt_s ) );

    rC( mas_qstd_mstmt_fetch( mstmt_s, &has_data ) );
    if ( has_data )
    {
      unsigned is_null = 0;

      rC( mas_qstd_mstmt_get_result_longlong( mstmt_s, nr++, &theid, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_PROPS_NRESULTS );
    }

    mas_qstd_mstmt_free_result( mstmt_s );
  }
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_insget_props_id( mas_qstd_t * qstd, unsigned long long data_id, const char *sdetype, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );
  unsigned long long theid = 0;

  {
    int np = 0;
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_PROPS );

    QRGP( mstmt );
    rC( mas_qstd_ret_code( qstd ) );
  /* rC( mas_qstd_mstmt_ret_code( mstmt ) ); */
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, data_id, FALSE ) );

    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_mode, stat->st_mode ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_uid, stat->st_uid ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_gid, stat->st_gid ? FALSE : TRUE ) );
  /*
     struct timespec {
     time_t tv_sec;
     long tv_nsec;
     };
   */
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_atim.tv_sec, stat->st_atim.tv_sec ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_mtim.tv_sec, stat->st_mtim.tv_sec ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_ctim.tv_sec, stat->st_ctim.tv_sec ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_size, FALSE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_rdev, stat->st_rdev ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_blksize, stat->st_blksize ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, ( unsigned long long ) stat->st_blocks, stat->st_blocks ? FALSE : TRUE ) );
    rC( mas_qstd_mstmt_set_param_string( mstmt, np++, sdetype ) );
    assert( np == STD_MSTMT_INSERT_PROPS_NFIELDS );
    rC( mas_qstd_mstmt_execute( mstmt ) );

    if ( rGOOD && mas_qstd_mstmt_affected_rows( mstmt ) == 1 )
    {
      theid = mas_qstd_mstmt_insert_id( mstmt );
      rC( mas_qstd_mstmt_ret_code( mstmt ) );
    }
  }
/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_selinsget_props_id( mas_qstd_t * qstd, unsigned long long data_id, const char *sdetype, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );

  unsigned long long theid = mas_qstd_mstmt_selget_props_id( qstd, data_id );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !theid )
  {
    theid = mas_qstd_mstmt_insget_props_id( qstd, data_id, sdetype, stat );
    rC( mas_qstd_ret_code( qstd ) );
  }

/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}

unsigned long long
mas_qstd_mstmt_insselget_props_id( mas_qstd_t * qstd, unsigned long long data_id, const char *sdetype, const mas_qstd_stat_t * stat )
{
  rDECLBAD;
  QRGP( qstd );
  unsigned long long theid = mas_qstd_mstmt_insget_props_id( qstd, data_id, sdetype, stat );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !theid )
  {
    theid = mas_qstd_mstmt_selget_props_id( qstd, data_id );
    rC( mas_qstd_ret_code( qstd ) );
  }
/* QRG( theid >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return theid;
}
