#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_types.h"
#include "duf_errors_headers.h"


#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql.h"
#include "duf_sql2.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"


static int
template_scan_init( void )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_leaf2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_before2_deleted( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_middle2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_middle2_deleted( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_after2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
template_scan_node_after2_deleted( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

int
template_dirent_dir_scan_before2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pstat,
                                  unsigned long long dirid, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

int
template_dirent_file_scan_before2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pstat,
                                   unsigned long long dirid, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static const char *final_sql[] = {


  NULL,
};


duf_scan_callbacks_t duf_template_callbacks = {
  .title = "module",
  .def_opendir = 0,
  .init_scan = template_scan_init,
  .scan_mode_2 = 1,

  .node_scan_before2 = template_scan_node_before2,
  .node_scan_before2_deleted = template_scan_node_before2_deleted,

  .node_scan_after2 = template_scan_node_after2,
  .node_scan_after2_deleted = template_scan_node_after2_deleted,

  .node_scan_middle2 = template_scan_node_middle2,
  .node_scan_middle2_deleted = template_scan_node_middle2_deleted,

  .leaf_scan_fd2 = template_dirent_content2,

  .leaf_scan2 = template_scan_leaf2,
  .leaf_scan2_deleted = template_scan_leaf2_deleted,

  .dirent_file_scan_before2 = template_dirent_file_scan_before2,
  .dirent_dir_scan_before2 = template_dirent_dir_scan_before2,

  .leaf_fieldset = "fn.pathid AS dirid " /* */
        " , fn.name AS filename, fd.size AS filesize" /* */
        " , uid, gid, nlink, inode, mtim AS mtime " /* */
        " , dup5cnt AS nsame "   /* */
        " , fd.id AS filenameid" /* */
        " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
        ,
  .leaf_selector2 = " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        " fn.pathid = :dirid"   /* */
        ,
  .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /* */
        ,
  .node_fieldset = "pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ,
  .node_selector2 =             /* */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.ParentId=:dirid" /* */
        ,
  .final_sql_argv = final_sql,
};
