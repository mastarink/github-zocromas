#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
/* #include <string.h> */
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>
/* #include <mastar/mysqlpfs/mysqlpfs_query.h> */

#include "qstd_structs.h"
#include "qstd_enums.h"
#include "qstd_query.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_base.h"

static mas_qstd_t *instance = NULL;

int
mas_qstd_ret_code( mas_qstd_t * qstd )
{
  return qstd ? qstd->ret_code : -1;
}

void
mas_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix )
{
  if ( qstd )
  {
    qstd->pfs = mysqlpfs_create_setup( host, user, passwd, db, port, table_prefix );
    qstd->std_mstmts = mas_qstd_mstmt_create_array(  );
  }
}

mas_qstd_t *
mas_qstd_create( void )
{
  return mas_calloc( 1, sizeof( mas_qstd_t ) );
}

mas_qstd_t *
mas_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix )
{
  mas_qstd_t *qstd = mas_qstd_create(  );

  mas_qstd_init( qstd, host, user, passwd, db, port, table_prefix );
  return qstd;
}

mas_qstd_t *
mas_qstd_instance_setup( const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix )
{
  if ( !instance )
    instance = mas_qstd_create_setup( host, user, passwd, db, port, table_prefix );
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
  rDECLBAD;
  char *creops[] = {
    "START TRANSACTION",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_SIZES " ("              /* */
            "size BIGINT NOT NULL PRIMARY KEY"                       /* */
            ", nsame INTEGER NOT NULL DEFAULT 0, INDEX nsame (nsame)" /* */
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
            ", data_id INTEGER NOT NULL DEFAULT 0, UNIQUE INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", mode INTEGER"                                         /* */
            ", uid INTEGER, INDEX uid (uid)"                         /* */
            ", gid INTEGER, INDEX gid (gid)"                         /* */
            ", atim DATETIME(6), INDEX atim (atim)"                  /* */
            ", mtim DATETIME(6), INDEX mtim (mtim)"                  /* */
            ", ctim DATETIME(6), INDEX ctim (ctim)"                  /* */
            ", size BIGINT NOT NULL DEFAULT 0, INDEX size (size), FOREIGN KEY (size) REFERENCES " QSTD_TABLE_SIZES " (size)" /* */
            ", last_updated DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", rdev INTEGER"                                         /* */
            ", blksize INTEGER"                                      /* */
            ", blocks INTEGER"                                       /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_PARENTS " ("            /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", dir_id INTEGER, UNIQUE INDEX dir (dir_id)"            /* */
            ", nchilds INTEGER " /*" NOT NULL DEFAULT 0 " */ " , INDEX nchilds (nchilds)" /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_NAMES " ("              /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", parent_id INTEGER NOT NULL DEFAULT 0, INDEX parent (parent_id), FOREIGN KEY (parent_id) REFERENCES " QSTD_TABLE_PARENTS " (id)" /* */
            ", name VARCHAR(255) BINARY COMMENT 'NULL is root', INDEX name (name)" /* */
            ", last_updated  DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", data_id INTEGER NOT NULL DEFAULT 0, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
          /* ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" (* *) */
            ", UNIQUE INDEX parent_name (parent_id, name) COMMENT 'this pair is unique'" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_SHA1 " ("               /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", sum BINARY(20), UNIQUE INDEX sum (sum)"               /* */
            ", nsame INTEGER NOT NULL DEFAULT 0, INDEX nsame (nsame)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_SHA1DREF " ("           /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", data_id INTEGER NOT NULL DEFAULT 0, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
            ", sum_id INTEGER NOT NULL DEFAULT 0, INDEX sum (sum_id), FOREIGN KEY (sum_id) REFERENCES " QSTD_TABLE_SHA1 " (id)" /* */
            ", UNIQUE INDEX pair (data_id, sum_id) COMMENT 'this pair is unique'" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_MD5 " ("                /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", sum BINARY(16), UNIQUE INDEX sum (sum)"               /* */
            ", nsame INTEGER NOT NULL DEFAULT 0, INDEX nsame (nsame)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS " QSTD_TABLE_MD5DREF " ("            /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", data_id INTEGER NOT NULL DEFAULT 0, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES " QSTD_TABLE_DATAS " (id)" /* */
            ", sum_id INTEGER NOT NULL DEFAULT 0, INDEX sum (sum_id), FOREIGN KEY (sum_id) REFERENCES " QSTD_TABLE_MD5 " (id)" /* */
            ", UNIQUE INDEX pair (data_id, sum_id) COMMENT 'this pair is unique'" /* */
            ")",
    "CREATE OR REPLACE VIEW " QSTD_VIEW_ALL " AS "                   /* */
            " SELECT "                                               /* */
            " fn.name, fn.parent_id, fn.id AS name_id, fd.id AS data_id, p.id AS node_id, fp.detype, fd.inode " /* */
            "  , fp.atim AS atim, fp.mtim AS mtim, fp.ctim AS ctim " /* */
            "  , fs.nsame AS nsamesize"                              /* */
            "  , fd.dev, fp.mode, fd.nlink, fp.uid, fp.gid, fp.size, fp.blksize, fp.blocks, fp.rdev " /* */
            "  , sha.nsame AS nsamesha1, sha.id AS sha1id, LOWER(HEX(sha.sum)) AS hex_sha1" /* */
            "        , GREATEST(fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS least_updated " /* */
            "      FROM " QSTD_TABLE_NAMES "    AS fn "              /* */
            " LEFT JOIN " QSTD_TABLE_PARENTS "  AS  p ON (fn.id=p.dir_id) " /* */
            " LEFT JOIN " QSTD_TABLE_DATAS "    AS fd ON (fn.data_id=fd.id) " /* */
            "      JOIN " QSTD_TABLE_PROPS "    AS fp ON (fp.data_id=fd.id) " /* */
            " LEFT JOIN " QSTD_TABLE_SIZES "    AS fs ON (fp.size=fs.size) " /* */
            " LEFT JOIN " QSTD_TABLE_SHA1DREF " AS shd ON (shd.data_id=fd.id) " /* */
            " LEFT JOIN " QSTD_TABLE_SHA1 "     AS sha ON (shd.sum_id=sha.id) " /* */
            ,
    "CREATE OR REPLACE VIEW " QSTD_VIEW_FILES_OLD " AS "             /* */
            " SELECT  "                                              /* */
            " fn.name, fn.parent_id, fn.id AS name_id, fd.id AS data_id, p.id AS node_id, fp.detype, fd.inode " /* */
            "  , fp.atim AS atim, fp.mtim AS mtim, fp.ctim AS ctim " /* */
            "  , fs.nsame AS nsamesize"                              /* */
            "  , fd.dev, fp.mode, fd.nlink, fp.uid, fp.gid, fp.size, fp.blksize, fp.blocks, fp.rdev " /* */
            "  , sha.nsame AS nsamesha1, sha.id AS sha1id, LOWER(HEX(sha.sum)) AS hex_sha1" /* */
            "        , GREATEST(fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,fd.last_updated,fp.last_updated,fs.last_updated) AS least_updated " /* */
            "   FROM " QSTD_TABLE_NAMES " AS fn "                    /* */
            "   LEFT JOIN " QSTD_TABLE_PARENTS "  AS  p  ON (fn.id=p.dir_id) " /* */
            "   LEFT JOIN " QSTD_TABLE_DATAS "    AS fd  ON (fn.data_id=fd.id) " /* */
            "        JOIN " QSTD_TABLE_PROPS "    AS fp  ON (fp.data_id=fd.id) " /* */
            "   LEFT JOIN " QSTD_TABLE_SIZES "    AS fs  ON (fp.size=fs.size) " /* */
            "        JOIN " QSTD_TABLE_SHA1DREF " AS shd ON (shd.data_id=fd.id) " /* */
            "	LEFT JOIN " QSTD_TABLE_SHA1 "     AS sha ON (shd.sum_id=sha.id) " /* */
            " WHERE p.id IS NULL"                                    /* */
          /* " HAVING node_id IS NULL"                                    (* *) */
            ,
    "CREATE OR REPLACE VIEW " QSTD_VIEW_FILES " AS "                 /* */
            " SELECT * FROM " QSTD_VIEW_ALL " HAVING node_id IS NULL" /* */
            ,
  /* " WHERE fp.detype='REG'", */
    "CREATE OR REPLACE VIEW " QSTD_VIEW_DIRS " AS "                  /* */
            " SELECT p.id AS node_id, fn.parent_id AS parent_id, fn.name AS name, fn.id AS name_id, fd.id AS data_id, fp.mtim AS mtim" /* */
            "        , GREATEST(fn.last_updated,p.last_updated,fd.last_updated,fp.last_updated) AS latest_updated " /* */
            "        , LEAST(   fn.last_updated,p.last_updated,fd.last_updated,fp.last_updated) AS least_updated " /* */
            "   FROM " QSTD_TABLE_NAMES " AS fn "                    /* */
            "   LEFT JOIN " QSTD_TABLE_PARENTS "    AS  p ON (fn.id=p.dir_id) " /* */
            "   LEFT JOIN " QSTD_TABLE_DATAS "  AS fd ON (fn.data_id=fd.id) " /* */
            "   JOIN " QSTD_TABLE_PROPS "       AS fp ON (fp.data_id=fd.id) " /* */
            " WHERE p.id IS NOT NULL",
  /* " WHERE fp.detype='DIR'", */
    "COMMIT",
  };

  if ( qstd && qstd->pfs )
  {
    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ); i++ )
    {
      rC( mas_qstd_query( qstd, creops[i] ) );
      if ( rBAD )
      {
        WARN( "(%d) %s", rCODE, creops[i] );
        mas_qstd_query( qstd, "ROLLBACK" );
        break;
      }
    }
  }

  rRET;
}

