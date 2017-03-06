#define RGEMSG mysql_error(mas_qstd_mysql(qstd))
#include "qstd_defs.h"
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_base.h"

void
mas_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port )
{
  if ( qstd )
  {
    qstd->pfs = mysqlpfs_create_setup( host, user, passwd, db, port );
    qstd->std_mstmts = mas_qstd_mstmt_create_array(  );
  }
}

mas_qstd_t *
mas_qstd_create( void )
{
  return mas_calloc( 1, sizeof( mas_qstd_t ) );
}

mas_qstd_t *
mas_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port )
{
  mas_qstd_t *qstd = mas_qstd_create(  );

  mas_qstd_init( qstd, host, user, passwd, db, port );
  return qstd;
}

void
mas_qstd_reset( mas_qstd_t * qstd )
{
  if ( qstd )
  {
    mas_qstd_mstmt_delete_array( qstd->std_mstmts );
    mysqlpfs_delete( qstd->pfs );
  }
}

void
mas_qstd_delete( mas_qstd_t * qstd )
{
  mas_qstd_reset( qstd );
  if ( qstd )
    mas_free( qstd );
}

/**********************************************************************************/

mysqlpfs_mstmt_t **
mas_qstd_mstmt_create_array( void )
{
  return mas_calloc( STD_MSTMT_MAX, sizeof( mysqlpfs_mstmt_t ** ) );
}

void
mas_qstd_mstmt_reset_array( mysqlpfs_mstmt_t ** mstmts )
{
  if ( mstmts )
    for ( int nst = 0; nst < STD_MSTMT_MAX; nst++ )
    {
      if ( mstmts[nst] )
      {
        mas_mysqlpfs_mstmt_delete( mstmts[nst] );
      }
      mstmts[nst] = NULL;
    }
}

void
mas_qstd_mstmt_delete_array( mysqlpfs_mstmt_t ** mstmts )
{
  mas_qstd_mstmt_reset_array( mstmts );
  mas_free( mstmts );
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_init( mas_qstd_t * qstd, mas_qstd_id_t stdid )
{
  rDECL( 0 );
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( qstd && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
    mysqlpfs_mstmt_t **mstmts = qstd->std_mstmts;

    mstmt = mstmts[stdid];
    if ( mstmt )
    {
      mas_mysqlpfs_mstmt_delete( mstmt );
      mstmt = mstmts[stdid] = NULL;
    }
    QRGP( qstd->pfs );
    if ( qstd->pfs )
    {
      mysqlpfs_t *pfs = qstd->pfs;

      switch ( stdid )
      {
      case STD_MSTMT_INSERT_NAMES:
        {
          char *insop = "INSERT INTO filenames(name,parent_id,data_id,detype) VALUES (?,?,?,?)";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 4, 0, insop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 2 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 3, 255 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_SELECT_NAMES_ID:
        {
          char *selop = "SELECT id FROM filenames AS fn " /* "LEFT JOIN parents as p ON (fn.parent_id=p.id)" */ " WHERE name=? AND fn.parent_id<=>?";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_INSERT_PARENTS:
        {
          char *insop = "INSERT INTO parents(dir_id) VALUES (?)";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_SELECT_PARENTS_ID:
        {
          char *selop = "SELECT id FROM parents WHERE dir_id<=>?";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_INSERT_SIZES:
        {
          char *insop = "INSERT IGNORE INTO filesizes(size) VALUES (?)";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_SELECT_SIZES_ID:
        {
          char *selop = "SELECT size FROM filesizes WHERE size=?";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_INSERT_DATAS:
        {
          char *insop = "INSERT IGNORE INTO filedatas(dev,inode) VALUES (?,?)";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 0, insop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_SELECT_DATAS_ID:
        {
          char *selop = "SELECT id FROM filedatas WHERE dev=? AND inode=?";

          mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );
          QRGP( mstmt );

          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
          QRGS( rCODE );
          rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
          QRGS( rCODE );
        }
        break;
      case STD_MSTMT_MAX:
        break;
      }
    }
    mstmts[stdid] = mstmt;
  }
  return mstmt;
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_get( mas_qstd_t * qstd, mas_qstd_id_t stdid )
{
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( qstd && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
  /* mysqlpfs_mstmt_t **mstmts = pfs->std_mstmts; */
    mysqlpfs_mstmt_t **mstmts = qstd->std_mstmts;

    mstmt = mstmts[stdid];
    if ( !mstmt )
    {
      mstmt = mas_qstd_mstmt_init( qstd, stdid );
      QRGP( mstmt );
    }
    QRGP( mstmt );
  }
  else
  {
    WARN( "qstd:%p; stdid:%d; STD_MSTMT_MAX:%d;", qstd, stdid, STD_MSTMT_MAX );
    QRG( -1 );
  }

  return mstmt;
}
