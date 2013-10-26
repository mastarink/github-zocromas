#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <sqlite3.h>

#include <mastar/wrap/mas_memory.h>
#include "duf_def.h"
#include "duf_utils.h"


#include "duf_insert.h"


sqlite3_int64
insert_filedata( sqlite3_int64 file_inode, struct stat *pst_dir, struct stat *pst_file )
{
  int r;
  sqlite3_int64 resd;
  char *sqli;

  sqli = sqlite3_mprintf( "INSERT INTO filedatas (dev,inode,size,md5id,now) values ('%lu','%lu','%lu','%lu',datetime())",
                          pst_dir->st_dev, file_inode, pst_file ? pst_file->st_size : 0, 0 );
  r = sqlite3_exec( pDb, sqli, NULL, NULL, &errmsg );
  sqlite3_free( sqli );
  if ( r == SQLITE_CONSTRAINT )
  {
    char *sqls;
    int row, column;
    char **presult = NULL;

    sqls = sqlite3_mprintf( "SELECT id FROM filedatas WHERE dev='%lu' and inode='%lu'", pst_dir->st_dev, file_inode );
    r = sqlite3_get_table( pDb, sqls, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row )
        for ( int ir = column; ir <= column * row; ir += column )
          resd = strtol( presult[ir], NULL, 10 );
    }
    else
      SQL_ERR( r );
    sqlite3_free_table( presult );
    sqlite3_free( sqls );
  }
  else if ( r == SQLITE_OK )
    resd = sqlite3_last_insert_rowid( pDb );
  else
    SQL_ERR( r );
  return resd;
}

sqlite3_int64
insert_path( const char *base_name, struct stat * pst_dir, sqlite3_int64 dir_id_up )
{
  sqlite3_int64 dir_id = 0;
  int r;
  char *sql;
  char *qbase_name;

  qbase_name = single_quotes_2( base_name );
  sql = sqlite3_mprintf( "INSERT INTO paths (dev, inode, dirname, parentid, now) values ('%lu','%lu','%s','%lu',datetime())",
                         pst_dir->st_dev, pst_dir->st_ino, qbase_name ? qbase_name : base_name, dir_id_up );
  mas_free( qbase_name );
  /* fprintf( stderr, "'%s' : '%s' : '%s' :: { %lu : %lu : %s } : %s\n", path, dir_name, base_name, pst_dir->st_dev, */
  /*          pst_dir->st_ino, dir_name, sql );                                                                      */
  r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );
  sqlite3_free( sql );
  sql = NULL;
  if ( r == SQLITE_CONSTRAINT )
  {
    int row, column;
    char **presult = NULL;

    sql = sqlite3_mprintf( "SELECT id FROM paths WHERE dev='%lu' and inode='%lu'", pst_dir->st_dev, pst_dir->st_ino );
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row == 1 )
        dir_id = strtol( presult[column], NULL, 10 );
    }
    else
      SQL_ERR( r );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
    sql = NULL;
  }
  else if ( r == SQLITE_OK )
    dir_id = sqlite3_last_insert_rowid( pDb );
  else
    SQL_ERR( r );
  return dir_id;
}

sqlite3_int64
insert_filename( const char *fname, sqlite3_int64 dir_id, sqlite3_int64 resd )
{
  sqlite3_int64 resf = -1;
  int r;
  char *sql;
  char *fqname;

  fqname = single_quotes_2( fname );
  sql = sqlite3_mprintf( "INSERT INTO filenames (pathid, dataid, name, now) values ('%lu','%lu','%s',datetime())",
                         dir_id, resd, fqname ? fqname : fname );
  r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );

  if ( r == SQLITE_OK )
  {
    resf = sqlite3_last_insert_rowid( pDb );
    fprintf( stderr, "%llu. [%s]\x1b[K\r", resf, fqname ? fqname : fname );
  }
  else if ( r == SQLITE_CONSTRAINT )
  {
  }
  else
    SQL_ERR( r );

  if ( fqname )
    mas_free( fqname );
  sqlite3_free( sql );
  return resf;
}

sqlite3_int64
insert_md5( unsigned long long *md64 )
{
  sqlite3_int64 resmd = -1;
  char *sql;
  int r;

  sql = sqlite3_mprintf( "INSERT INTO md5 (md5sum1,md5sum2,now) values ('%llu','%llu',datetime())", md64[1], md64[0] );
  r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );

  if ( r == SQLITE_CONSTRAINT )
  {
    char *sql;
    int row, column;
    char **presult = NULL;

    sql = sqlite3_mprintf( "SELECT id FROM md5 WHERE md5sum1='%llu' and md5sum2='%llu'", md64[1], md64[0] );
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK && row )
      for ( int ir = column; ir <= column * row; ir += column )
      {
        resmd = strtol( presult[ir], NULL, 10 );
      }
    else
      SQL_ERR( r );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
  }
  else if ( r == SQLITE_OK )
    resmd = sqlite3_last_insert_rowid( pDb );
  else
    SQL_ERR( r );

  sqlite3_free( sql );
  return resmd;
}

sqlite3_int64
insert_keydata( sqlite3_int64 pathid, sqlite3_int64 nameid, ino_t inode, sqlite3_int64 resmd )
{
  sqlite3_int64 resp = -1;
  char *sql;
  int r;

  sql = sqlite3_mprintf( "INSERT INTO keydata (pathid, nameid, inode, md5id) VALUES ('%llu', '%llu', '%llu', '%llu')", pathid,
                         nameid, inode, resmd );
  r = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg );

  if ( r == SQLITE_CONSTRAINT )
  {
  }
  else if ( r == SQLITE_OK )
    resp = sqlite3_last_insert_rowid( pDb );
  else
    SQL_ERR( r );

  sqlite3_free( sql );
  return resp;
}
