#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/md5.h>

/* #include <sqlite3.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include "duf_def.h" */
#include "duf_sql.h"
#include "duf_utils.h"
#include "duf_insert.h"


#include "duf_md5.h"

static unsigned long long
update_md5_file( const char *fpath, unsigned long long filedataid, size_t fsize )
{
  unsigned long long resmd = -1;
  FILE *f;

  if ( fpath )
  {
    f = fopen( fpath, "r" );
    if ( f )
    {
      MD5_CTX ctx;
      unsigned char md[MD5_DIGEST_LENGTH];
      char *buffer;
      size_t bufsz = 1024 * 1024 * 10;

      memset( &md, 0, sizeof( md ) );
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
        resmd = insert_md5( ( ( unsigned long long * ) &md[0] ), fsize );
        fprintf( stderr, "resmd: %llu: %s\x1b[K\n", resmd, fpath );
        if ( resmd )
        {
          duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", resmd, filedataid );
        }
        fprintf( stderr, "F\n" );
      }
    }
    else
    {
      fprintf( stderr, "ERROR open file : %s\n", fpath );
    }
  }
  else
  {
    fprintf( stderr, "ERROR file not set\n" );
    exit( 11 );
  }
  /* fprintf( stderr, "\n" ); */
  return resmd;
}

static int
duf_sql_update_md5( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fun )
{
  unsigned long long pathid = -1, old_pathid = -1, nameid = -1;
  char **ppath;
  char *fname;
  unsigned long long filedataid;
  size_t fsize = 0;
  unsigned long long resmd = -1;
  ino_t inode;

  ppath = ( char ** ) udata;
  /* fprintf(stderr, "@@@@@@@@@@@ %u [[%s]] [%s]\x1b[K\n", column, presult[ 7], presult[ 8]); */
  filedataid = strtoll( presult[0], NULL, 10 );
  inode = strtoll( presult[2], NULL, 10 );
  if ( presult[3] )
    resmd = strtoll( presult[3], NULL, 10 );
  if ( presult[4] )
    fsize = strtoll( presult[4], NULL, 10 );
  if ( presult[5] )
    nameid = strtoll( presult[5], NULL, 10 );
  else
  {
    fprintf( stderr, "duf_filenames record absent for duf_filedatas.id=duf_filenames.dataid=%lld\n", filedataid );
    exit( 11 );
  }
  if ( presult[6] )
    pathid = strtoll( presult[6], NULL, 10 );
  else
  {
    fprintf( stderr, "%s;%s;%s;%s;%s;%s;%s;\n", presult[0], presult[1], presult[2], presult[3], presult[4], presult[5], presult[6] );
    exit( 11 );
  }
  fname = presult[7];
  if ( fname )
  {
    char *snow;
    double dnow = 0;
    time_t now = 0;

    snow = presult[8];
    if ( snow )
    {
      sscanf( snow, "%lg", &dnow );
      now = ( time_t ) dnow;
    }
    if ( old_pathid != pathid || !*ppath )
    {
      if ( *ppath )
        mas_free( *ppath );
      *ppath = path_id_to_path( pathid );
    }
    if ( *ppath && fsize )
    {
      int rs;
      char *fpath;
      struct stat st;

      fpath = join_path( *ppath, fname );
      rs = stat( fpath, &st );
      if ( 0 && now )
      {

        fprintf( stderr, "%d. [%lu;%lu;now:%lu - %lu] (%lu) %s\x1b[K\n", rs, st.st_atime, st.st_mtime, st.st_ctime, now, fsize, fpath );
      }
      if ( fsize > 0 && ( !resmd || rs || !now || now < st.st_mtime || now < st.st_ctime ) )
      {
        if ( 1 )
        {
          fprintf( stderr, "fsize>0:%d; !resmd:%d(%lld:%s); rs:%d; t1:%d; t2:%d all:%d  now:%lu; mtime:%lu %s\x1b[K\n", fsize > 0, !resmd,
                   resmd, presult[3], rs, now < st.st_mtime, now < st.st_ctime,
                   ( !resmd || rs || ( now < st.st_mtime || now < st.st_ctime ) ), now, st.st_mtime, fpath );
        }
        resmd = update_md5_file( fpath, filedataid, fsize );
        if ( pathid && nameid && inode && resmd )
          insert_keydata( pathid, nameid, inode, resmd );
      }
      mas_free( fpath );
    }
    old_pathid = pathid;
  }
  return 0;
}

