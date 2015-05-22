#define DUF_SQL_PDI_STMT
#include <string.h>






#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"


#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"






/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */

static int
scan_init( void )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_SFIELD2( filename );

  DUF_TRACE( mod, 0, "dummy dirent %s : %s", duf_levinfo_path( pdi ), filename );
  assert( fd == duf_levinfo_dfd( pdi ) );
  assert( pst_file == duf_levinfo_stat( pdi ) );

  DEBUG_ENDR( r );
}

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_leaf2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_before2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy node before: %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy node middle: %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_middle2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy node middle %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy node after: %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_after2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_SFIELD2( filename );
  DUF_TRACE( mod, 0, "dummy node after %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

int
dirent_dir_scan_before2( const char *fname, const struct stat *pstat, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 0, "dummy dirent dir before: %s : %s", duf_levinfo_path( pdi ), fname );


  DEBUG_ENDR( r );
}

int
dirent_file_scan_before2( const char *fname, const struct stat *pstat, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 0, "dummy dirent file before: %s : %s", duf_levinfo_path( pdi ), fname );


  DEBUG_ENDR( r );
}

static duf_beginning_t final_sql = {.done = 0,
  .sql = {


          NULL,
          }
};


duf_scan_callbacks_t duf_dummy_callbacks = {
  .title = "dummy",
  .name = "dummy",
  .def_opendir = 0,
  .init_scan = scan_init,

  .node_scan_before2 = scan_node_before2,
  .node_scan_before2_deleted = scan_node_before2_deleted,

  .node_scan_after2 = scan_node_after2,
  .node_scan_after2_deleted = scan_node_after2_deleted,

  .node_scan_middle2 = scan_node_middle2,
  .node_scan_middle2_deleted = scan_node_middle2_deleted,

  .leaf_scan_fd2 = dirent_content2,

  .leaf_scan2 = scan_leaf2,
  .leaf_scan2_deleted = scan_leaf2_deleted,

  .dirent_file_scan_before2 = dirent_file_scan_before2,
  .dirent_dir_scan_before2 = dirent_dir_scan_before2,

  .leaf = {.fieldset = "fn.pathid AS dirid " /* */
           ", fn.name AS filename, fd.size AS filesize" /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", dup5cnt AS nsame " /* */
           ", fd." DUF_SQL_IDNAME " AS filenameid" /* */
           ", fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           " FROM      " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid            =fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF "md5       AS md ON ( md." DUF_SQL_IDNAME "=fd.md5id              ) " /* */
           "    WHERE "         /* */
           " fn.pathid = :parentdirID" /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           },
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs            AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files           AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           },
  .final_sql_argv = &final_sql,
};
