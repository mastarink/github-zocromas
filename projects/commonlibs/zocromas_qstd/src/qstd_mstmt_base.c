#define R_GOOD(_r) (_r>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>

#include "qstd_structs.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_base.h"

static mas_qstd_t *instance = NULL;

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

mas_qstd_t *
mas_qstd_instance_setup( const char *host, const char *user, const char *passwd, const char *db, int port )
{
  if ( !instance )
  {
    instance = mas_qstd_create_setup( host, user, passwd, db, port );
  }
  return instance;
}

mas_qstd_t *
mas_qstd_instance( void )
{
  return instance;
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

void
mas_qstd_instance_delete( void )
{
  mas_qstd_delete( instance );
  instance = NULL;
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_create_setup( mas_qstd_t * qstd, int nparams, int nresults, const char *sqlop )
{
  mysqlpfs_mstmt_t *mstmt = NULL;

  mstmt = mas_mysqlpfs_mstmt_create_setup( qstd->pfs, nparams, nresults, sqlop );
  return mstmt;
}

mysqlpfs_mstmt_t *
mas_qstd_instance_mstmt_create_setup( int nparams, int nresults, const char *sqlop )
{
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( instance )
    mstmt = mas_qstd_mstmt_create_setup( instance, nparams, nresults, sqlop );
  return mstmt;
}

void
mas_qstd_mstmt_delete( mysqlpfs_mstmt_t * mstmt )
{
  mas_mysqlpfs_mstmt_delete( mstmt );
}

/**********************************************************************************/
int
mas_qstd_create_tables( mas_qstd_t * qstd )
{
  rDECL( 0 );
  char *creops[] _uUu_ = {
    "START TRANSACTION",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_SIZES " ("              /* */
            "size BIGINT NOT NULL PRIMARY KEY"                       /* */
            ", nsame INTEGER NOT NULL, INDEX nsame (nsame)"          /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_DATAS " ("              /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", dev INTEGER NOT NULL"                                 /* */
            ", inode BIGINT NOT NULL"                                /* */
            ", nlink INTEGER, INDEX nlink (nlink)"                   /* */
            ", nlinkdb INTEGER, INDEX nlinkdb (nlinkdb)"             /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", UNIQUE INDEX dev_inode (dev,inode) COMMENT 'this pair is unique'" /* */
            " )",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_PROPS " ("              /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", data_id INTEGER, UNIQUE INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", mode INTEGER"                                         /* */
            ", uid INTEGER, INDEX uid (uid)"                         /* */
            ", gid INTEGER, INDEX gid (gid)"                         /* */
            ", atim DATETIME(6), INDEX atim (atim)"                  /* */
            ", mtim DATETIME(6), INDEX mtim (mtim)"                  /* */
            ", ctim DATETIME(6), INDEX ctim (ctim)"                  /* */
            ", size BIGINT NOT NULL, INDEX size (size), FOREIGN KEY (size) REFERENCES " QSTD_TABLE_SIZES " (size)" /* */
            ", last_updated DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", rdev INTEGER"                                         /* */
            ", blksize INTEGER"                                      /* */
            ", blocks INTEGER"                                       /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_PARENTS " ("            /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", dir_id INTEGER, UNIQUE INDEX dir (dir_id)"            /* */
            ", nchilds INTEGER NOT NULL, INDEX nchilds (nchilds)"    /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_NAMES " ("              /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", parent_id INTEGER NOT NULL, INDEX parent (parent_id), FOREIGN KEY (parent_id) REFERENCES " QSTD_TABLE_PARENTS " (id)" /* */
            ", name VARCHAR(255) COMMENT 'NULL is root', INDEX name (name)" /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", data_id INTEGER, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", UNIQUE INDEX parent_name (parent_id, name) COMMENT 'this pair is unique'" /* */
            ")",
    "CREATE  VIEW " QSTD_VIEW_ALL " AS "                             /* */
            " SELECT fn.name, fn.parent_id, fn.id AS name_id, fd.id AS data_id, p.id AS node_id, fp.detype, fd.inode " /* */
            "     , fp.atim AS atim, fp.mtim AS mtim, fp.ctim AS ctim " /* */
            "     , fs.nsame AS nsamesize"                           /* */
            "     , fd.dev, fp.mode, fd.nlink, fp.uid, fp.gid, fp.size, fp.blksize, fp.blocks, fp.rdev "
          /* */
            "        , GREATEST(fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS least_updated " /* */
            "   FROM " QSTD_TABLE_NAMES " AS fn "                    /* */
            "   LEFT JOIN " QSTD_TABLE_PARENTS "    AS  p ON (fn.id=p.dir_id) " /* */
            "   LEFT JOIN " QSTD_TABLE_DATAS "  AS fd ON (fn.data_id=fd.id) " /* */
            "   JOIN " QSTD_TABLE_PROPS "       AS fp ON (fp.data_id=fd.id) " /* */
            "   LEFT JOIN " QSTD_TABLE_SIZES "  AS fs ON (fp.size=fs.size) ", /* */
    "CREATE  VIEW " QSTD_VIEW_FILES " AS "                           /* */
            " SELECT fn.name, fn.parent_id, fn.id AS name_id, fd.id AS data_id, fp.mtim AS mtim, fs.nsame AS nsamesize, fp.size AS size " /* */
            "        , GREATEST(fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS least_updated " /* */
            "   FROM " QSTD_TABLE_NAMES " AS fn "                    /* */
            "   LEFT JOIN " QSTD_TABLE_DATAS "  AS fd ON (fn.data_id=fd.id) " /* */
            "   JOIN " QSTD_TABLE_PROPS "       AS fp ON (fp.data_id=fd.id) " /* */
            "   LEFT JOIN " QSTD_TABLE_SIZES "  AS fs ON (fp.size=fs.size) " /* */
            " WHERE fp.detype='REG'",
    "CREATE  VIEW " QSTD_VIEW_DIRS " AS "                            /* */
            " SELECT p.id AS node_id, fn.parent_id AS parent_id, fn.name AS name, fn.id AS name_id, fd.id AS data_id, fp.mtim AS mtim" /* */
            "        , GREATEST(fn.last_updated,p.last_updated,fd.last_updated,fp.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,p.last_updated,fd.last_updated,fp.last_updated) AS least_updated " /* */
            "   FROM " QSTD_TABLE_NAMES " AS fn "                    /* */
            "   LEFT JOIN " QSTD_TABLE_PARENTS "    AS  p ON (fn.id=p.dir_id) " /* */
            "   LEFT JOIN " QSTD_TABLE_DATAS "  AS fd ON (fn.data_id=fd.id) " /* */
            "   JOIN " QSTD_TABLE_PROPS "       AS fp ON (fp.data_id=fd.id) " /* */
            " WHERE fp.detype='DIR'",
    "COMMIT",
  };

  if ( qstd && qstd->pfs )
  {
    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !rCODE; i++ )
    {
      rC( mas_mysqlpfs_query( qstd->pfs, creops[i] ) );
      INFO( "(%d) %s", rCODE, creops[i] );
    }
  }

  rRET;
}

int
mas_qstd_drop_tables( mas_qstd_t * qstd )
{
  rDECL( 0 );
  const char *creops[] _uUu_ = {
    "START TRANSACTION",
    "DROP VIEW  IF EXISTS " QSTD_VIEW_ALL "",
    "DROP VIEW  IF EXISTS " QSTD_VIEW_FILES "",
    "DROP VIEW  IF EXISTS " QSTD_VIEW_DIRS "",
    "DROP TABLE IF EXISTS " QSTD_TABLE_NAMES "",
    "DROP TABLE IF EXISTS " QSTD_TABLE_PARENTS "",
    "DROP TABLE IF EXISTS " QSTD_TABLE_PROPS "",
    "DROP TABLE IF EXISTS " QSTD_TABLE_DATAS "",
    "DROP TABLE IF EXISTS " QSTD_TABLE_SIZES "",
    "COMMIT",
  };

  if ( qstd && qstd->pfs )
  {
    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !rCODE; i++ )
    {
      rC( mas_mysqlpfs_query( qstd->pfs, creops[i] ) );
      INFO( "(%d) %s", rCODE, creops[i] );
    }
  }

  rRET;
}

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
mas_qstd_mstmt_init_prepare( mas_qstd_t * qstd, mas_qstd_id_t stdid )
{
  rDECL( 0 );
  mysqlpfs_mstmt_t *mstmt = NULL;

  QRGP( qstd->pfs );
  if ( qstd->pfs )
  {
    mysqlpfs_t *pfs = qstd->pfs;

    switch ( stdid )
    {
    case STD_MSTMT_INSERT_NAMES:
      {
        char *insop = "INSERT INTO " QSTD_TABLE_NAMES "(name,parent_id,data_id,detype) VALUES (?,?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 4, 0, insop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 2 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 3, 255 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_NAMES_ID:
      {
        char *selop = "SELECT id FROM " QSTD_TABLE_NAMES " AS fn "   /* "LEFT JOIN "QSTD_TABLE_PARENTS" as p ON (fn.parent_id=p.id)" */
                " WHERE name=? AND fn.parent_id<=>?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_INSERT_PARENTS:
      {
        char *insop = "INSERT INTO " QSTD_TABLE_PARENTS "(dir_id) VALUES (?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_PARENTS_ID:
      {
        char *selop = "SELECT id FROM " QSTD_TABLE_PARENTS " WHERE dir_id<=>?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_INSERT_SIZES:
      {
        char *insop = "INSERT IGNORE INTO " QSTD_TABLE_SIZES "(size) VALUES (?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_SIZES_ID:
      {
        char *selop = "SELECT size FROM " QSTD_TABLE_SIZES " WHERE size=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_INSERT_DATAS:
      {
        char *insop = "INSERT IGNORE INTO " QSTD_TABLE_DATAS "(dev,inode,nlink) VALUES (?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 3, 0, insop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 2 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_DATAS_ID:
      {
        char *selop = "SELECT id FROM " QSTD_TABLE_DATAS " WHERE dev=? AND inode=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_INSERT_PROPS:
      {
      /*                                             0       1      2    3   4   5    6    7    8    9    a       b    */
        char *insop = "INSERT  INTO " QSTD_TABLE_PROPS "(data_id,detype,mode,uid,gid,atim,mtim,ctim,size,rdev,blksize,blocks) " /* */
              /*         0 1 2 3 4  5                6                7               8 9 a b  */
                "VALUES (?,?,?,?,?,FROM_UNIXTIME(?),FROM_UNIXTIME(?),FROM_UNIXTIME(?),?,?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 12, 0, insop );
        QRGP( mstmt );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 1, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 2 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 3 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 4 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 5 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 6 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 7 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 8 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 9 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 10 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 11 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_PROPS_ID:
      {
        char *selop = "SELECT id FROM " QSTD_TABLE_PROPS " WHERE data_id=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_NODES_ID:
      {
        char *selop = "SELECT node_id FROM " QSTD_VIEW_DIRS " WHERE parent_id=? AND name=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );
        QRGP( mstmt );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 1, 255 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
      break;
    case STD_MSTMT_MAX:
      break;
    }
  }
  return mstmt;
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_init( mas_qstd_t * qstd, mas_qstd_id_t stdid )
{
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
    mstmt = mstmts[stdid] = mas_qstd_mstmt_init_prepare( qstd, stdid );
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
