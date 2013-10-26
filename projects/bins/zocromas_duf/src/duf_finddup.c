#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <sqlite3.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_def.h"
#include "duf_utils.h"
#include "duf_remove.h"

#include "duf_finddup.h"

static void
get_duplicates( sqlite3_int64 md5id )
{
  int r;
  char *sql;
  int row, column;
  char **presult = NULL;

  fprintf( stderr, "md5id: %llu ========================================================\n", md5id );
  sql = sqlite3_mprintf( "SELECT filenames.pathid, filedatas.inode, filedatas.md5id, filedatas.size, filenames.name "
                         " FROM md5 " " LEFT JOIN filedatas ON (md5.id=filedatas.md5id) "
                         " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) "
                         " LEFT JOIN paths ON (filenames.pathid=paths.id) " " WHERE " "filedatas.md5id='%llu'", md5id );

  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    if ( row )
      for ( int ir = column; ir <= column * row; ir += column )
      {
        char *path;
        char *fname;
        size_t fsize;

        fname = presult[ir + 4];
        fsize = strtol( presult[ir + 3], NULL, 10 );
        path = path_id_to_path( strtoll( presult[ir + 0], NULL, 10 ) );
        if ( fname && path )
        {
          char *fpath;

          fpath = join_path( path, fname );
          if ( ir == column )
          {
            fprintf( stderr, "md5id: %5s size:%lu\n", presult[ir + 2], fsize );
          }
          fprintf( stderr, "  %u. pathid: %5s inode: %9s fpath: %s\n", ir / column, presult[ir], presult[ir + 1], fpath );
          mas_free( fpath );
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

void
find_duplicates( void )
{
  int r;
  char *sql;
  int row, column;
  char **presult = NULL;

  fprintf( stderr, "Find duplicates\n" );

  sql = sqlite3_mprintf( "SELECT " " md5.id, COUNT(*) as cnt "
                         " FROM md5 " " LEFT JOIN filedatas ON (md5.id=filedatas.md5id) "
                         " LEFT JOIN filenames ON (filedatas.id=filenames.dataid) "
                         " LEFT JOIN paths ON (filenames.pathid=paths.id) "
                         " WHERE " " dupcnt is null " " GROUP BY md5sum1,md5sum2 " " HAVING cnt>1 " " ORDER BY cnt ASC" );
  /* cnt ==> md5.dupcnt */
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    fprintf( stderr, "dupcnt row:%u\n", row );
    if ( row )
      for ( int ir = column; ir <= column * row; ir += column )
      {
        sqlite3_int64 cnt;
        sqlite3_int64 md5id;

        cnt = strtoll( presult[ir + 1], NULL, 10 );
        md5id = strtoll( presult[ir], NULL, 10 );
        fprintf( stderr, "%u. %llu dupcnt:%llu\x1b[K\r", ir / column, md5id, cnt );
        SQL_EXEC( "UPDATE md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id );
      }
  }
  else
    SQL_ERR( r );
  sqlite3_free_table( presult );
  sqlite3_free( sql );
}

void
print_duplicates( void )
{
  int r;
  char *sql;
  int row, column;
  char **presult = NULL;


  sql = sqlite3_mprintf( "SELECT " " md5.id, md5.dupcnt " " FROM md5 " " WHERE dupcnt IS NOT NULL" " ORDER BY dupcnt DESC" );
  /* cnt ==> md5.dupcnt */
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    if ( row )
      for ( int ir = column; ir <= column * row; ir += column )
      {
        /* sqlite3_int64 cnt; */
        sqlite3_int64 md5id;

        md5id = strtoll( presult[ir], NULL, 10 );
        /* cnt = strtoll( presult[ir + 1], NULL, 10 ); */
        get_duplicates( md5id );
      }
  }
  else
    SQL_ERR( r );
  sqlite3_free_table( presult );
  sqlite3_free( sql );
}

void
print_duplicate_dirs( void )
{
  int r;
  char *sql =
        "SELECT id, mdline, COUNT(*) AS cnt FROM paths " " WHERE mdline IS NOT NULL and mdline != ';'" " GROUP BY mdline "
        " HAVING cnt>1 " " ORDER BY cnt DESC";
  int row, column;
  char **presult = NULL;

  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    if ( row )
    {
      for ( int ir = column; ir <= column * row; ir += column )
      {
        char *mdline;
        /* sqlite3_int64 cnt; */

        mdline = presult[ir + 1];
        /* cnt = strtoll( presult[ir + 2], NULL, 10 ); */
        {
          int r2;
          char *sql2;
          int row2, column2;
          char **presult2 = NULL;

          sql2 = sqlite3_mprintf( "SELECT id FROM paths " " WHERE mdline = '%s'", mdline );
          r2 = sqlite3_get_table( pDb, sql2, &presult2, &row2, &column2, &errmsg );
          if ( r2 == SQLITE_OK )
          {
            if ( row2 )
            {
              char **targv = NULL;
              int targc = 0;
              char *path_ids = NULL;

              for ( int ir2 = column2; ir2 <= column2 * row2; ir2 += column2 )
              {
                char *path;
                sqlite3_int64 pathid;

                pathid = strtoll( presult2[ir2], NULL, 10 );
                path = path_id_to_path( pathid );
                targc = mas_add_argv_arg( targc, &targv, path );
                if ( path_ids )
                  path_ids = mas_strcat_x( path_ids, "," );
                path_ids = mas_strcat_x( path_ids, presult2[ir2] /* pathid */  );
                if ( path )
                  mas_free( path );
              }
              fprintf( stderr, "========================< %d >\n", targc );
              fprintf( stderr, "====< %s >\n", path_ids );
              remove_same_files_from_same_dirs( path_ids, targc, targv );
              mas_del_argv( targc, targv, 0 );
              fprintf( stderr, "==========================\n" );
              if ( path_ids )
                free( path_ids );
            }
          }
          sqlite3_free( sql2 );
          sqlite3_free_table( presult2 );
        }
        /* fprintf( stderr, "[%llu] %s\n", cnt, mdline ); */
      }
    }
  }
  sqlite3_free_table( presult );
}

void
print_hardlinks( void )
{
  int r;
  int row, column;
  char **presult = NULL;

  char *sql = "SELECT inode, COUNT(*) AS cnt FROM keydata GROUP BY inode HAVING cnt>1 ORDER BY inode";

  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
  if ( r == SQLITE_OK )
  {
    fprintf( stderr, "-Hardlinks: [%u]--------\n", row );
    if ( row )
      for ( int ir = column; ir <= column * row; ir += column )
      {
        int r2;
        int row2, column2;
        char **presult2 = NULL;
        char *sql2;
        sqlite3_int64 inode;

        inode = strtoll( presult[ir + 0], NULL, 10 );
        sql2 = sqlite3_mprintf( "SELECT keydata.pathid, filenames.id, filenames.name "
                                " FROM keydata LEFT JOIN filenames on (keydata.nameid=filenames.id) WHERE inode='%llu'", inode );

        r2 = sqlite3_get_table( pDb, sql2, &presult2, &row2, &column2, &errmsg );
        if ( r2 == SQLITE_OK )
        {
          fprintf( stderr, "---row2:[%u]--------\n", row2 );
          if ( row2 )
            for ( int ir2 = column2; ir2 <= column2 * row2; ir2 += column2 )
            {
              sqlite3_int64 pathid;
              sqlite3_int64 nameid;
              char *fpath;

              pathid = strtoll( presult2[ir2 + 0], NULL, 10 );
              nameid = strtoll( presult2[ir2 + 1], NULL, 10 );
              fpath = path_id_to_path( pathid );
              fpath = mas_strcat_x( fpath, "/" );
              fpath = mas_strcat_x( fpath, presult2[ir2 + 2] );
              fprintf( stderr, "%llu : %llu : '%s'\n", inode, nameid, fpath );
              if ( fpath )
                mas_free( fpath );
            }
          fprintf( stderr, "-----------=-=\n" );
        }
        else
          SQL_ERR( r2 );
        sqlite3_free_table( presult2 );
      }
  }
  else
    SQL_ERR( r );
  sqlite3_free_table( presult );
}
