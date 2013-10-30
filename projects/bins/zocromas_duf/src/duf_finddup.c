#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* #include <sqlite3.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

/* #include "duf_def.h" */
#include "duf_utils.h"
#include "duf_sql.h"
#include "duf_remove.h"

#include "duf_finddup.h"

int
zero_duplicates( void )
{
  int r;

  fprintf( stderr, "Zero duplicates\x1b[K\n" );
  r = duf_sql_exec( "UPDATE duf_md5 SET dupcnt=null" );
  return r;
}

static int
duf_sql_update_duplicates( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t  fun )
{
  unsigned long long cnt;
  unsigned long long md5id;

  cnt = strtoll( presult[1], NULL, 10 );
  md5id = strtoll( presult[0], NULL, 10 );
  fprintf( stderr, "%u. %llu dupcnt:%llu\x1b[K\r", nrow, md5id, cnt );
  return duf_sql( "UPDATE duf_md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id );
}

int
update_duplicates( void )
{
  int r;

  fprintf( stderr, "Find duplicates\x1b[K\n" );
  r = duf_sql_select( duf_sql_update_duplicates, NULL, NULL, 0, "SELECT " " duf_md5.id, COUNT(*) as cnt "
                      " FROM duf_md5 " " LEFT JOIN duf_filedatas ON (duf_md5.id=duf_filedatas.md5id) "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id) "
                      " WHERE " " dupcnt is null " " GROUP BY md5sum1,md5sum2 " " HAVING cnt>1 " " ORDER BY cnt ASC" );
  fprintf( stderr, "End find duplicates\x1b[K\n" );
  return r;
}

static int
duf_sql_get_duplicates( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t  fun )
{
  char *path;
  char *fname;
  size_t fsize;

  fname = presult[4];
  fsize = strtol( presult[3], NULL, 10 );
  path = path_id_to_path( strtoll( presult[0], NULL, 10 ) );
  if ( fname && path )
  {
    char *fpath;

    fpath = join_path( path, fname );
    if ( nrow == 0 )
    {
      fprintf( stderr, "md5id: %5s size:%lu\n", presult[2], fsize );
    }
    fprintf( stderr, "  %u. pathid: %5s inode: %9s fpath: %s\n", nrow, presult[0], presult[1], fpath );
    mas_free( fpath );
  }
  if ( path )
    mas_free( path );
  return 0;
}

static int
get_duplicates( unsigned long long md5id )
{
  int r = 0;

  r = duf_sql_select( duf_sql_get_duplicates, NULL, NULL, 0,
                      "SELECT duf_filenames.pathid, duf_filedatas.inode, duf_filedatas.md5id, duf_filedatas.size, duf_filenames.name "
                      " FROM duf_md5 " " LEFT JOIN duf_filedatas ON (duf_md5.id=duf_filedatas.md5id) "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id) " " WHERE " "duf_filedatas.md5id='%llu'", md5id );
  return r;
}

static int
duf_sql_print_duplicates( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t  fun )
{
  unsigned long long md5id;

  md5id = strtoll( presult[0], NULL, 10 );
  get_duplicates( md5id );
  return 0;
}

int
print_duplicates( unsigned long limit )
{
  int r;

  r = duf_sql_select( duf_sql_print_duplicates, NULL, NULL, 0,
                      "SELECT " " duf_md5.id, duf_md5.dupcnt " " FROM duf_md5 " " WHERE dupcnt IS NOT NULL"
                      " ORDER BY (dupcnt * size) DESC LIMIT %lu", limit );
  return r;
}

