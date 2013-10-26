#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#include <sqlite3.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_def.h"
#include "duf_utils.h"
#include "duf_insert.h"


#include "duf_md5.h"

static sqlite3_int64
update_md5_file( const char *fpath, sqlite3_int64 filedataid )
{
  sqlite3_int64 resmd = -1;
  FILE *f;

  f = fopen( fpath, "r" );
  if ( f )
  {
    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    char *buffer;
    size_t bufsz = 1024 * 1024 * 10;

    buffer = mas_malloc( bufsz );
    MD5_Init( &ctx );
    while ( !feof( f ) )
    {
      int r;

      r = fread( buffer, 1, bufsz, f );
      MD5_Update( &ctx, buffer, r );
    }
    fclose( f );
    mas_free( buffer );
    MD5_Final( md, &ctx );
    {

      resmd = insert_md5( ( ( unsigned long long * ) &md[0] ) );
      fprintf( stderr, "resmd: %llu: %s\x1b[K\r", resmd, fpath );
      SQL_EXEC( "UPDATE filedatas SET md5id='%llu' WHERE id='%lld'", resmd, filedataid );
    }
  }
  else
  {
    fprintf( stderr, "ERROR open file : %s\n", fpath );
  }
  return resmd;
}

void
update_md5( void )
{
  char *sql;
  int row, column;
  char **presult = NULL;
  int r;

  fprintf( stderr, "Calc checksum\n" );

  sql = sqlite3_mprintf
        ( "SELECT filedatas.id, filedatas.dev, filedatas.inode, filedatas.md5id, filedatas.size, filenames.id, filenames.pathid, filenames.name "
          " FROM filedatas " " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) " );
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  /* fprintf( stderr, "%ux%u : %s\n", row, column, sql ); */
  if ( r == SQLITE_OK )
  {
    if ( row > 0 )
    {
      char *path = NULL;
      sqlite3_int64 pathid = -1, old_pathid = -1, nameid = -1;

      for ( int ir = column; ir <= column * row; ir += column )
      {
        char *fname;
        sqlite3_int64 filedataid;
        size_t fsize = 0;
        sqlite3_int64 resmd = -1;
        ino_t inode;


        filedataid = strtoll( presult[ir + 0], NULL, 10 );
        inode = strtoll( presult[ir + 2], NULL, 10 );
        if ( presult[ir + 3] )
          resmd = strtoll( presult[ir + 3], NULL, 10 );
        if ( presult[ir + 4] )
          fsize = strtoll( presult[ir + 4], NULL, 10 );
        nameid = strtoll( presult[ir + 5], NULL, 10 );
        pathid = strtoll( presult[ir + 6], NULL, 10 );
        fname = presult[ir + 7];

        if ( old_pathid != pathid || !path )
        {
          if ( path )
            mas_free( path );
          path = path_id_to_path( pathid );
        }
        if ( fsize )
        {
          char *fpath;

          fpath = join_path( path, fname );
          if ( !resmd && fsize > 0 )
            resmd = update_md5_file( fpath, filedataid );
          insert_keydata( pathid, nameid, inode, resmd );
          mas_free( fpath );
        }
        old_pathid = pathid;
      }
      if ( path )
        mas_free( path );
    }
  }
  else
    SQL_ERR( r );
  sqlite3_free_table( presult );
  sqlite3_free( sql );
}
