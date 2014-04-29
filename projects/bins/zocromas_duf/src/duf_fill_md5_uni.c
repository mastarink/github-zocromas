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

static unsigned long long
duf_insert_md5_uni( unsigned long long *md64, size_t fsize, int need_id, int *pr )
{
  unsigned long long md5id = -1;
  int r = 0;
  int changes = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( md64 && md64[1] && md64[0] )
  {
    r = duf_sql( "INSERT OR IGNORE INTO duf_md5 (md5sum1,md5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "md5id" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &md5id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb, ( const duf_dirhandle_t * ) NULL,
                            "SELECT id AS md5id FROM duf_md5 WHERE md5sum1='%lld' AND md5sum2='%lld'", md64[1], md64[0] );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        md5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_ERROR( "insert md5 %d", r );
    }
  }
  else
  {
    DUF_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
    DUF_TEST_R( r );
  }

  if ( pr )
    *pr = r;

  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, md5id );
  return md5id;
}

static unsigned long long
duf_upd_md5_fpath_uni( const char *fpath, unsigned long long filedataid, int *pr )
{
  int r = 0;
  unsigned long long md5id = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( fpath )
  {
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
        char *buffer;
        size_t bufsz = 1024 * 1024 * 10;

        DUF_TRACE( md5, 0, "opened fpath:'%s'", fpath );
        memset( &ctx, 0, sizeof( ctx ) );
        memset( &md, 0, sizeof( md ) );
        buffer = mas_malloc( bufsz );
        if ( MD5_Init( &ctx ) != 1 )
          r = DUF_ERROR_MD5;
        DUF_TEST_R( r );
        while ( r >= 0 && !feof( f ) )
        {
          size_t rr;

          rr = fread( buffer, 1, bufsz, f );
          DUF_ERROR( "fread:%lu", rr );
          if ( ferror( f ) )
          {
            r = DUF_ERROR_READ;
            break;
          }
          if ( MD5_Update( &ctx, buffer, rr ) != 1 )
          {
            r = DUF_ERROR_MD5;
            break;
          }
        }
        DUF_TEST_R( r );
        fclose( f );
        mas_free( buffer );
        if ( MD5_Final( md, &ctx ) != 1 )
        {
          r = DUF_ERROR_MD5;
        }
        DUF_TEST_R( r );
        if ( r >= 0 )
          for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
          {
            mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];
          }
        if ( r >= 0 )
        {
          unsigned long long *pmd;

          pmd = ( unsigned long long * ) &mdr;
          md5id = duf_insert_md5_uni( ( ( unsigned long long * ) &mdr[0] ), st.st_size, 1 /*need_id */ , &r );
          DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu: %s", pmd[1], pmd[0], md5id, fpath );
          DUF_TEST_R( r );
          if ( r >= 0 && md5id )
          {
            duf_sql( "UPDATE duf_filedatas SET md5id='%llu' WHERE id='%lld'", ( int * ) NULL, md5id, filedataid );
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
      r = DUF_ERROR_STAT;
    }
  }
  else
  {
    DUF_ERRSYS( "file not set" );
  }
  if ( pr )
    *pr = r;

  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, md5id );
  return md5id;
}