/* void                                                                                                                           */
/* print_duplicate_dirs_old( void )                                                                                               */
/* {                                                                                                                              */
/*   int r;                                                                                                                       */
/*   char *sql =                                                                                                                  */
/*         "SELECT id, mdline, COUNT(*) AS cnt FROM duf_paths " " WHERE mdline IS NOT NULL and mdline != ';'" " GROUP BY mdline " */
/*         " HAVING cnt>1 " " ORDER BY cnt DESC";                                                                                 */
/*   int row, column;                                                                                                             */
/*   char **presult = NULL;                                                                                                       */
/*                                                                                                                                */
/*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                                                         */
/*   if ( r == SQLITE_OK )                                                                                                        */
/*   {                                                                                                                            */
/*     if ( row )                                                                                                                 */
/*     {                                                                                                                          */
/*       for ( int ir = column; ir <= column * row; ir += column )                                                                */
/*       {                                                                                                                        */
/*         char *mdline;                                                                                                          */
/*                                                                                                                                */
/*         (* sqlite3_int64 cnt; *)                                                                                               */
/*                                                                                                                                */
/*         mdline = presult[ir + 1];                                                                                              */
/*         (* cnt = strtoll( presult[ir + 2], NULL, 10 ); *)                                                                      */
/*         {                                                                                                                      */
/*           int r2;                                                                                                              */
/*           char *sql2;                                                                                                          */
/*           int row2, column2;                                                                                                   */
/*           char **presult2 = NULL;                                                                                              */
/*                                                                                                                                */
/*                                                                                                                                */
/*           sql2 = sqlite3_mprintf( "SELECT id FROM duf_paths " " WHERE mdline = '%s'", mdline );                                */
/*           r2 = sqlite3_get_table( pDb, sql2, &presult2, &row2, &column2, &errmsg );                                            */
/*           if ( r2 == SQLITE_OK )                                                                                               */
/*           {                                                                                                                    */
/*             if ( row2 )                                                                                                        */
/*             {                                                                                                                  */
/*               char **targv = NULL;                                                                                             */
/*               int targc = 0;                                                                                                   */
/*               char *path_ids = NULL;                                                                                           */
/*                                                                                                                                */
/*               for ( int ir2 = column2; ir2 <= column2 * row2; ir2 += column2 )                                                 */
/*               {                                                                                                                */
/*                 char *path;                                                                                                    */
/*                 sqlite3_int64 pathid;                                                                                          */
/*                                                                                                                                */
/*                 pathid = strtoll( presult2[ir2], NULL, 10 );                                                                   */
/*                 path = path_id_to_path( pathid );                                                                              */
/*                 targc = mas_add_argv_arg( targc, &targv, path );                                                               */
/*                 if ( path_ids )                                                                                                */
/*                   path_ids = mas_strcat_x( path_ids, "," );                                                                    */
/*                 path_ids = mas_strcat_x( path_ids, presult2[ir2] (* pathid *)  );                                              */
/*                 if ( path )                                                                                                    */
/*                   mas_free( path );                                                                                            */
/*               }                                                                                                                */
/*               fprintf( stderr, "========================< %d >\n", targc );                                                    */
/*               fprintf( stderr, "====< %s >\n", path_ids );                                                                     */
/*               (* remove_same_files_from_same_dirs( path_ids, targc, targv ); *)                                                */
/*               mas_del_argv( targc, targv, 0 );                                                                                 */
/*               fprintf( stderr, "==========================\n" );                                                               */
/*               if ( path_ids )                                                                                                  */
/*                 free( path_ids );                                                                                              */
/*             }                                                                                                                  */
/*           }                                                                                                                    */
/*           sqlite3_free( sql2 );                                                                                                */
/*           sqlite3_free_table( presult2 );                                                                                      */
/*         }                                                                                                                      */
/*         (* fprintf( stderr, "[%llu] %s\n", cnt, mdline ); *)                                                                   */
/*       }                                                                                                                        */
/*     }                                                                                                                          */
/*   }                                                                                                                            */
/*   sqlite3_free_table( presult );                                                                                               */
/* }                                                                                                                              */

