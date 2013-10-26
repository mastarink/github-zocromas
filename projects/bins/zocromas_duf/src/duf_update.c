#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#include <sqlite3.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_def.h"
#include "duf_utils.h"
#include "duf_dirent.h"
#include "duf_insert.h"

#include "duf_update.h"



static void
update_file_ni( const char *path, const char *fname, ino_t file_inode, struct stat *pst_dir, sqlite3_int64 dir_id )
{
  sqlite3_int64 resd = 0;
  sqlite3_int64 resf = 0;
  struct stat st_file, *pst_file = NULL;

  {
    int r;
    char *fpath;

    fpath = join_path( path, fname );
    r = stat( fpath, &st_file );
    if ( !r )
      pst_file = &st_file;
    mas_free( fpath );
  }
  resd = insert_filedata( file_inode, pst_dir, pst_file );
  resf = insert_filename( fname, dir_id, resd );
}

static void
update_file_de( const char *path, const struct dirent *de, struct stat *pst_dir, sqlite3_int64 dir_id )
{
  update_file_ni( path, de->d_name, de->d_ino, pst_dir, dir_id );
}

static void
update_dirent( const char *wpath, struct dirent *de, struct stat *pst_dir, sqlite3_int64 dir_id, int recurse, int dofiles )
{
  if ( recurse && de->d_type == DT_DIR && !( de->d_name[0] == '.' && ( de->d_name[1] == '.' || de->d_name[1] == 0 ) ) )
  {
    char *recpath;

    recpath = join_path( wpath, de->d_name );
    update_path( recpath, dir_id, recurse, dofiles );
    if ( recpath )
      mas_free( recpath );
  }
  else if ( dofiles && de->d_type == DT_REG )
    update_file_de( wpath, de, pst_dir, dir_id );
}

static int
update_path_entries( const char *wpath, struct stat *pst_dir, sqlite3_int64 dir_id, int recurse, int dofiles )
{
  struct dirent **list = NULL;
  int nlist;

  nlist = scandir( wpath, &list, direntry_filter, alphasort );
  if ( nlist < 0 )
    fprintf( stderr, "ERROR %s: nlist = %d\n", wpath, nlist );
  for ( int il = 0; il < nlist; il++ )
  {
    update_dirent( wpath, list[il], pst_dir, dir_id, recurse, dofiles );
    mas_free( list[il] );
  }
  mas_free( list );
  return nlist;
}

sqlite3_int64
update_rpath( char *rpath, sqlite3_int64 dir_id_up, int recurse, int dofiles, const char *tail )
{
  sqlite3_int64 dir_id = 0;
  char *dir_name;
  const char *base_name;

  /* fprintf( stderr, "Scan %s\n", rpath ); */

  base_name = basename( rpath );
  dir_name = dirname( rpath );
  if ( base_name && !*base_name && dir_name && *dir_name == '/' && dir_name[1] == 0 )
    base_name = tail;
  {
    char *wpath = NULL;

    wpath = join_path( dir_name, base_name );
    {
      int r;
      struct stat st_dir;

      r = stat( wpath, &st_dir );
      if ( !r )
      {
        int items = 0;

        if ( dir_id_up < 0 )
          dir_id_up = update_path( dir_name, -1, 0, 0 );
        dir_id = insert_path( base_name, &st_dir, dir_id_up );

        if ( dir_id > 0 && *dir_name && !( *dir_name == '.' && ( !dir_name[1] || dir_name[1] == '.' ) ) )
          items = update_path_entries( wpath, &st_dir, dir_id, recurse, dofiles );
        SQL_EXEC( "UPDATE paths SET items='%u' WHERE id='%lu'", items, dir_id );
      }
    }
    if ( wpath )
      mas_free( wpath );
  }
  return dir_id;
}

sqlite3_int64
update_path( const char *path, sqlite3_int64 dir_id_up, int recurse, int dofiles )
{
  sqlite3_int64 dir_id = 0;
  char *rpath;

  rpath = mas_malloc( PATH_MAX );
  if ( rpath )
  {
    ( void ) realpath( path, rpath );
    if ( strlen( rpath ) > 1 )
      dir_id = update_rpath( rpath, dir_id_up, recurse, dofiles, path + 1 );

    mas_free( rpath );
  }
  return dir_id;
}

void
update_mdline_path( sqlite3_int64 pathid )
{
  char *sql2;

  sql2 = sqlite3_mprintf( "SELECT md5id " " FROM keydata " " WHERE pathid='%llu' " " ORDER BY md5id", pathid );
  {
    int r;
    int row, column;
    char **presult = NULL;

    r = sqlite3_get_table( pDb, sql2, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      /* fprintf( stderr, "pathid:%llu - #%u\n", pathid, row ); */
      fprintf( stderr, "Calc mdline pathid:%llu [%s] : %u\n", pathid, sql2, row );
      if ( row )
      {
        char *buf;
        char *pbuf;
        size_t len;

        len = row * 16 + 1;
        buf = mas_malloc( len );
        pbuf = buf;
        for ( int ir = column; ir <= column * row; ir += column )
        {
          sqlite3_int64 md5id;
          size_t l;

          md5id = strtol( presult[ir], NULL, 10 );
          fprintf( stderr, "Calc mdline %u - pathid:%llu; md5id:%llu\x1b[K\r", ir / column, pathid, md5id );
          snprintf( pbuf, len, "%llx;", md5id );
          l = strlen( pbuf );
          pbuf += l;
          len -= l;
        }
        fprintf( stderr, ">> %s :: %llu\n", buf, pathid );
        SQL_EXEC( "UPDATE paths SET mdline='%s' WHERE id='%lu'", buf, pathid );
        mas_free( buf );
      }
      else
      {
        SQL_EXEC( "UPDATE paths SET mdline='%s' WHERE id='%lu'", ";", pathid );
      }
      sqlite3_free_table( presult );
    }
  }
  sqlite3_free( sql2 );
}

void
update_mdline( void )
{
  int r;
  char *sql = "SELECT id " " FROM paths " " WHERE mdline IS NULL " " ORDER BY id";
  int row, column;
  char **presult = NULL;

  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    if ( row )
    {
      fprintf( stderr, "Calc mdline's %u\n", row );
      for ( int ir = column; ir <= column * row; ir += column )
      {
        sqlite3_int64 pathid;

        pathid = strtol( presult[ir], NULL, 10 );
        update_mdline_path( pathid );
      }
    }
  }
  sqlite3_free_table( presult );
}
