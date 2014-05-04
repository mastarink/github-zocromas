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

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_file.h" */
#include "duf_path.h"

#include "duf_sql_def.h"
#include "duf_sql_field.h"
#include "duf_sql.h"
#include "duf_sql1.h"

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

  DEBUG_START(  );
  if ( md64 && md64[1] && md64[0] )
  {
    r = duf_sql( "INSERT OR IGNORE INTO duf_md5 (md5sum1,md5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "md5id" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &md5id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb /*, ( const duf_dirhandle_t * ) NULL off */ ,
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

  DEBUG_ENDULL( md5id );
  return md5id;
}

static int
duf_make_md5_uni( int fd, unsigned char *pmd )
{
  int r = 0;
  size_t bufsz = 1024 * 1024 * 10;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
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
          int rr;

          /* rr = fread( buffer, 1, bufsz, f ); */

          rr = read( fd, buffer, bufsz );
          /* if ( ferror( f ) ) */
          if ( rr < 0 )
          {
            DUF_ERRSYS( "read file" );

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
  if ( MD5_Final( pmd, &ctx ) != 1 )
    r = DUF_ERROR_MD5;
  return r;
}

static int
duf_scan_entry_content( int fd, const struct stat *pst_file, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;
  unsigned char mdr[MD5_DIGEST_LENGTH];
  unsigned char md[MD5_DIGEST_LENGTH];

  DUF_UFIELD( filedataid );

  memset( md, 0, sizeof( md ) );
  r = duf_make_md5_uni( fd, md );
  DUF_TEST_R( r );
  /* reverse */
  for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
    mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];

  if ( r >= 0 )
  {
    unsigned long long md5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &mdr;
    md5id = duf_insert_md5_uni( pmd, pst_file->st_size, 1 /*need_id */ , &r );
    if ( r >= 0 && md5id )
    {
      r = duf_sql( "UPDATE duf_filedatas SET md5id='%llu' WHERE id='%lld'", ( int * ) NULL, md5id, filedataid );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );
    DUF_TRACE( scan, 2, "  L%u: scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id );
  }
  return r;
}

static int
duf_scan_entry_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  unsigned char mdr[MD5_DIGEST_LENGTH];
  unsigned char md[MD5_DIGEST_LENGTH];

  DUF_UFIELD2( filedataid );

  memset( md, 0, sizeof( md ) );
  r = duf_make_md5_uni( fd, md );
  DUF_TEST_R( r );
  /* reverse */
  for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
    mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];

  if ( r >= 0 )
  {
    unsigned long long md5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &mdr;
    md5id = duf_insert_md5_uni( pmd, pst_file->st_size, 1 /*need_id */ , &r );
    if ( r >= 0 && md5id )
    {
      r = duf_sql( "UPDATE duf_filedatas SET md5id='%llu' WHERE id='%lld'", ( int * ) NULL, md5id, filedataid );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );
    DUF_TRACE( scan, 2, "  L%u: scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id );
  }
  return r;
}

static int
duf_scan_entry_content_by_precord( duf_depthinfo_t * pdi, duf_record_t * precord, const char *fname, unsigned long long filesize )
{
  int r = 0;
  int ffd = duf_levinfo_dfd( pdi );

  DUF_ERROR( "ffd:%d for L%u", ffd, duf_pdi_depth( pdi ) );
  if ( ffd )
  {
    DUF_TRACE( md5, 2, "openat ffd:%d", ffd );
    if ( ffd > 0 )
    {
      r = duf_scan_entry_content( ffd, duf_levinfo_stat( pdi ), pdi, precord );
    }
    else
    {
      DUF_ERRSYS( "open to read file : %s", fname );
      r = ffd;
      r = DUF_ERROR_OPEN;
    }
    DUF_TEST_R( r );
    DUF_TEST_R( r );
  }
  else
    r = DUF_ERROR_DATA;
  DUF_TEST_R( r );
  return r;
}

/* callback of type duf_scan_hook_file_t */
__attribute__ ( ( unused ) )
     static int collect_openat_md5_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( filename );
  DUF_UFIELD( filesize );
  DEBUG_START(  );
  r = duf_scan_entry_content_by_precord( pdi, precord, filename, filesize );
  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
__attribute__ ( ( unused ) )
     static int collect_openat_md5_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;
  const char *real_path = NULL;

  DEBUG_START(  );

  real_path = duf_levinfo_path( pdi );
  DUF_TRACE( md5, 0, "L%d; id%-7llu  real_path=%s;", duf_pdi_depth( pdi ), pathid_unused, real_path );

  DEBUG_ENDR( r );
  return r;
}

/* currently used for --same-md5  ??? */




 /* *INDENT-OFF*  */
static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) "
        " FROM duf_filedatas AS fd "
	  " JOIN duf_md5 AS md ON (fd.md5id=md.id) "
        " WHERE duf_md5.md5sum1=md.md5sum1 AND duf_md5.md5sum2=md.md5sum2)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (Pathid, numdirs) "
        " SELECT p.id AS Pathid, COUNT(*) AS numdirs "
	  " FROM duf_paths AS p "
            " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) "
	" GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET "
      " numdirs=(SELECT COUNT(*) AS numdirs "
                  " FROM duf_paths AS p "
                      " WHERE p.parentid=duf_pathtot_dirs.Pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, Pathid) "
      "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid "
	  " FROM duf_filenames AS fn "
	    " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)"
	      " JOIN duf_paths AS p ON (fn.Pathid=p.id)"
	        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

 /* *INDENT-ON*  */

NULL,};



duf_scan_callbacks_t duf_collect_openat_md5_callbacks = {
  .title = "collect o md5",
  .init_scan = NULL,
  .opendir = 1,
  .scan_mode_step=1,
  /* .entry_dir_scan_before = NULL, */
  /* .entry_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_md5_scan_node_before, */
  /*  .leaf_scan =  collect_openat_md5_scan_leaf, */
  .leaf_scan_fd = duf_scan_entry_content,
  .leaf_scan_fd2 = duf_scan_entry_content2,
  .fieldset =
        " duf_filenames.Pathid AS dirid "
        " , duf_filedatas.id AS filedataid, duf_filedatas.inode AS inode "
        " , duf_filenames.name AS filename, duf_filedatas.size AS filesize "
        " , uid, gid, nlink, inode, mtim AS mtime, md.dupcnt AS nsame "
        " , duf_filenames.id AS filenameid " " , duf_filedatas.mode AS filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas ON (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md ON (md.id=duf_filedatas.md5id)" " LEFT JOIN duf_sizes as sz ON (sz.size=duf_filedatas.size)" "    WHERE "
        /* " sz.dupcnt > 1 AND " */
        " duf_filenames.Pathid='%llu' ",
  .leaf_selector2 =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas ON (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md ON (md.id=duf_filedatas.md5id)"
        " LEFT JOIN duf_sizes as sz ON (sz.size=duf_filedatas.size)" "    WHERE " " duf_filenames.Pathid=:dirid ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  .node_selector2 =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=duf_paths.id) " " WHERE duf_paths.parentid=:dirid ",
  .final_sql_argv = final_sql,
};
