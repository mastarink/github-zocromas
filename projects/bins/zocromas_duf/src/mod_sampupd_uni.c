#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>              /* Definition of AT_* constants */
#include <sys/stat.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_sampupd_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
sampupd_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
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
sampupd_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
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
sampupd_scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
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
sampupd_scan_node_after( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_node_after2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
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
sampupd_scan_node_middle( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_node_middle2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
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





static const char *final_sql[] = {
  "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
        " FROM " DUF_DBPREF "filedatas AS fd " /* */
        " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md.id) " /* */
        " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)",
  /* "DELETE FROM " DUF_DBPREF "pathtot_files", */
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " GROUP BY fn.Pathid",
  "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)",
 
   "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents.id=paths.parentid) " /* */
        " GROUP BY parents.id"  /* */
        ,


  "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths AS p " /* */
        " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )",
  /* "DELETE FROM " DUF_DBPREF "keydata", */
  "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " /* */
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id)" /* */
        " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p.id)" /* */
        " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md.id)",

  /* *INDENT-ON*  */

  NULL,
};




duf_scan_callbacks_t duf_sampupd_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .opendir = 1,
  .scan_mode_2 = 1,

  .node_scan_before = sampupd_scan_node_before,
  .node_scan_before2 = sampupd_scan_node_before2,

  .node_scan_after = sampupd_scan_node_after,
  .node_scan_after2 = sampupd_scan_node_after2,

  .node_scan_middle = sampupd_scan_node_middle,
  .node_scan_middle2 = sampupd_scan_node_middle2,

  .leaf_scan = sampupd_scan_leaf,
  .leaf_scan2 = sampupd_scan_leaf2,

  .leaf_scan_fd2 = sampupd_scan_dirent_content2,

  .leaf_fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize" /* */
        " , uid, gid, nlink, inode, mtim AS mtime " /* */
        " , dup5cnt AS nsame"    /* */
        " , fn.id AS filenameid, fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
        ,
  .leaf_selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        " fn.Pathid='%llu' "    /* */
        ,
  .leaf_selector2 = /*	*/
        /* "SELECT %s " */
    	" FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirid "  /* */
        " AND (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dup5cnt>=:minsame) AND (:maxsame IS NULL OR md.dup5cnt<=:maxsame) " /* */
        ,
 .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        "     (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dup5cnt>=:minsame) AND (:maxsame IS NULL OR md.dup5cnt<=:maxsame) " /* */
        ,
  .node_fieldset = "pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
	,
  .node_selector = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM     " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.ParentId='%llu' " /* */
        ,
  .node_selector2 = /*	*/
        /* "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
        /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.ParentId = :dirid " /* */
        ,
  .final_sql_argv = final_sql   /* */
        ,
};
