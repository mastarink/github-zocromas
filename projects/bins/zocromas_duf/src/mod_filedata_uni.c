#define DUF_SQL_PDI_STMT

#include <fcntl.h>              /* Definition of AT_* constants */





#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"


#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql2.h"




#include "duf_filedata.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */

static int
filedata_scan_entry_reg2(  /* duf_sqlite_stmt_t * pstmt_unused, */ const char *fname, const struct stat *pst_file, /* unsigned long long dirid, */
                          duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  ( void ) /* dataid= */ duf_insert_filedata_uni( pdi, pst_file, 0 /*need_id */ , &r );
  DEBUG_ENDR( r );
}

#if 0

/* In db sure */
DUF_UNUSED static int
filedata_scan_leaf2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  int fd;
  int ufd;
  struct stat *st;
  struct stat fdst, ufdst;
  struct stat fpst;
  const char *path;
  const char *name;

  DEBUG_STARTR( r );

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
  DUF_SHOW_ERROR( "[%lu:%lu:%lu:%lu] %s%s", st->st_ino, fdst.st_ino, ufdst.st_ino, fpst.st_ino, path, name );
  DEBUG_ENDR( r );
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
DUF_UNUSED static int
filedata_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  struct stat *st;
  const char *path;
  const char *name;

  DEBUG_STARTR( r );
  st = duf_levinfo_stat( pdi );
  path = duf_levinfo_path( pdi );
  name = duf_levinfo_itemname( pdi );
  DUF_PRINTF( 0, "[%lu] %s / %s", st->st_ino, path, name );

  DEBUG_ENDR( r );
}

DUF_UNUSED static int
filedata_scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  /* DUF_SFIELD( filename ); */

  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}


/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
DUF_UNUSED static int
filedata_scan_node_after( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DEBUG_STARTR( r );

  /* DUF_SFIELD( filename ); */



  DEBUG_ENDR( r );
}

DUF_UNUSED static int
filedata_scan_node_after2( duf_sqlite_stmt_t * pstmt_unused, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* DUF_SFIELD( filename ); */



  DEBUG_ENDR( r );
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
DUF_UNUSED static int
filedata_scan_node_middle( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DEBUG_STARTR( r );


  DEBUG_ENDR( r );
}

DUF_UNUSED static int
filedata_scan_node_middle2( duf_sqlite_stmt_t * pstmt_unused, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  DEBUG_ENDR( r );
}

/* Possibly not in db */
DUF_UNUSED static int
filedata_scan_dirent_content2( duf_sqlite_stmt_t * pstmt_unused, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int ufd;
  struct stat *st;
  struct stat fdst, ufdst;
  struct stat fpst;
  const char *path;
  const char *name;

  DEBUG_STARTR( r );

  assert( fd == duf_levinfo_dfd( pdi ) );
  assert( pst_file == duf_levinfo_stat( pdi ) );


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
  DUF_SHOW_ERROR( "[%lu:%lu:%lu:%lu:%lu] %s%s", st->st_ino, fdst.st_ino, ufdst.st_ino, fpst.st_ino, pst_file->st_ino, path, name );
  DEBUG_ENDR( r );
}





DUF_UNUSED static int
filedata_scan_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DEBUG_STARTR( r );

  /* DUF_TRACE( scan, 10, "scan entry dir by %s", fname ); */
  DEBUG_ENDR( r );
}

DUF_UNUSED static int
filedata_scan_entry_dir2( duf_sqlite_stmt_t * pstmt_unused, const char *fname, const struct stat *pstat, unsigned long long dirid,
                          duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* DUF_TRACE( scan, 10, "scan entry dir2 by %s", fname ); */
  DEBUG_ENDR( r );
}

DUF_UNUSED static int
filedata_scan_entry_reg( const char *fname, const struct stat *pst_file, unsigned long long dirid, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DEBUG_STARTR( r );

  ( void ) /* dataid= */ duf_insert_filedata_uni( pdi, pst_file, 0 /*need_id */ , &r );
  DEBUG_ENDR( r );
}
#endif



static duf_beginning_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "md5." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)" */
          ,
          "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "exif AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " /* */
          " WHERE exif." DUF_SQL_IDNAME "=x." DUF_SQL_IDNAME " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,

          "DELETE FROM " DUF_DBPREF "sizes",
          "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /* */
          "SELECT size, COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size" /* */
          ,


#if 0
          /* "DELETE FROM " DUF_DBPREF "pathtot_files", */
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn " /* */
          " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
          " minsize=(SELECT min(size) AS minsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", maxsize=(SELECT max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)" /* */
          ,

          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.ParentId) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
          /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                      */
          /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                 */
          /*       " FROM " DUF_DBPREF "paths AS p " (* *)                       */
          /*       " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *) */
          /*       ,                                                             */
#endif

          /* "DELETE FROM " DUF_DBPREF "keydata", */
          /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
          /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
          /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
          /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
          /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
          /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */

          /* *INDENT-ON*  */

          NULL,
          }
};


duf_scan_callbacks_t duf_filedata_callbacks = {
  .title = "file data",
  .name = "filedata",
  .init_scan = NULL,
  .def_opendir = 1,

  /* .node_scan_before = filedata_scan_node_before, */
  /* .node_scan_before2 = filedata_scan_node_before2, */

  /* .node_scan_after = filedata_scan_node_after,   */
  /* .node_scan_after2 = filedata_scan_node_after2, */

  /* .node_scan_middle = filedata_scan_node_middle,   */
  /* .node_scan_middle2 = filedata_scan_node_middle2, */

  /* .dirent_dir_scan_before = filedata_scan_entry_dir,   */
  /* .dirent_dir_scan_before2 = filedata_scan_entry_dir2, */

  /* .dirent_file_scan_before = filedata_scan_entry_reg, */
  .dirent_file_scan_before2 = filedata_scan_entry_reg2,
  /* .leaf_scan_fd2 = filedata_scan_dirent_content2, */



  /* .leaf_scan = filedata_scan_leaf, */
  /* .leaf_scan2 = filedata_scan_leaf2, */


  
  .use_std_leaf = 0, /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  .use_std_node = 0, /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  .leaf = {.fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize" /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", dup5cnt AS nsame" /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid, fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           " WHERE "            /* */
           " fn.Pathid = :parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           }
  ,
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           }
  ,
  .final_sql_argv = &final_sql  /* */
        ,
};
