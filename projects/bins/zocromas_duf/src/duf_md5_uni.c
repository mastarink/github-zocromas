#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_config.h"

#include "duf_file.h"
#include "duf_path.h"

#include "duf_sql.h"
#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_md5_uni.h"
/* ###################################################################### */

static int
duf_sql_insert_md5_uni( duf_record_t * precord, va_list args, void *sel_cb_udata,
                        duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  unsigned long long *presmd;
  unsigned long long md5id = duf_sql_ull_by_name( "md5id", precord, 0 );

  presmd = ( unsigned long long * ) sel_cb_udata;
  if ( presmd )
    *presmd = md5id;
  duf_sql( "UPDATE duf_md5 SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", *presmd );
  return 0;
}

static unsigned long long
duf_insert_md5_uni( unsigned long long *md64, size_t fsize )
{
  unsigned long long resmd = -1;
  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_md5 (md5sum1,md5sum2,size,ucnt,now) values ('%lld','%lld','%llu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, md64[1], md64[0], ( unsigned long long ) fsize );
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_md5_uni, &resmd, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT id as md5id FROM duf_md5 WHERE md5sum1='%lld' and md5sum2='%lld'", md64[1], md64[0] );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resmd = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5_uni %d\n", r );

  return resmd;
}

static unsigned long long
duf_upd_md5_f_uni( const char *fpath, unsigned long long filedataid )
{
  unsigned long long resmd = 0;
  FILE *f;

  if ( fpath )
  {
    int r = 0;
    struct stat st;

    r = stat( fpath, &st );
    if ( !r )
    {
      f = fopen( fpath, "r" );
      if ( f )
      {
        MD5_CTX ctx;
        unsigned char md[MD5_DIGEST_LENGTH];
        unsigned char mdr[MD5_DIGEST_LENGTH];
        char *buffer;
        size_t bufsz = 1024 * 1024 * 10;

        if ( duf_config->cli.trace.md5 )
          fprintf( stderr, "[MD5  ] %20s: opened fpath:'%s'\n", __func__, fpath );
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
        for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
        {
          mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];
        }

        {
          resmd = duf_insert_md5_uni( ( ( unsigned long long * ) &mdr[0] ), st.st_size );
          /* if ( 1 )                                                      */
          /* {                                                             */
          /*   fprintf( stderr, "resmd: %llu: %s\x1b[K\r", resmd, fpath ); */
          /* }                                                             */
          if ( duf_config->cli.trace.md5 )
          {
            fprintf( stderr, "[MD5  ] %20s: ", __func__ );
            /* for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ ) */
            /* {                                                          */
            /*   fprintf( stderr, "%02x", md[i] );                        */
            /* }                                                          */
            fprintf( stderr, "\n" );
            fprintf( stderr, "[MD5  ] %20s: resmd: %llu: %s\n", __func__, resmd, fpath );
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


/* callback of type duf_scan_callback_file_t */
static int
duf_md5_file_uni( unsigned long long pathid, unsigned long long filenameid,
                  const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */


/* run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file */


  /* 
   * --uni-scan   -R   --md5   --files   -FF
   *                   ^^^^^   ^^^^^^^
   * */
  {
    char *fpath = filenameid_to_filepath( filenameid );
    unsigned long long filedataid = duf_sql_ull_by_name( "filedataid", precord, 0 );
    unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 );

    if ( duf_config->cli.trace.md5 )
      fprintf( stderr, "[MD5  ] %20s: filedataid:%lld; filesize:%lld; fpath:'%s'\n", __func__, filedataid, filesize, fpath );

    duf_upd_md5_f_uni( fpath, filedataid );
    mas_free( fpath );
  }
  if ( duf_config->cli.trace.md5 > 1 )
    fprintf( stderr, "[MD5  ] %20s: %s\n", __func__, name );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_md5_dir_uni( unsigned long long pathid, const char *name, unsigned long long items, duf_dirinfo_t * pdi,
                 struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( duf_config->cli.trace.md5 > 1 )
    fprintf( stderr, "[MD5  ] %20s: pathid=%llu\n", __func__, pathid );
  {
    char *path = duf_pathid_to_path( pathid );

    if ( duf_config->cli.trace.md5 > 1 )
      fprintf( stderr, "[MD5  ] %20s: path=%s\n", __func__, path );

    mas_free( path );
  }
  return 0;
}
/* currently used for --same-md5  ??? */

duf_scan_callbacks_t duf_md5_callbacks = {
  .fieldset = "duf_filenames.pathid, duf_filenames.id as filenameid, duf_filedatas.id as filedataid, duf_filedatas.size as filesize, duf_filenames.name as filename",
  .init_scan = NULL,
  .directory_scan = duf_md5_dir_uni,
  .file_scan = duf_md5_file_uni,
};
