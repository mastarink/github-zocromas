#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_levinfo.h"

#include "duf_file.h"
#include "duf_path.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_md5_uni.h" */
/* ###################################################################### */

static int
duf_sel_cb_insert_md5_uni( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb,
                           void *str_cb_udata_notused, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  int r = 0;
  unsigned long long *presmd;

  DUF_UFIELD( md5id );

  presmd = ( unsigned long long * ) sel_cb_udata;
  if ( presmd )
    *presmd = md5id;
  r = duf_sql( "UPDATE duf_md5 SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", ( int * ) NULL, *presmd );
  return r;
}

static unsigned long long
duf_insert_md5_uni( unsigned long long *md64, size_t fsize )
{
  unsigned long long resmd = -1;
  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_md5 (md5sum1,md5sum2,size,ucnt,now) values ('%lld','%lld','%llu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, md64[1], md64[0], ( unsigned long long ) fsize );
  if ( r == DUF_SQL_CONSTRAINT )
  {
    r = duf_sql_select( duf_sel_cb_insert_md5_uni, &resmd, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT id as md5id FROM duf_md5 WHERE md5sum1='%lld' and md5sum2='%lld'", md64[1], md64[0] );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resmd = duf_sql_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5_uni %d\n", r );

  return resmd;
}

static unsigned long long
duf_upd_md5_fpath_uni( const char *fpath, unsigned long long filedataid )
{
  unsigned long long resmd = 0;

  if ( fpath )
  {
    int r = 0;
    struct stat st;

    r = stat( fpath, &st );
    if ( !r )
    {
      FILE *f;

      f = fopen( fpath, "r" );
      if ( f )
      {
        MD5_CTX ctx;
        unsigned char md[MD5_DIGEST_LENGTH];
        unsigned char mdr[MD5_DIGEST_LENGTH];
        unsigned long long *pmd;
        char *buffer;
        size_t bufsz = 1024 * 1024 * 10;

        DUF_TRACE( md5, 0, "opened fpath:'%s'", fpath );
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
        pmd = ( unsigned long long * ) &mdr;
        {
          resmd = duf_insert_md5_uni( ( ( unsigned long long * ) &mdr[0] ), st.st_size );
          DUF_TRACE( md5, 0, "%016llx%016llx : resmd: %llu: %s", pmd[1], pmd[0], resmd, fpath );
          if ( resmd )
          {
            duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", ( int * ) NULL, resmd, filedataid );
          }
        }
      }
      else
      {
        /* TODO : file deleted ... */
        DUF_ERRSYS( "open to read file : %s", fpath );
      }
    }
    else
    {
      /* TODO : file deleted ... */
      DUF_ERRSYS( "stat file : %s", fpath );
    }
  }
  else
  {
    DUF_ERRSYS( "file not set" );
  }
  return resmd;
}

static unsigned long long
duf_upd_md5_pdh_uni( int dfd, const char *fname, unsigned long long filedataid )
{
  unsigned long long resmd = 0;

  int r = 0;
  struct stat st_file;

  r = fstatat( dfd, fname, &st_file, 0 );
  DUF_TRACE( md5, 0, "fstatat:%d", r );
  DUF_TEST_R( r );
  if ( r < 0 )
  {
    char *fpath = NULL;

    /* filenameid_to_filepath( filenameid, pdi, &r ); */

    /* DUF_TRACE( md5, 0, "filedataid:%lld; filesize:%lld; fpath:'%s'", filedataid, filesize, fpath ); */
    /* TODO : file deleted ... */
    DUF_ERRSYS( "stat file %s - %s", fpath ? fpath : "..", fname );
    /* mas_free( fpath ); */
  }
  DUF_TEST_R( r );
  if ( !r )
  {
    int fd;

    fd = openat( dfd, fname, O_NOFOLLOW | O_RDONLY );
    if ( fd >= 0 )
    {
      FILE *f;

      DUF_TRACE( md5, 0, "openat:%d", fd );
      f = fdopen( fd, "r" );

      if ( f )
      {
        MD5_CTX ctx;
        unsigned char md[MD5_DIGEST_LENGTH];
        unsigned char mdr[MD5_DIGEST_LENGTH];
        unsigned long long *pmd;
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
        for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
        {
          mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];
        }
        pmd = ( unsigned long long * ) &mdr;
        {
          resmd = duf_insert_md5_uni( ( ( unsigned long long * ) &mdr[0] ), st_file.st_size );
          DUF_TRACE( md5, 0, "%016llx%016llx : resmd: %llu: %s", pmd[1], pmd[0], resmd, fname );
          if ( resmd )
          {
            duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", ( int * ) NULL, resmd, filedataid );
          }
        }
      }
      else
      {
        /* TODO : file deleted ... */
        DUF_ERRSYS( "fdopen to read file : %s", fname );
      }
      close( fd );
    }
    else
    {
      DUF_ERRSYS( "open to read file : %s", fname );
      r = fd;
    }

  }
  DUF_TEST_R( r );
  /* else                            */
  /* {                               */
  /*   DUF_ERRSYS( "file not set" ); */
  /* }                               */
  return resmd;
}

static unsigned long long
duf_insert_keydata_uni( unsigned long long pathid, unsigned long long filenameid, ino_t inode, unsigned long long resmd, int *pr )
{
  int r = 0;
  unsigned long long resp = 0;

  r = duf_sql_c( "INSERT INTO duf_keydata (pathid, filenameid, md5id, ucnt, now) " " VALUES ('%llu', '%llu', '%llu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, pathid, filenameid, resmd );
  if ( r == DUF_SQL_CONSTRAINT )
    r = 0;
  else if ( !r /* assume SQLITE_OK */  )
    resp = duf_sql_last_insert_rowid(  );
  else
    fprintf( stderr, "ERROR %s %d\n", __func__, r );
  if ( pr )
    *pr = r;
  return resp;
}

/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_fill_md5_uni( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
{
  int r = 0;
  int udfd = duf_levinfo_udfd( pdi );

  DUF_SFIELD( filename );
  DUF_TRACE( md5, 0, "start" );
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

/* run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file */


  /* 
   * --uni-scan   -R   --md5   --files   -FF
   *                   ^^^^^   ^^^^^^^
   * */
  {
    DUF_UFIELD( filenameid );
    DUF_UFIELD( filedataid );
    DUF_UFIELD( filesize );
    DUF_UFIELD( dirid );
    DUF_UFIELD( inode );
    {
      unsigned long long resmd;

      DUF_TRACE( md5, 0, "udfd:%d", udfd );
      if ( udfd )
      {
        resmd = duf_upd_md5_pdh_uni( udfd, filename, filedataid );
      }
      else
      {
        char *fpath = filenameid_to_filepath( filenameid, pdi, &r );

        DUF_TRACE( md5, 0, "filedataid:%lld; filesize:%lld; fpath:'%s'", filedataid, filesize, fpath );
        resmd = duf_upd_md5_fpath_uni( fpath, filedataid );
        mas_free( fpath );
      }
      if ( dirid && filenameid && inode && resmd )
        duf_insert_keydata_uni( dirid, filenameid, inode, resmd, &r );
      DUF_TRACE( md5, 0, "md5id:%llu", resmd );
    }
  }
  DUF_TRACE( md5, 1, "filename:'%s'", filename );
  if ( duf_config->cli.trace.md5 > 1 )
    printf( "[MD5  ] %20s: %s\n", __func__, filename );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_fill_md5_uni( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                 duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( md5, 1, "pathid=%llu", pathid );
  if ( duf_config->cli.trace.md5 > 1 )
    printf( "[MD5  ] %20s: pathid=%llu\n", __func__, pathid );
  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    DUF_TRACE( md5, 1, "path=%s", path );
    if ( duf_config->cli.trace.md5 > 1 )
      printf( "[MD5  ] %20s: path=%s\n", __func__, path );

    mas_free( path );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* currently used for --same-md5  ??? */
char *aa[] = { "a", "b", "c" };

static char *final_sql[] = {
  "UPDATE duf_md5 set dupcnt=(SELECT count(*) " " FROM duf_filedatas as fd " " JOIN duf_md5 as md ON (fd.md5id=md.id) "
        "              WHERE duf_md5.md5sum1=md.md5sum1 and duf_md5.md5sum2=md.md5sum2)",
  NULL,
};


duf_scan_callbacks_t duf_fill_md5_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_fill_md5_uni,
  .file_scan = duf_file_scan_fill_md5_uni,
  .fieldset =
        " duf_filenames.pathid as dirid "
        " , duf_filedatas.id as filedataid, duf_filedatas.inode as inode "
        " , duf_filenames.name as filename, duf_filedatas.size as filesize "
        " , uid, gid, nlink, inode, mtim as mtime " " , dupcnt as nsame "
        " , duf_filenames.id as filenameid " " , duf_filedatas.mode as filemode, md.md5sum1, md.md5sum2 ",
  .file_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 as md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.pathid='%llu' ",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames as sfn "                                                       */
        /* "          JOIN duf_filedatas as sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 as smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) as nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