int
mas_qstd_drop_tables( mas_qstd_t * qstd )
{
  rDECLBAD;
  const char *creops[] = {
    "START TRANSACTION",
    "DROP VIEW  IF EXISTS " QSTD_VIEW_ALL,
    "DROP VIEW  IF EXISTS " QSTD_VIEW_FILES,
    "DROP VIEW  IF EXISTS " QSTD_VIEW_FILES_OLD,
    "DROP VIEW  IF EXISTS " QSTD_VIEW_DIRS,
    "DROP TABLE IF EXISTS " QSTD_TABLE_NAMES,
    "DROP TABLE IF EXISTS " QSTD_TABLE_PARENTS,
    "DROP TABLE IF EXISTS " QSTD_TABLE_PROPS,
    "DROP TABLE IF EXISTS " QSTD_TABLE_MD5DREF,
    "DROP TABLE IF EXISTS " QSTD_TABLE_MD5,
    "DROP TABLE IF EXISTS " QSTD_TABLE_SHA1DREF,
    "DROP TABLE IF EXISTS " QSTD_TABLE_SHA1,
    "DROP TABLE IF EXISTS " QSTD_TABLE_DATAS,
    "DROP TABLE IF EXISTS " QSTD_TABLE_SIZES,
    "COMMIT",
  };

  if ( qstd && qstd->pfs )
  {
    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ); i++ )
    {
      rC( mas_qstd_query( qstd, creops[i] ) );
      if ( rBAD )
      {
        WARN( "(%d) %s", rCODE, creops[i] );
        mas_qstd_query( qstd, "ROLLBACK" );                          /* ????????? */
        break;
      }
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
        mas_mysqlpfs_mstmt_delete( mstmts[nst] );
      mstmts[nst] = NULL;
    }
}

