#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include "mysqlpfs_base.h"
#include "mysqlpfs.h"

int
test1( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  {
    mysqlpfs_row_t row;
    int num = 0;

    mas_mysqlpfs_query_result( pfs, "SHOW STATUS" );
    while ( ( row = mas_mysqlpfs_fetch_row( pfs ) ) )
    {
      printf( "%d #X# %s: %s\n", num, row[0], row[1] );
      num++;
    }

    mas_mysqlpfs_free_result( pfs );
  }
  mysqlpfs_delete( pfs );

/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}

/***
 ***/

static int
row_cb( mysqlpfs_row_t row, int num )
{
  printf( "%d #X# %s: %s\n", num, row[0], row[1] );
  return 0;
}

static _uUu_ int
test2( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

/* fprintf( stderr, "PFS:%p\n", pfs ); */
  if ( pfs )
  {
    mas_mysqlpfs_query_result_cb( pfs, "SHOW STATUS", row_cb );

    mysqlpfs_delete( pfs );
  }
/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}

static _uUu_ int
test3a( void )
{
  const char *creops[] _uUu_ = {
    "START TRANSACTION",
    "DROP TABLE IF EXISTS filenames",
    "DROP TABLE IF EXISTS filedatas",
    "DROP TABLE IF EXISTS fileprops",
    "DROP TABLE IF EXISTS filesizes",
    "COMMIT",
  };
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !r; i++ )
    {
      r = mas_mysqlpfs_query( pfs, creops[i] );
      fprintf( stderr, "(%d) %s\n", r, creops[i] );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}

static _uUu_ int
test3b( void )
{
  char *creops[] _uUu_ = {
    "START TRANSACTION",
    "CREATE TABLE IF NOT EXISTS filesizes ("                         /* */
            "size INTEGER  PRIMARY KEY"                              /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS fileprops ("                         /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK')" /* */
            ", mode INTEGER"                                         /* */
            ", nlink INTEGER"                                        /* */
            ", uid INTEGER"                                          /* */
            ", gid INTEGER"                                          /* */
            ", atim DATETIME"                                        /* */
            ", mtim DATETIME"                                        /* */
            ", ctim DATETIME"                                        /* */
            ", size INTEGER"                                         /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" /* */
            ", rdev INTEGER"                                         /* */
            ", INDEX size (size)"                                    /* */
            ", INDEX detype (detype)"                                /* */
            ", FOREIGN KEY (size) REFERENCES filesizes (size)"       /* */
            ")",
    "CREATE TABLE IF NOT EXISTS filedatas (" "id INTEGER PRIMARY KEY AUTO_INCREMENT" /* */
            ", dev INTEGER NOT NULL"                                 /* */
            ", inode INTEGER NOT NULL"                               /* */
            ", props_id INTEGER, INDEX props (props_id), FOREIGN KEY (props_id) REFERENCES fileprops (id)" /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" /* */
            ", rdev INTEGER"                                         /* */
            ", UNIQUE INDEX dev_inoce (dev,inode) COMMENT 'this pair is unique'" /* */
          /* */ " )",
    "CREATE TABLE IF NOT EXISTS filenames ("                         /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", parent_id INTEGER COMMENT 'NULL is root', INDEX parent (parent_id), FOREIGN KEY (parent_id) REFERENCES filenames (id)" /* */
            ", name VARCHAR(255) COMMENT 'NULL is root', INDEX name (name)" /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP" /* */
            ", data_id INTEGER, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES filedatas (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", UNIQUE INDEX parent_name (parent_id,name) COMMENT 'this pair is unique'" /* */
            ")",
    "COMMIT",
  };
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !r; i++ )
    {
      r = mas_mysqlpfs_query( pfs, creops[i] );
      fprintf( stderr, "(%d) %s\n", r, creops[i] );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
/*
MYSQL_TYPE_TINY
MYSQL_TYPE_SHORT
MYSQL_TYPE_LONGLONG
MYSQL_TYPE_FLOAT
MYSQL_TYPE_DOUBLE
MYSQL_TYPE_TIME
MYSQL_TYPE_DATE
MYSQL_TYPE_DATETIME
MYSQL_TYPE_TIMESTAMP
MYSQL_TYPE_STRING
MYSQL_TYPE_BLOB
MYSQL_TYPE_NULL
*/
static int
testcb( const char *name, size_t depth, void *li _uUu_, void *pfsv )
{
  mysqlpfs_t *_uUu_ pfs = ( mysqlpfs_t * ) pfsv;

  fprintf( stderr, "%ld. %s\n", depth, name );
#if 0
  char *insops[] _uUu_ = {
    "PREPARE insname_stmt FROM 'INSERT INTO filenames SET name=?'"
          /* INSERT INTO filesizes */
          /* INSERT INTO fileprops */
          /* INSERT INTO filedatas */
            "EXECUTE insname_stmt USING @file_name",
    "DEALLOCATE PREPARE insname_stmt",
  };
#endif
  {
    int r = 0;
    char *insop = "INSERT INTO filenames(name) VALUES (?)";
    MYSQL_STMT *stmt = mysql_stmt_init( mas_mysqlpfs_mysql( pfs ) );

    unsigned long length = strlen( name );
    MYSQL_BIND param[1], result[1];

    r = mysql_stmt_prepare( stmt, insop, strlen( insop ) );
    fprintf( stderr, "PREPARE: %d\n", r );
    memset( param, 0, sizeof( param ) );
    memset( result, 0, sizeof( result ) );

    param[0].buffer_type = MYSQL_TYPE_STRING;
    param[0].buffer = ( char * ) name;
    param[0].buffer_length = 255;
  /* param[0].is_unsigned = 0; */
    param[0].is_null = 0;
    param[0].length = &length;

    r = mysql_stmt_bind_param( stmt, param );
    fprintf( stderr, "BIND: %d\n", r );

    r = mysql_stmt_execute( stmt );
    fprintf( stderr, "EXECUTE: %d\n", r );
/* mariadb_stmt_execute_direct(); */
#if 0
    mysql_stmt_reset(  );
#else
    r = mysql_stmt_close( stmt );
    fprintf( stderr, "CLOSE: %d\n", r );
#endif
    /* mysql_stmt_bind_result
     * mysql_stmt_fetch
     * */
  }
#if 0
  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( insops ) / sizeof( insops[0] ) && !r; i++ )
    {
    /* r = mas_mysqlpfs_query( pfs, insops[i] ); */
      fprintf( stderr, "(%d) %s\n", r, insops[i] );
    }
  }
#endif
  return 0;
}

static _uUu_ int
test4( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      char *path = masxfs_pathinfo_pi2path( pi );

      masxfs_pathinfo_each_depth_cb( pi, testcb, pfs );

      EXAMS( path, path0, "%s : %s" );
      fprintf( stderr, "restored path:%s\n", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* test1(  ); */
/* test2(  ); */
  test3a(  );
  test3b(  );
  test4(  );
  return 0;
}
