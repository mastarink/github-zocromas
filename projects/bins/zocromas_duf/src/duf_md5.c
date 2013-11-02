#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <openssl/md5.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"
#include "duf_path.h"
#include "duf_file_pathid.h"
#include "duf_file_md5id.h"
#include "duf_insert.h"


#include "duf_md5.h"

static unsigned long long
duf_update_md5_file( const char *fpath, unsigned long long filedataid, size_t fsize )
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
        resmd = duf_insert_md5( ( ( unsigned long long * ) &md[0] ), fsize );
        if ( 1 )
        {
          fprintf( stderr, "resmd: %llu: %s\x1b[K\r", resmd, fpath );
        }
        if ( resmd )
        {
          duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", resmd, filedataid );
        }
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

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *         int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata );
 * */
static int
duf_sql_update_md5( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  unsigned long long pathid = -1, old_pathid = -1, nameid = -1;
  char **ppath;
  char *fname;
  unsigned long long filedataid;
  size_t fsize = 0;
  unsigned long long resmd = -1;
  ino_t inode;

  ppath = ( char ** ) sel_cb_udata;
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
      *ppath = duf_pathid_to_path( pathid );
    }
    if ( *ppath && fsize )
    {
      int rs;
      char *fpath;
      struct stat st;

      fpath = duf_join_path( *ppath, fname );
      rs = stat( fpath, &st );
      if ( 0 && now )
      {
        fprintf( stderr, "%d. [%lu;%lu;now:%lu - %lu] (%lu) %s\x1b[K\n", rs, st.st_atime, st.st_mtime, st.st_ctime, now, fsize, fpath );
      }
      if ( fsize > 0 && ( !resmd || rs || !now || now < st.st_mtime || now < st.st_ctime ) )
      {
        if ( 0 )
        {
          fprintf( stderr, "fsize>0:%d; !resmd:%d(%lld:%s); rs:%d; t1:%d; t2:%d all:%d  now:%lu; mtime:%lu %s\x1b[K\n", fsize > 0, !resmd,
                   resmd, presult[3], rs, now < st.st_mtime, now < st.st_ctime,
                   ( !resmd || rs || ( now < st.st_mtime || now < st.st_ctime ) ), now, st.st_mtime, fpath );
        }
        resmd = duf_update_md5_file( fpath, filedataid, fsize );
        if ( pathid && nameid && inode && resmd )
          duf_insert_keydata( pathid, nameid, inode, resmd );
      }
      mas_free( fpath );
    }
    old_pathid = pathid;
  }
  return 0;
}

int
duf_update_md5( void )
{
  int r = 0;
  char *path = NULL;

  fprintf( stderr, "Calc md5\x1b[K\n" );
  r = duf_sql_select( duf_sql_update_md5, &path, STR_CB_DEF, STR_CB_UDATA_DEF, 0,
                      "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id,"
                      " duf_filedatas.size, duf_filenames.id, duf_filenames.pathid, duf_filenames.name,"
                      " strftime('%%s',  duf_md5.now) as seconds, " " duf_md5.now " " FROM duf_filedatas "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) " );
  if ( path )
    mas_free( path );
  fprintf( stderr, "End calc md5\x1b[K\n" );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *       int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata );
 * */
int
duf_sql_scan_md5( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  int r = 0;

  if ( str_cb )
  {
    unsigned long long pathid;
    unsigned long long filenameid;

    char *path;
    md5_std_data_t data;

    pathid = strtoll( presult[0], NULL, 10 );
    filenameid = strtoll( presult[1], NULL, 10 );
    data.md5id = strtoll( presult[3], NULL, 10 );
    data.size = strtoll( presult[4], NULL, 10 );
    data.dupcnt = presult[5] ? strtoll( presult[5], NULL, 10 ) : 0;
    path = duf_pathid_to_path( pathid );

/* duf_str_cb_t :
 *           int fun( pathid, path, filenameid, name,       str_cb_udata ); */
    r = ( *str_cb ) ( pathid, path, filenameid, presult[2], &data );
    mas_free( path );
  }
  return r;
}

int
duf_scan_vmd5_sql( duf_str_cb_t str_cb, const char *sql, va_list args )
{
  return duf_sql_vselect( duf_sql_scan_md5, SEL_CB_UDATA_DEF, str_cb, STR_CB_UDATA_DEF, DUF_TRACE_YES, sql, args );
}

int
duf_scan_md5_sql( duf_str_cb_t str_cb, const char *sql, ... )
{
  int r = 0;
  va_list args;

  va_start( args, sql );
  r = duf_scan_vmd5_sql( str_cb, sql, args );
  va_end( args );
  return r;
}

/*
 * sql must select pathid, filenameid, filename, md5id, size
 * */
static int
duf_scan_md5id( duf_str_cb_t str_cb, unsigned long long dupcnt_min, unsigned long long limit )
{
  int r = 0;

  if ( limit )
  {
    if ( dupcnt_min )
      r = duf_scan_md5_sql( str_cb, "SELECT 0, 0, '', id, size, dupcnt FROM duf_md5 WHERE dupcnt>=%llu ORDER BY size DESC LIMIT %llu",
                            dupcnt_min, limit );
    else
      r = duf_scan_md5_sql( str_cb, "SELECT 0, 0, '', id, size, dupcnt FROM duf_md5 ORDER BY size DESC LIMIT %llu", limit );
  }
  else
  {
    if ( dupcnt_min )
      r = duf_scan_md5_sql( str_cb, "SELECT 0, 0, '', id, size, dupcnt FROM duf_md5 WHERE dupcnt>=%llu  ORDER BY size DESC", dupcnt_min );
    else
      r = duf_scan_md5_sql( str_cb, "SELECT 0, 0, '', id, size, dupcnt FROM duf_md5 ORDER BY size DESC", dupcnt_min );
  }
  return r;
}

/* 
 * duf_str_cb_t:
 *             int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata ); 
 * */
int
duf_sql_scan_print_md5( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
{
  md5_std_data_t *mdata;

  mdata = ( md5_std_data_t * ) str_cb_udata;
  printf( "-- %7llu - %lld\n", mdata->md5id, mdata->size );
  /* printf( "%s ", name ); */
  return 0;
}

int
duf_print_md5( unsigned long long limit )
{
  int r = 0;

  duf_scan_md5id( duf_sql_scan_print_md5, 0, limit );
  return r;
}

/* 
 * duf_str_cb_t:
 *                  int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata ); 
 * */
int
duf_sql_scan_print_md5_same( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                             void *str_cb_udata )
{
  md5_std_data_t *mdata;

  mdata = ( md5_std_data_t * ) str_cb_udata;
  printf( "-- size=%lld; %lld files (id:%llu) ----------\n", mdata->size, mdata->dupcnt, mdata->md5id );
  duf_print_files_by_md5id( mdata->md5id );
  return 0;
}

int
duf_print_md5_same( unsigned long long dupcnt_min, unsigned long long limit )
{
  int r = 0;

  duf_scan_md5id( duf_sql_scan_print_md5_same, dupcnt_min, limit );
  return r;
}