void
mas_qstd_mstmt_delete_array( mysqlpfs_mstmt_t ** mstmts )
{
  mas_qstd_mstmt_reset_array( mstmts );
  mas_free( mstmts );
}

static int
mas_qstd_mstmt_init_prepare_results_everything( mysqlpfs_mstmt_t * mstmt, int *pnr )
{
  rDECLBAD;
  rC( mas_qstd_mstmt_bind_param( mstmt ) );
  rC( mas_qstd_mstmt_prepare_result_string( mstmt, ( *pnr )++ ) );   /* name */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* inode */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* node_id */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* dev */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* mode */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* nlink */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* uid */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* gid */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* size */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* blksize */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* blocks */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* rdev */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* atim */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* mtim */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* ctim */
/*
 *   +  name
 *      parent_id
 *      name_id
 *      data_id
 *   +  node_id
 *      detype
 *   +  inode
 *   +  atim
 *   +  mtim
 *   +  ctim
 *      nsamesize
 *   +  dev
 *   +  mode
 *   +  nlink
 *   +  uid
 *   +  gid
 *   +  size
 *   +  blksize
 *   +  blocks
 *   +  rdev
 *      nsamesha1
 *      sha1id
 *      hex_sha1
 */

/* TODO assert ( mysql_stmt_param_count(...) == (*pnr) ) */
  if ( rGOOD )
    assert( ( *pnr ) == STD_MSTMT_SELECT_EVERYTHING_NRESULTS );
  rC( mas_qstd_mstmt_bind_result( mstmt ) );
  rRET;
}