int
update_md5( void )
{
  int r = 0;

  /* char *sql; */
  /* int row, column; */
  /* char **presult = NULL; */
  char *path = NULL;

  fprintf( stderr, "Calc md5\x1b[K\n" );
  r = duf_sql_select( duf_sql_update_md5, &path, NULL, 0,
                      "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id,"
                      " duf_filedatas.size, duf_filenames.id, duf_filenames.pathid, duf_filenames.name,"
                      " strftime('%%s',  duf_md5.now) as seconds, " " duf_md5.now " " FROM duf_filedatas "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) " );
  /* {                                                                                                                                          */
  /*   sql = sqlite3_mprintf                                                                                                                    */
  /*         ( "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id,"                                          */
  /*           " duf_filedatas.size, duf_filenames.id, duf_filenames.pathid, duf_filenames.name,"                                               */
  /*           " strftime('%%s',  duf_md5.now) as seconds, "                                                                                    */
  /*           " duf_md5.now "                                                                                                                  */
  /*           " FROM duf_filedatas "                                                                                                           */
  /*           " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "                                                           */
  /*           " LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) " );                                                                     */
  /*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                                                                     */
  /*   (* fprintf( stderr, "%ux%u : %s\n", row, column, sql ); *)                                                                               */
  /*   (* sleep( 3 ); *)                                                                                                                        */
  /*   if ( r == SQLITE_OK )                                                                                                                    */
  /*   {                                                                                                                                        */
  /*     if ( row > 0 )                                                                                                                         */
  /*     {                                                                                                                                      */
  /*       char *path = NULL;                                                                                                                   */
  /*       sqlite3_int64 pathid = -1, old_pathid = -1, nameid = -1;                                                                             */
  /*                                                                                                                                            */
  /*       for ( int ir = column; ir <= column * row; ir += column )                                                                            */
  /*       {                                                                                                                                    */
  /*         char *fname;                                                                                                                       */
  /*         sqlite3_int64 filedataid;                                                                                                          */
  /*         size_t fsize = 0;                                                                                                                  */
  /*         sqlite3_int64 resmd = -1;                                                                                                          */
  /*         ino_t inode;                                                                                                                       */
  /*                                                                                                                                            */
  /*         (* fprintf(stderr, "@@@@@@@@@@@ %u [[%s]] [%s]\x1b[K\n", column, presult[ir + 7], presult[ir + 8]); *)                             */
  /*                                                                                                                                            */
  /*         filedataid = strtoll( presult[ir + 0], NULL, 10 );                                                                                 */
  /*         inode = strtoll( presult[ir + 2], NULL, 10 );                                                                                      */
  /*         if ( presult[ir + 3] )                                                                                                             */
  /*           resmd = strtoll( presult[ir + 3], NULL, 10 );                                                                                    */
  /*         if ( presult[ir + 4] )                                                                                                             */
  /*           fsize = strtoll( presult[ir + 4], NULL, 10 );                                                                                    */
  /*         if ( presult[ir + 5] )                                                                                                             */
  /*           nameid = strtoll( presult[ir + 5], NULL, 10 );                                                                                   */
  /*         else                                                                                                                               */
  /*         {                                                                                                                                  */
  /*           fprintf( stderr, "duf_filenames record absent for duf_filedatas.id=duf_filenames.dataid=%lld\n%s\n", filedataid, sql );          */
  /*           exit( 11 );                                                                                                                      */
  /*         }                                                                                                                                  */
  /*         if ( presult[ir + 6] )                                                                                                             */
  /*           pathid = strtoll( presult[ir + 6], NULL, 10 );                                                                                   */
  /*         else                                                                                                                               */
  /*         {                                                                                                                                  */
  /*           fprintf( stderr, "%s;%s;%s;%s;%s;%s;%s;\n", presult[ir + 0], presult[ir + 1], presult[ir + 2], presult[ir + 3], presult[ir + 4], */
  /*                    presult[ir + 5], presult[ir + 6] );                                                                                     */
  /*           exit( 11 );                                                                                                                      */
  /*         }                                                                                                                                  */
  /*         fname = presult[ir + 7];                                                                                                           */
  /*         if ( fname )                                                                                                                       */
  /*         {                                                                                                                                  */
  /*           char *snow;                                                                                                                      */
  /*           double dnow = 0;                                                                                                                 */
  /*           time_t now = 0;                                                                                                                  */
  /*                                                                                                                                            */
  /*           snow = presult[ir + 8];                                                                                                          */
  /*           if ( snow )                                                                                                                      */
  /*           {                                                                                                                                */
  /*             sscanf( snow, "%lg", &dnow );                                                                                                  */
  /*             now = ( time_t ) dnow;                                                                                                         */
  /*           }                                                                                                                                */
  /*           if ( old_pathid != pathid || !path )                                                                                             */
  /*           {                                                                                                                                */
  /*             if ( path )                                                                                                                    */
  /*               mas_free( path );                                                                                                            */
  /*             path = path_id_to_path( pathid );                                                                                              */
  /*           }                                                                                                                                */
  /*           if ( path && fsize )                                                                                                             */
  /*           {                                                                                                                                */
  /*             int rs;                                                                                                                        */
  /*             char *fpath;                                                                                                                   */
  /*             struct stat st;                                                                                                                */
  /*                                                                                                                                            */
  /*             fpath = join_path( path, fname );                                                                                              */
  /*             rs = stat( fpath, &st );                                                                                                       */
  /*             if ( 0 && now )                                                                                                                */
  /*             {                                                                                                                              */
  /*                                                                                                                                            */
  /*               fprintf( stderr, "%d. [%lu:%lu:%lu - %lu] (%lu) %s\x1b[K\n", rs, st.st_atime, st.st_mtime, st.st_ctime, now, fsize, fpath ); */
  /*             }                                                                                                                              */
  /*             if ( fsize > 0 && ( !resmd || rs || !now || now <= st.st_mtime || now <= st.st_ctime ) )                                       */
  /*             {                                                                                                                              */
  /*               if ( 1 )                                                                                                                     */
  /*               {                                                                                                                            */
  /*                 fprintf( stderr, "fsize>0:%d; !resmd:%d(%lld:%s); rs:%d; t1:%d; t2:%d all:%d  %s/%lu:%lu %s\x1b[K\n", fsize > 0, !resmd,   */
  /*                          resmd, presult[ir + 3], rs, now <= st.st_mtime, now <= st.st_ctime,                                               */
  /*                          ( !resmd || rs || ( now <= st.st_mtime || now <= st.st_ctime ) ), snow, now, st.st_mtime, fpath );                */
  /*               }                                                                                                                            */
  /*               resmd = update_md5_file( fpath, filedataid, fsize );                                                                         */
  /*               if ( pathid && nameid && inode && resmd )                                                                                    */
  /*                 insert_keydata( pathid, nameid, inode, resmd );                                                                            */
  /*             }                                                                                                                              */
  /*             mas_free( fpath );                                                                                                             */
  /*           }                                                                                                                                */
  /*           old_pathid = pathid;                                                                                                             */
  /*         }                                                                                                                                  */
  /*       }                                                                                                                                    */
  /*       if ( path )                                                                                                                          */
  /*         mas_free( path );                                                                                                                  */
  /*     }                                                                                                                                      */
  /*   }                                                                                                                                        */
  /*   else                                                                                                                                     */
  /*     SQL_ERR( r );                                                                                                                          */
  /*   sqlite3_free_table( presult );                                                                                                           */
  /*   sqlite3_free( sql );                                                                                                                     */
  /* }                                                                                                                                          */
  if ( path )
    mas_free( path );
  fprintf( stderr, "End calc md5\x1b[K\n" );
  return r;
}