/* void                                                                                                                                            */
/* print_hardlinks( void )                                                                                                                         */
/* {                                                                                                                                               */
/*   int r;                                                                                                                                        */
/*   int row, column;                                                                                                                              */
/*   char **presult = NULL;                                                                                                                        */
/*                                                                                                                                                 */
/*   char *sql = "SELECT inode, COUNT(*) AS cnt FROM duf_keydata GROUP BY inode HAVING cnt>1 ORDER BY inode";                                      */
/*                                                                                                                                                 */
/*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                                                                          */
/*   if ( r == SQLITE_OK )                                                                                                                         */
/*   {                                                                                                                                             */
/*     fprintf( stderr, "-Hardlinks: [%u]--------\n", row );                                                                                       */
/*     if ( row )                                                                                                                                  */
/*       for ( int ir = column; ir <= column * row; ir += column )                                                                                 */
/*       {                                                                                                                                         */
/*         int r2;                                                                                                                                 */
/*         int row2, column2;                                                                                                                      */
/*         char **presult2 = NULL;                                                                                                                 */
/*         char *sql2;                                                                                                                             */
/*         sqlite3_int64 inode;                                                                                                                    */
/*                                                                                                                                                 */
/*         fprintf( stderr, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" );                                                              */
/*         inode = strtoll( presult[ir + 0], NULL, 10 );                                                                                           */
/*         (* duf_sql_select( duf_sql_select_test, "SELECT duf_keydata.pathid, duf_filenames.id, duf_filenames.name "                           *) */
/*         (*                 " FROM duf_keydata LEFT JOIN duf_filenames on (duf_keydata.nameid=duf_filenames.id) WHERE inode='%llu'", inode ); *) */
/*         sql2 = sqlite3_mprintf( "SELECT duf_keydata.pathid, duf_filenames.id, duf_filenames.name "                                              */
/*                                 " FROM duf_keydata LEFT JOIN duf_filenames on (duf_keydata.nameid=duf_filenames.id) WHERE inode='%llu'",        */
/*                                 inode );                                                                                                        */
/*                                                                                                                                                 */
/*         r2 = sqlite3_get_table( pDb, sql2, &presult2, &row2, &column2, &errmsg );                                                               */
/*         if ( r2 == SQLITE_OK )                                                                                                                  */
/*         {                                                                                                                                       */
/*           fprintf( stderr, "---row2:[%u]--------\n", row2 );                                                                                    */
/*           if ( row2 )                                                                                                                           */
/*             for ( int ir2 = column2; ir2 <= column2 * row2; ir2 += column2 )                                                                    */
/*             {                                                                                                                                   */
/*               sqlite3_int64 pathid;                                                                                                             */
/*               sqlite3_int64 nameid;                                                                                                             */
/*               char *fpath;                                                                                                                      */
/*                                                                                                                                                 */
/*               pathid = strtoll( presult2[ir2 + 0], NULL, 10 );                                                                                  */
/*               nameid = strtoll( presult2[ir2 + 1], NULL, 10 );                                                                                  */
/*               fpath = path_id_to_path( pathid );                                                                                                */
/*               fpath = mas_strcat_x( fpath, "/" );                                                                                               */
/*               fpath = mas_strcat_x( fpath, presult2[ir2 + 2] );                                                                                 */
/*               fprintf( stderr, "%llu : %llu : '%s'\n", inode, nameid, fpath );                                                                  */
/*               if ( fpath )                                                                                                                      */
/*                 mas_free( fpath );                                                                                                              */
/*             }                                                                                                                                   */
/*           fprintf( stderr, "-----------=-=\n" );                                                                                                */
/*         }                                                                                                                                       */
/*         else                                                                                                                                    */
/*           SQL_ERR( r2 );                                                                                                                        */
/*         sqlite3_free_table( presult2 );                                                                                                         */
/*       }                                                                                                                                         */
/*   }                                                                                                                                             */
/*   else                                                                                                                                          */
/*     SQL_ERR( r );                                                                                                                               */
/*   sqlite3_free_table( presult );                                                                                                                */
/* }                                                                                                                                               */