static int
mas_qstd_mstmt_init_prepare_results_everythingx( mysqlpfs_mstmt_t * mstmt, int *pnr )
{
  rDECLBAD;
  rC( mas_qstd_mstmt_bind_param( mstmt ) );
  rC( mas_qstd_mstmt_prepare_result_string( mstmt, ( *pnr )++ ) );   /* name */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* inode */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* node_id */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* dev */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* mode */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* nlink */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* uid */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* gid */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* size */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* blksize */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* blocks */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* rdev */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* atim */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* mtim */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* ctim */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* nsamesize */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* nsamesha1 */
  rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, ( *pnr )++ ) ); /* sha1id */
  rC( mas_qstd_mstmt_prepare_result_string( mstmt, ( *pnr )++ ) );   /* hex_sha1 */
/*
 *   +  name
 *      parent_id
 *      name_id
 *      data_id
 *   +  node_id
 *      detype
 *   +  inode
 *   +  atim
 *   +  mtim
 *   +  ctim
 *   +  nsamesize
 *   +  dev
 *   +  mode
 *   +  nlink
 *   +  uid
 *   +  gid
 *   +  size
 *   +  blksize
 *   +  blocks
 *   +  rdev
 *   +  nsamesha1
 *   +  sha1id
 *   +  hex_sha1
 */

  if ( rGOOD )
    assert( ( *pnr ) == STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS );
  if ( rGOOD )
    assert( ( *pnr ) == STD_MSTMT_SELECT_EVERYTHINGXX_NRESULTS );
  rC( mas_qstd_mstmt_bind_result( mstmt ) );
  rRET;
}

