#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <fcntl.h>              /* Definition of AT_* constants */
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_dbg.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_field.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_filedata.h"




/* callback of type duf_scan_hook_file_t */
static int
filedata_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

/* In db sure */
static int
filedata_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;
  int fd;
  int ufd;
  struct stat *st;
  struct stat fdst, ufdst;
  struct stat fpst;
  const char *path;
  const char *name;

  DEBUG_START(  );

  fd = duf_levinfo_dfd( pdi );
  ufd = duf_levinfo_dfd_up( pdi );
  st = duf_levinfo_stat( pdi ); /* stat info for file */
  path = duf_levinfo_path( pdi ); /* location for file, ends with '/' */
  name = duf_levinfo_itemname( pdi ); /* file name */
  r = fstat( fd, &fdst );
  r = fstatat( ufd, name, &ufdst, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
  {
    char *fpath;

    fpath = mas_strdup( path );
    fpath = mas_strcat_x( fpath, name );
    r = stat( fpath, &fpst );
    mas_free( fpath );
  }
/* Same! st->; fdst.; ufdst.; fpst.; -- use fd, st, path, name */
  DUF_ERROR( "[%lu:%lu:%lu:%lu] %s%s", st->st_ino, fdst.st_ino, ufdst.st_ino, fpst.st_ino, path, name );
  DEBUG_ENDR( r );
  return r;
}


/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
filedata_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;
  struct stat *st;
  const char *path;
  const char *name;

  DEBUG_START(  );
  st = duf_levinfo_stat( pdi );
  path = duf_levinfo_path( pdi );
  name = duf_levinfo_itemname( pdi );
  DUF_PRINTF( 0, "[%lu] %s / %s", st->st_ino, path, name );

  DEBUG_ENDR( r );
  return r;
}

static int
filedata_scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}


/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
filedata_scan_node_after( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
filedata_scan_node_after2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}


/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
filedata_scan_node_middle( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
filedata_scan_node_middle2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

/* Possibly not in db */
static int
filedata_scan_dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  int ufd;
  struct stat *st;
  struct stat fdst, ufdst;
  struct stat fpst;
  const char *path;
  const char *name;

  DEBUG_START(  );

  ufd = duf_levinfo_dfd_up( pdi );
  st = duf_levinfo_stat( pdi ); /* stat info for file */
  path = duf_levinfo_path( pdi ); /* location for file, ends with '/' */
  name = duf_levinfo_itemname( pdi ); /* file name */
  r = fstat( fd, &fdst );
  r = fstatat( ufd, name, &ufdst, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
  {
    char *fpath;

    fpath = mas_strdup( path );
    fpath = mas_strcat_x( fpath, name );
    r = stat( fpath, &fpst );
    mas_free( fpath );
  }
/* Same! st->; fdst.; ufdst.; fpst.; pst_file->; -- use fd, st, path, name */
  DUF_ERROR( "[%lu:%lu:%lu:%lu:%lu] %s%s", st->st_ino, fdst.st_ino, ufdst.st_ino, fpst.st_ino, pst_file->st_ino, path, name );
  DEBUG_ENDR( r );
  return r;
}





static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) " /* */
        " FROM duf_filedatas AS fd " /* */
        " JOIN duf_md5 AS md ON (fd.md5id=md.id) " /* */
        " WHERE duf_md5.md5sum1=md.md5sum1 AND duf_md5.md5sum2=md.md5sum2)",
  /* "DELETE FROM duf_pathtot_files", */
  "INSERT OR IGNORE INTO duf_pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " GROUP BY fn.Pathid",
  "UPDATE duf_pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT p.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM duf_paths AS p " /* */
        " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " /* */
        " GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM duf_paths AS p " /* */
        " WHERE p.parentid=duf_pathtot_dirs.Pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, Pathid) " /* */
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)" /* */
        " JOIN duf_paths AS p ON (fn.Pathid=p.id)" /* */
        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

  /* *INDENT-ON*  */

  NULL,
};

static int
filedata_scan_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  DUF_TRACE( scan, 0, "scan entry dir by %s", fname );
  DUF_TEST_R( r );
  return r;
}

static int
filedata_scan_entry_dir2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pstat, unsigned long long dirid,
                          duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_TRACE( scan, 0, "scan entry dir2 by %s", fname );
  DUF_TEST_R( r );
  return r;
}

static int
filedata_scan_entry_reg( const char *fname, const struct stat *pst_file, unsigned long long dirid, duf_depthinfo_t * pdi,
                    duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  ( void ) /* dataid= */ duf_insert_filedata_uni( pdi, pst_file, 0 /*need_id */ , &r );
  DEBUG_ENDR( r );
  return r;
}

static int
filedata_scan_entry_reg2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pst_file, unsigned long long dirid,
                     duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  ( void ) /* dataid= */ duf_insert_filedata_uni( pdi, pst_file, 0 /*need_id */ , &r );
  DEBUG_ENDR( r );
  return r;
}


duf_scan_callbacks_t duf_filedata_callbacks = {
  .title = "filedata",
  .init_scan = NULL,
  .opendir = 1,
  .scan_mode_2 = 1,

  .node_scan_before = filedata_scan_node_before,
  .node_scan_before2 = filedata_scan_node_before2,

  .node_scan_after = filedata_scan_node_after,
  .node_scan_after2 = filedata_scan_node_after2,

  .node_scan_middle = filedata_scan_node_middle,
  .node_scan_middle2 = filedata_scan_node_middle2,

  .dirent_dir_scan_before = filedata_scan_entry_dir,
  .dirent_dir_scan_before2 = filedata_scan_entry_dir2,

  .dirent_file_scan_before = filedata_scan_entry_reg,
  .dirent_file_scan_before2 = filedata_scan_entry_reg2,



  .leaf_scan = filedata_scan_leaf,
  .leaf_scan2 = filedata_scan_leaf2,

  .leaf_scan_fd2 = filedata_scan_dirent_content2,

  .fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize" /* */
        " , uid, gid, nlink, inode, mtim AS mtime " /* */
        " , dupcnt AS nsame"    /* */
        " , fn.id AS filenameid, fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
        ,
  .leaf_selector = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        " fn.Pathid='%llu' "    /* */
        ,
  .leaf_selector2 = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN duf_md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirid "  /* */
        " AND (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dupcnt>=:minsame) AND (:maxsame IS NULL OR md.dupcnt<=:maxsame) " /* */
        ,
  .node_selector = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM     duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid='%llu' " /* */
        ,
  .node_selector2 = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = :dirid " /* */
        ,
  .final_sql_argv = final_sql   /* */
        ,
};