static unsigned long long
duf_upd_md5_pdh_uni( int dfd, const char *fname, unsigned long long filedataid )
{
  unsigned long long md5id = 0;

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
      /* FILE *f; */
      unsigned char mdr[MD5_DIGEST_LENGTH];

      {
        DUF_TRACE( md5, 0, "openat:%d", fd );
        /* f = fdopen( fd, "r" ); */

        /* if ( f ) */
        {
          MD5_CTX ctx;
          unsigned char md[MD5_DIGEST_LENGTH];
          size_t bufsz = 1024 * 1024 * 10;

          memset( &ctx, 0, sizeof( ctx ) );
          memset( &md, 0, sizeof( md ) );
          {
            char *buffer;

            buffer = mas_malloc( bufsz );
            if ( buffer )
            {
              if ( MD5_Init( &ctx ) != 1 )
                r = DUF_ERROR_MD5;
              DUF_TEST_R( r );
              if ( r >= 0 )
              {
                /* while ( r >= 0 && !feof( f ) ) */
                while ( r >= 0 )
                {
                  size_t rr;

                  /* rr = fread( buffer, 1, bufsz, f ); */

                  rr = read( fd, buffer, bufsz );
                  /* if ( ferror( f ) ) */
                  if ( rr < 0 )
                  {
                    r = DUF_ERROR_READ;
                    DUF_TEST_R( r );
                    break;
                  }
                  if ( rr > 0 )
                  {
                    if ( MD5_Update( &ctx, buffer, rr ) != 1 )
                      r = DUF_ERROR_MD5;
                  }
                  else
                    break;
                  DUF_TEST_R( r );
                }
              }
              mas_free( buffer );
            }
            else
            {
              r = DUF_ERROR_MEMORY;
            }
          }
          if ( MD5_Final( md, &ctx ) != 1 )
            r = DUF_ERROR_MD5;
          DUF_TEST_R( r );
          /* reverse */
          for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
          {
            mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];
          }
        }
        {
          unsigned long long *pmd;

          pmd = ( unsigned long long * ) &mdr;
          if ( r >= 0 )
          {
            md5id = duf_insert_md5_uni( ( ( unsigned long long * ) &mdr[0] ), st_file.st_size, 1 /*need_id */ , &r );
            DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu: %s", pmd[1], pmd[0], md5id, fname );
            if ( r >= 0 && md5id )
            {
              r = duf_sql( "UPDATE duf_filedatas SET md5id='%llu' WHERE id='%lld'", ( int * ) NULL, md5id, filedataid );
              DUF_TEST_R( r );
            }
          }
        }
        /* fclose( f ); */
        /* TODO : file deleted ... */
      }
      /* else                                               */
      /* {                                                  */
      /*   DUF_ERRSYS( "fdopen to read file : %s", fname ); */
      /*   r = DUF_ERROR_OPEN;                              */
      /*   DUF_TEST_R( r );                                 */
      /* }                                                  */
      close( fd );
    }
    else
    {
      DUF_ERRSYS( "open to read file : %s", fname );
      r = fd;
      r = DUF_ERROR_OPEN;
    }
    DUF_TEST_R( r );
  }
  DUF_TEST_R( r );
  /* else                            */
  /* {                               */
  /*   DUF_ERRSYS( "file not set" ); */
  /* }                               */
  return md5id;
}

/* callback of type duf_scan_callback_file_t */
static int
fill_md5_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
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
    /* DUF_UFIELD( dirid ); */
    /* DUF_UFIELD( inode ); */
    {
      unsigned long long md5id = 0;

      DUF_TRACE( md5, 0, "udfd:%d", udfd );
      if ( udfd )
      {
        md5id = duf_upd_md5_pdh_uni( udfd, filename, filedataid );
      }
      else
      {
        char *fpath = filenameid_to_filepath( filenameid, pdi, &r );

        DUF_TRACE( md5, 0, "filedataid:%lld; filesize:%lld; fpath:'%s'", filedataid, filesize, fpath );
        md5id = duf_upd_md5_fpath_uni( fpath, filedataid, &r );
        mas_free( fpath );
      }
      /* if ( dirid && filenameid && inode && md5id )                     */
      /*   duf_insert_keydata_uni( dirid, filenameid, inode, md5id, &r ); */
      DUF_TRACE( md5, 0, "md5id:%llu", md5id );
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
fill_md5_scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
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




 /* *INDENT-OFF*  */
static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) "
        " FROM duf_filedatas AS fd "
	  " JOIN duf_md5 AS md ON (fd.md5id=md.id) "
        " WHERE duf_md5.md5sum1=md.md5sum1 AND duf_md5.md5sum2=md.md5sum2)",
  /* "DELETE FROM duf_pathtot_files", */
  "INSERT OR IGNORE INTO duf_pathtot_dirs (pathid, numdirs) "
        " SELECT p.id AS pathid, COUNT(*) AS numdirs "
	  " FROM duf_paths AS p "
            " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) "
	" GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET "
      " numdirs=(SELECT COUNT(*) AS numdirs "
                  " FROM duf_paths AS p "
                      " WHERE p.parentid=duf_pathtot_dirs.pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, pathid) "
      "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS pathid "
	  " FROM duf_filenames AS fn "
	    " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)"
	      " JOIN duf_paths AS p ON (fn.pathid=p.id)"
	        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

 /* *INDENT-ON*  */

NULL,};



duf_scan_callbacks_t duf_collect_md5_callbacks = {
  .title = __FILE__,
  .opendir = 1,
  .init_scan = NULL,
  .node_scan_before = fill_md5_scan_node_before,
  .leaf_scan = fill_md5_scan_leaf,
  .fieldset =
        " duf_filenames.pathid AS dirid "
        " , duf_filedatas.id AS filedataid, duf_filedatas.inode AS inode "
        " , duf_filenames.name AS filename, duf_filedatas.size AS filesize "
        " , uid, gid, nlink, inode, mtim AS mtime " " , dupcnt AS nsame "
        " , duf_filenames.id AS filenameid " " , duf_filedatas.mode AS filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.pathid='%llu' ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths AS subpaths WHERE subpaths.parentid=duf_paths.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