mysqlpfs_mstmt_t *
mas_qstd_mstmt_init_prepare( mas_qstd_t * qstd, mas_qstd_id_t stdid )
{
  rDECLBAD;
  mysqlpfs_mstmt_t *mstmt = NULL;

  QRGP( qstd->pfs );
  if ( qstd->pfs )
  {
    mysqlpfs_t *pfs = qstd->pfs;

    switch ( stdid )
    {
    case STD_MSTMT_INSERT_NAMES:
      {
        int np = 0;
        char *insop = "INSERT INTO " QSTD_TABLE_NAMES "(name,parent_id,data_id) VALUES (?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_NAMES_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, np++, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
      /* rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, np++, 255 ) ); */
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_NAMES_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_NAMES_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_NAMES " AS fn "   /* "LEFT JOIN "QSTD_TABLE_PARENTS" as p ON (fn.parent_id=p.id)" */
                " WHERE BINARY name=? AND fn.parent_id<=>?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_NAMES_NFIELDS, STD_MSTMT_SELECT_NAMES_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, np++, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_NAMES_NFIELDS );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_NAMES_NRESULTS );
      }
      break;
    case STD_MSTMT_INSERT_PARENTS:
      {
        int np = 0;
        char *insop = "INSERT INTO " QSTD_TABLE_PARENTS "(dir_id) VALUES (?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_PARENTS_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_PARENTS_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_PARENTS_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_PARENTS " WHERE dir_id<=>?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_PARENTS_NFIELDS, STD_MSTMT_SELECT_PARENTS_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_PARENTS_NFIELDS );

        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_PARENTS_NRESULTS );
      }
      break;
    case STD_MSTMT_INSERT_SIZES:
      {
        int np = 0;
        char *insop = "INSERT IGNORE INTO " QSTD_TABLE_SIZES "(size) VALUES (?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_SIZES_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_SIZES_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_SIZES_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT size FROM " QSTD_TABLE_SIZES " WHERE size=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_SIZES_NFIELDS, STD_MSTMT_SELECT_SIZES_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_SIZES_NFIELDS );

        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_SIZES_NRESULTS );
      }
      break;
    case STD_MSTMT_INSERT_DATAS:
      {
        int np = 0;
        char *insop = "INSERT IGNORE INTO " QSTD_TABLE_DATAS "(dev,inode,nlink) VALUES (?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_DATAS_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_DATAS_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_DATAS_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_DATAS " WHERE dev=? AND inode=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_DATAS_NFIELDS, STD_MSTMT_SELECT_DATAS_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_DATAS_NFIELDS );

        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_DATAS_NRESULTS );
      }
      break;
    case STD_MSTMT_INSERT_PROPS:
      {
        int np = 0;

      /*                                             0       1      2    3   4   5    6    7    8    9    a       b    */
        char *insop = "INSERT  INTO " QSTD_TABLE_PROPS "(data_id,mode,uid,gid,atim,mtim,ctim,size,rdev,blksize,blocks,detype) " /* */
              /*         0 1 2 3 4  5                6                7               8 9 a b  */
                "VALUES (?,?,?,?,FROM_UNIXTIME(?),FROM_UNIXTIME(?),FROM_UNIXTIME(?),?,?,?,?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_PROPS_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, np++, 255 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_PROPS_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_PROPS_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_PROPS " WHERE data_id=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_PROPS_NFIELDS );

        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_PROPS_NRESULTS );
      }
      break;
    case STD_MSTMT_INSERT_SHA1:
      {
        int np = 0;

        char *insop = "INSERT  INTO " QSTD_TABLE_SHA1 "(sum) VALUES (?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_SHA1_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_binary( mstmt, np++, 20 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_SHA1_NFIELDS );
      }
      break;
    case STD_MSTMT_INSERT_SHA1DREF:
      {
        int np = 0;

        char *insop = "INSERT  INTO " QSTD_TABLE_SHA1DREF "(data_id,sum_id) VALUES (?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_INSERT_SHA1DREF_NFIELDS, STD_MSTMT_INSERT_NRESULTS, insop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_INSERT_SHA1DREF_NFIELDS );
      }
      break;
    case STD_MSTMT_SELECT_SHA1_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_SHA1 " WHERE sum=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_binary( mstmt, np++, 20 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_PROPS_NFIELDS );

        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_PROPS_NRESULTS );
      }
      break;
    case STD_MSTMT_SELECT_SHA1DREF:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT id FROM " QSTD_TABLE_SHA1DREF " WHERE data_id=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 1, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_PROPS_NFIELDS );

        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_PROPS_NRESULTS );
      }
      break;
    case STD_MSTMT_SELECT_NODES_ID:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT node_id FROM " QSTD_VIEW_DIRS " WHERE parent_id=? AND BINARY name=?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_NODES_NFIELDS, STD_MSTMT_SELECT_NODES_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );

        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, np++ ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, np++, 255 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_NODES_NFIELDS );

        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, nr++ ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_NODES_NRESULTS );
      }
      break;
    case STD_MSTMT_SELECT_EVERYTHING_PN:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? AND name=? "                  /* */
              /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHING_NFIELDS_PN, STD_MSTMT_SELECT_EVERYTHING_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        rC( mas_qstd_mstmt_prepare_param_string( mstmt, np++ ) );    /* name */
      /* TODO assert ( mysql_stmt_param_count(...) == np ) */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHING_NFIELDS_PN );
