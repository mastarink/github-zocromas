#include <fcntl.h>              /* Definition of AT_* constants */

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_filedata.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"

/* ########################################################################################## */
static int register_pdifiledata( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi );

/* ########################################################################################## */


static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ ...",
  .done = 0,
  .beginend = 0,

  .sql = {
#if 0
          "UPDATE " DUF_SQL_TABLES_MD5_FULL " SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd " /* */
          " JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (fd.md5id=md." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_MD5_FULL "." DUF_SQL_IDFIELD "=md." DUF_SQL_IDFIELD ")" /* */
          /* " WHERE " DUF_SQL_TABLES_MD5_FULL ".md5sum1=md.md5sum1 AND " DUF_SQL_TABLES_MD5_FULL ".md5sum2=md.md5sum2)" */
          ,
          "UPDATE " DUF_SQL_TABLES_EXIF_FULL " SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd " /* */
          " JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (fd.exifid=x." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_EXIF_FULL "." DUF_SQL_IDFIELD "=x." DUF_SQL_IDFIELD " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,
#endif
          /* XXX Needless XXX "DELETE FROM " DUF_SQL_TABLES_SIZES_FULL (* *) , */
#if 0
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SIZES_FULL " (size, dupzcnt) " /* */
          /*     */ "SELECT size, COUNT(*) "
          /*     */ " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd GROUP BY fd.size"
#else
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SIZES_FULL " (size) SELECT size FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd" /* */ ,
          "UPDATE " DUF_SQL_TABLES_SIZES_FULL " SET dupzcnt=( " /* */
          /*   */ " SELECT COUNT(*) "
          /*     */ " FROM " DUF_SQL_TABLES_SIZES_FULL " AS sz "
          /*     */ " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fd.size=sz.size ) "
          /*     */ " WHERE " DUF_SQL_TABLES_SIZES_FULL ".size=sz.size "
          " )"                  /* */
#endif
          /* */
          ,


         NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_filedata_callbacks = {
  .title = "file data",
  .name = "filedata",
  .init_scan = NULL,
  .def_opendir = 1,


  .dirent_file_scan_before2 = register_pdifiledata,




/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .count_nodes = 1,
  .leaf = {                     /* */
           .name = "fd leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* Never used!? */
          NULL
           },
  .node = {                     /* */
           .name = "fd node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           "'filedata-node' AS fieldset_id, " /* */
           "  pt." DUF_SQL_IDFIELD " AS dirid" /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs " /* */
           ", " DUF_SQL_RNUMFILES( pt ) " AS rnfiles " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
           ,
           .filter = NULL       /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

static int
register_pdifiledata( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DOR( r, duf_pdistat2file( pdi ) );
  DEBUG_ENDR( r );
}
