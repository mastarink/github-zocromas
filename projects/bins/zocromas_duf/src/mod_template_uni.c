#define DUF_SQL_PDI_STMT
#include <string.h>






#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"


#include "duf_levinfo_ref.h"

#include "duf_sql.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"





/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */
#if 1
DUF_MOD_DECLARE_ALL_FUNCS( template )
#else
static int template_init( duf_depthinfo_t * pdi );
static int template_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_node_before2_del( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_node_after2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_node_after2_del( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_node_middle2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_node_middle2_del( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_de_content2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_leaf2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_leaf2_del( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int template_de_file_before2( duf_depthinfo_t * pdi );
static int template_de_dir_before2( duf_depthinfo_t * pdi );
#endif

static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {


          NULL,
          }
};


duf_scan_callbacks_t duf_template_callbacks = {
  .title = "module",
  .name = "template",
  .def_opendir = 0,
  .init_scan = template_init,

  .node_scan_before2 = template_node_before2,
  .node_scan_before2_deleted = template_node_before2_del,

  .node_scan_after2 = template_node_after2,
  .node_scan_after2_deleted = template_node_after2_del,

  .node_scan_middle2 = template_node_middle2,
  .node_scan_middle2_deleted = template_node_middle2_del,

  .leaf_scan_fd2 = template_de_content2,
  .leaf_scan_fd2_deleted = template_de_content2_del,

  .leaf_scan2 = template_leaf2,
  .leaf_scan2_deleted = template_leaf2_del,

  .dirent_file_scan_before2 = template_de_file_before2,
  .dirent_dir_scan_before2 = template_de_dir_before2,


  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {.fieldset = "fn.pathid AS dirid " /* */
           ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize" /* */
           ", 0 as ndirs, 0 as nfiles" /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           ", dup5cnt AS nsame " /* */
           ", fd." DUF_SQL_IDNAME " AS filenameid" /* */
           ", fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 = " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
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
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
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
           ,
           .selector_total2 =   /* */
           " /* t */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
template_init( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_de_content2( duf_sqlite_stmt_t * pstmt_unused, /* int fd_unused, *//* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  /* assert( fd_unused == duf_levinfo_dfd( pdi ) ); */

  DEBUG_ENDR( r );
}

static int
template_de_content2_del( duf_sqlite_stmt_t * pstmt_unused, /* int fd_unused, *//* const struct stat *pst_file_needless, */
                          duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  /* assert( fd_unused == duf_levinfo_dfd( pdi ) ); */

  DEBUG_ENDR( r );
}

static int
template_leaf2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_leaf2_del( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_node_before2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  DEBUG_ENDR( r );
}

static int
template_node_before2_del( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  DEBUG_ENDR( r );
}

static int
template_node_middle2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_node_middle2_del( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_node_after2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  DEBUG_ENDR( r );
}

static int
template_node_after2_del( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
template_de_dir_before2(  /* const char *fname_unused, const struct stat *pstat_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  /* unsigned long long dirid = duf_levinfo_dirid_up( pdi ); */

  DEBUG_ENDR( r );
}

static int
template_de_file_before2(  /* const char *fname_unused, const struct stat *pstat_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  /* unsigned long long dirid = duf_levinfo_dirid_up( pdi ); */

  DEBUG_ENDR( r );
}