#if 1
        rC( mas_qstd_mstmt_init_prepare_results_everything( mstmt, &nr ) );
#else
        rC( mas_qstd_mstmt_bind_param( mstmt ) );
        rC( mas_qstd_mstmt_prepare_result_string( mstmt, nr++ ) );   /* name */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* inode */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* node_id */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* dev */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* mode */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* nlink */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* uid */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* gid */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* size */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* blksize */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* blocks */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* rdev */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* atim */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* mtim */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* ctim */
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_EVERYTHING_NRESULTS );
        rC( mas_qstd_mstmt_bind_result( mstmt ) );
#endif
      }
      break;
    case STD_MSTMT_SELECT_EVERYTHING_P:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? "                             /* */
              /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHING_NFIELDS_P, STD_MSTMT_SELECT_EVERYTHING_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHING_NFIELDS_P );

        if ( rBAD )
          WARN( "%s", mas_qstd_mstmt_error( mstmt ) );

#if 1
        rC( mas_qstd_mstmt_init_prepare_results_everything( mstmt, &nr ) );
#else
        rC( mas_qstd_mstmt_bind_param( mstmt ) );
        rC( mas_qstd_mstmt_prepare_result_string( mstmt, nr++ ) );   /* name */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* inode */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* node_id */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* dev */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* mode */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* nlink */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* uid */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* gid */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* size */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* blksize */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* blocks */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* rdev */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* atim */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* mtim */
        rC( mas_qstd_mstmt_prepare_result_longlong( mstmt, nr++ ) ); /* ctim */
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_EVERYTHING_NRESULTS );
        rC( mas_qstd_mstmt_bind_result( mstmt ) );
