#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
#include "duf_file_md5id.h"
#include "duf_insert.h"


/* ###################################################################### */
#include "duf_md5.h"
/* ###################################################################### */




static unsigned long long
duf_update_md5_file( const char *fpath, unsigned long long filedataid, size_t fsize )
{
  unsigned long long resmd = 0;
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
      /* TODO : file deleted ... */
      fprintf( stderr, "ERROR open file : %s\n", fpath );
    }
  }
  else
  {
    fprintf( stderr, "ERROR file not set\n" );
  }
  /* fprintf( stderr, "\n" ); */
  return resmd;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
static int
duf_sql_update_md5( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                    void *str_cb_udata )
{
  unsigned long long pathid = -1, old_pathid = -1, filenameid = -1;
  char **ppath;
  const char *fname;
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
    filenameid = strtoll( presult[5], NULL, 10 );
  else
  {
    fprintf( stderr, "duf_filenames record absent for duf_filedatas.id=duf_filenames.dataid=%lld\n", filedataid );
  }
  if ( presult[6] )
    pathid = strtoll( presult[6], NULL, 10 );
  else
  {
    fprintf( stderr, "%s;%s;%s;%s;%s;%s;%s;\n", presult[0], presult[1], presult[2], presult[3], presult[4], presult[5], presult[6] );
  }
  fname = presult[7];
  if ( fname )
  {
    const char *snow;
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
        if ( pathid && filenameid && inode && resmd )
          duf_insert_keydata( pathid, filenameid, inode, resmd );
      }
      mas_free( fpath );
    }
    old_pathid = pathid;
  }
  return 0;
}

int
duf_update_md5_pathid( unsigned long long pathid, int recursive /* TODO */  )
{
  int r = 0;
  char *path = NULL;

  fprintf( stderr, "%lld Calc md5\x1b[K\n", pathid );
  if ( pathid )
    r = duf_sql_select( duf_sql_update_md5, &path, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id,"
                        " duf_filedatas.size, duf_filenames.id, duf_filenames.pathid, duf_filenames.name,"
                        " strftime('%%s',  duf_md5.now) as seconds, " " duf_md5.now " " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                        " LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) " " WHERE duf_filenames.pathid='%lld'", pathid );
  else
    r = duf_sql_select( duf_sql_update_md5, &path, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
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

int
duf_update_md5_path( const char *path, int recursive )
{
  return duf_update_md5_pathid( duf_path_to_pathid( path ), recursive );
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
int
duf_sql_scan_md5( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                  void *str_cb_udata )
{
  int r = 0;

  if ( str_cb )
  {
    unsigned long long pathid;
    unsigned long long filenameid;

    char *path;
    const char *filename;
    md5_std_data_t data;

    pathid = strtoll( presult[0], NULL, 10 );
    filenameid = strtoll( presult[1], NULL, 10 );
    filename = presult[2];
    data.md5id = strtoll( presult[3], NULL, 10 );
    data.size = strtoll( presult[4], NULL, 10 );
    data.dupcnt = presult[5] ? strtoll( presult[5], NULL, 10 ) : 0;
    path = duf_pathid_to_path( pathid );

/* duf_str_cb_t :
 *           int fun( pathid, path, filenameid, filename, stat,         str_cb_udata, presult ); */
    r = ( *str_cb ) ( pathid, path, filenameid, filename, DUF_STAT_DEF, &data, presult );
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
      r = duf_scan_md5_sql( str_cb, "SELECT 0, 0, '', id, size, dupcnt FROM duf_md5 " " WHERE dupcnt>=%llu ORDER BY size DESC LIMIT %llu",
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
 * */
int
duf_sql_scan_print_md5( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, const struct stat *st,
                        void *str_cb_udata, const char *const *presult )
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

  duf_scan_md5id( duf_sql_scan_print_md5, 0 /* dupcnt_min */ , limit );
  return r;
}

/* 
 * duf_str_cb_t:
 * */
int
duf_sql_scan_print_md5_same( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                             const struct stat *st, void *str_cb_udata, const char *const *presult )
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

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
static int
duf_sql_update_mdpaths_path( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                             void *str_cb_udata )
{
  MD5_CTX *pctx;
  unsigned long long md5s1, md5s2, pathid;

  pathid = va_arg( args, unsigned long long );

  fprintf( stderr, "(duf_sql_update_mdpaths_path) parthid:%lld / %s\n", pathid, presult[0] );
  pctx = ( MD5_CTX * ) sel_cb_udata;
  if ( presult[5] && presult[6] )
  {
    md5s1 = strtoll( presult[5], NULL, 10 );
    md5s2 = strtoll( presult[6], NULL, 10 );
    MD5_Update( pctx, &md5s1, sizeof( md5s1 ) );
    MD5_Update( pctx, &md5s2, sizeof( md5s2 ) );
  }

  return 0;
}

int
duf_update_mdpaths_pathid( unsigned long long pathid )
{
  int r = 0;
  MD5_CTX ctx;
  unsigned long long *md64;

  MD5_Init( &ctx );
  r = duf_sql_select( duf_sql_update_mdpaths_path, &ctx, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                      "SELECT duf_keydata.pathid, duf_keydata.nameid, duf_filenames.name, '', '', md5sum1, md5sum2 " " FROM duf_md5 "
                      " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "
                      " LEFT JOIN duf_filenames ON (duf_keydata.nameid=duf_filenames.id)"
                      " WHERE duf_keydata.pathid='%llu' " " ORDER by md5sum1, md5sum2 ", ( unsigned long long ) pathid );

  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    MD5_Final( md, &ctx );
    md64 = ( unsigned long long * ) md;
  }
  fprintf( stderr, "UPDATE duf_paths SET md5dir1='%lld', md5dir2='%lld' WHERE id='%llu'\n", md64[1], md64[0], pathid );
  duf_sql( "UPDATE duf_paths SET md5dir1='%lld', md5dir2='%lld' WHERE id='%lu'", md64[1], md64[0], pathid );

  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * */
static int
duf_sql_update_mdpaths( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                        void *str_cb_udata )
{
  unsigned long long pathid;

  pathid = strtol( presult[0], NULL, 10 );
  fprintf( stderr, "duf_sql_update_mdpaths %d\n", nrow );
  duf_update_mdpaths_pathid( pathid );
  return 0;
}

/* Sampe same(?) dirs                                                                                         */
/* 1439: /mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007                         */
/* 1676: /mnt/new_media/media/photo/Pictures.R.20120207.164339/Photo/unsorted/kodak/Kodak Pictures/08-14-2007 */
int
duf_update_mdpaths( unsigned long long pathid )
{
  int r;

  fprintf( stderr, "Start duf_update_mdpaths\n" );
  if ( pathid )
    r = duf_sql_select( duf_sql_update_mdpaths, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                        "SELECT id, dirname FROM duf_paths " " WHERE id='%lld' " " ORDER BY id", pathid );
  else
    r = duf_sql_select( duf_sql_update_mdpaths, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                        "SELECT id, dirname FROM duf_paths " " ORDER BY id" );
  fprintf( stderr, "End duf_update_mdpaths\n" );
  return r;
}