#endif
      }
      break;
    case STD_MSTMT_SELECT_EVERYTHINGX_PN:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                ", nsamesize, nsamesha1, sha1id, hex_sha1 "          /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? AND name=? "                  /* */
              /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_PN, STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        rC( mas_qstd_mstmt_prepare_param_string( mstmt, np++ ) );    /* name */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_PN );
        if ( rBAD )
          WARN( "%s", mas_qstd_mstmt_error( mstmt ) );

        rC( mas_qstd_mstmt_init_prepare_results_everythingx( mstmt, &nr ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS );

      }
      break;
    case STD_MSTMT_SELECT_EVERYTHINGX_P:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                ", nsamesize, nsamesha1, sha1id, hex_sha1 "          /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? "                             /* */
              /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_P, STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_P );
        if ( rBAD )
          WARN( "%s", mas_qstd_mstmt_error( mstmt ) );

        rC( mas_qstd_mstmt_init_prepare_results_everythingx( mstmt, &nr ) );
        if ( rGOOD )
          assert( nr == STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS );
      }
      break;
    case STD_MSTMT_SELECT_EVERYTHINGXX_PN:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                ", nsamesize, nsamesha1, sha1id, hex_sha1 "          /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? AND name=? "                  /* */
                "     AND (detype IN ('DIR','LNK') "                 /* */
              /* ('BLK','CHR','DIR','FIFO','LNK','-REG','SOCK') */
                "     OR ("                                          /* */
                "          ( nsamesize>=? AND nsamesize<=? ) "       /* */
                "         AND ( nsamesha1>=? AND nsamesha1<=? ) "    /* */
                "         AND ( size>=? AND size<=? ) "              /* */
                "         AND ( inode>=? AND inode<=? ) "            /* */
                "         AND ( sha1id>=? AND sha1id<=? ) "          /* */
                "         AND ( name REGEXP ? )  "                   /* */
                "      ) "                                           /* */
                "     ) "                                            /* */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_PN, STD_MSTMT_SELECT_EVERYTHINGXX_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        rC( mas_qstd_mstmt_prepare_param_string( mstmt, np++ ) );    /* name */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_nsamesize */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_nsamesize */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_nsamesha1 */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_nsamesha1 */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_size */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_size */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_inode */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_inode */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_sha1id */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_sha1id */
        rC( mas_qstd_mstmt_prepare_param_string( mstmt, np++ ) );    /* regexp */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_PN );
        if ( rBAD )
          WARN( "%s", mas_qstd_mstmt_error( mstmt ) );

        rC( mas_qstd_mstmt_init_prepare_results_everythingx( mstmt, &nr ) );

      }
      break;
    case STD_MSTMT_SELECT_EVERYTHINGXX_P:
      {
        int np = 0;
        int nr = 0;
        char *selop = "SELECT "                                      /* */
                "  name, inode, node_id "                            /* */
                ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev, atim, mtim, ctim" /* */
                ", nsamesize, nsamesha1, sha1id, hex_sha1 "          /* */
                "   FROM " QSTD_VIEW_ALL                             /* */
                "    WHERE parent_id=? "                             /* */
                "     AND (detype IN ('DIR','LNK') "                 /* */
              /* ('BLK','CHR','DIR','FIFO','LNK','-REG','SOCK') */
                "     OR ("                                          /* */
                "          ( nsamesize>=? AND nsamesize<=? ) "       /* */
                "         AND ( nsamesha1>=? AND nsamesha1<=? ) "    /* */
                "         AND ( size>=? AND size<=? ) "              /* */
                "         AND ( inode>=? AND inode<=? ) "            /* */
                "         AND ( sha1id>=? AND sha1id<=? ) "          /* */
                "         AND ( name REGEXP ? )  "                   /* */
                "      ) "                                           /* */
                "     ) "                                            /* */
                "   ORDER BY name_id";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_P, STD_MSTMT_SELECT_EVERYTHINGXX_NRESULTS, selop );
        QRGP( mstmt );
        rC( mas_qstd_mstmt_ret_code( mstmt ) );
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* parent_id */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_nsamesize */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_nsamesize */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_nsamesha1 */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_nsamesha1 */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_size */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_size */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_inode */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_inode */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* min_sha1id */
        rC( mas_qstd_mstmt_prepare_param_longlong( mstmt, np++ ) );  /* max_sha1id */
        rC( mas_qstd_mstmt_prepare_param_string( mstmt, np++ ) );    /* regexp */
        if ( rGOOD )
          assert( np == STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_P );
        if ( rBAD )
          WARN( "%s", mas_qstd_mstmt_error( mstmt ) );

        rC( mas_qstd_mstmt_init_prepare_results_everythingx( mstmt, &nr ) );
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
  rDECLBAD;
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
    if ( mstmt )
      rSETGOOD;
    QRGP( mstmt );
  }
  else
  {
    WARN( "qstd:%p; stdid:%d; STD_MSTMT_MAX:%d;", qstd, stdid, STD_MSTMT_MAX );
    QRG( -1 );
  }
  qstd->ret_code = rCODE;
  return mstmt;
}
