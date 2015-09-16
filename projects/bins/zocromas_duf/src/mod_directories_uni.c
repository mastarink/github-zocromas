#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"

#include "duf_path2db.h"        /* duf_dirname2dirid */


#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"

/* ########################################################################################## */
static int register_pdidirectory( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );

/* ########################################################################################## */

static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ directories",
  .done = 0,
  .sql = {
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid",
#endif
          "DELETE FROM path_pairs" /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT count(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fna" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fda ON (fna.dataid=fda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_MD5_FULL " AS mda ON (fda.md5id=mda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fdb.md5id=mda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb ON (fdb.rowid=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna.name=fnb.name" /* */
          " GROUP BY Pathid1, Pathid2" /* */
          ,


#if 0
          "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
          " minsize=(SELECT min(size) AS minsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", maxsize=(SELECT max(size) AS maxsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)",

          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_SQL_TABLES_PATHS_FULL " " /* */
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON (parents." DUF_SQL_IDNAME "=" DUF_SQL_TABLES_PATHS_FULL ".parentid) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
          " numdirs=(SELECT COUNT(*) AS numdirs " /* */
          " FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
          " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )",
#endif


          /* "DELETE FROM " DUF_DBPREF "keydata", */
          /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
          /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
          /*       " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " (* *)                                         */
          /*       " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
          /*       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
          /*       " JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */


          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_directories_callbacks = {
  .title = "directories",
  .name = "dirs",
  .init_scan = NULL,
  .def_opendir = 1,


  .dirent_dir_scan_before2 = register_pdidirectory,

  .count_nodes = 1,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  /* filename for debug only */
  .leaf = {                     /* */
           .name = "dirs leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* Never used!? */
#if 0
           "'dirs-leaf' AS fieldset_id, " /* */
           "  fn.Pathid AS dirid " /* */
           ", 0 AS ndirs, 0 AS nfiles" /* */
           ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
           ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", fn." DUF_SQL_IDNAME " AS nameid " /* */
           ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
           ", fd." DUF_SQL_IDNAME " AS dataid " /* */
           ", md.dup5cnt AS nsame " /* */
           ", fd.md5id AS md5id" /* */
           /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fd.exifid AS exifid, fd.mimeid AS mimeid " /* */
           ", fd.size AS filesize " /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           ,
           .matcher = " fn.Pathid = :parentdirID " /* */
           ,
           .filter = NULL       /* */
#else
           NULL
#endif
           },
  .node = {                     /* */
           .name = "dirs node",
           .type = DUF_NODE_NODE,
   .expand_sql = 1,        /* */
           .fieldset =          /* */
           /* "'dirs-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDNAME " AS dirid " /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           ,
           .filter = NULL       /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

/* make sure dir name in db */
static int
register_pdidirectory( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( pdi ) ) );
  {
    struct stat *st = duf_levinfo_stat( pdi );

    assert( st->st_dev == pst_dir_unused->st_dev );
    assert( st->st_ino == pst_dir_unused->st_ino );
    assert( st->st_mode == pst_dir_unused->st_mode );
    assert( st->st_nlink == pst_dir_unused->st_nlink );
    assert( st->st_uid == pst_dir_unused->st_uid );
    assert( st->st_gid == pst_dir_unused->st_gid );
    assert( st->st_rdev == pst_dir_unused->st_rdev );
    assert( st->st_size == pst_dir_unused->st_size );
    assert( st->st_blksize == pst_dir_unused->st_blksize );
    assert( st->st_blocks == pst_dir_unused->st_blocks );
    /* assert( st->st_atim == pst_dir_unused->st_atim ); */
    /* assert( st->st_mtim == pst_dir_unused->st_mtim ); */
    /* assert( st->st_ctim == pst_dir_unused->st_ctim ); */
    assert( 0 == memcmp( st, pst_dir_unused, sizeof( struct stat ) ) );
    assert( pst_dir_unused == st );
  }
#endif

/* fname === */
  DUF_TRACE( mod, 0, "@ scan entry dir 2 by %s", duf_levinfo_itemshowname( pdi ) );

  DOR( r, duf_levinfo_stat2dirid( pdi, 1 /* caninsert */ ,
                                  &duf_directories_callbacks.node /*, 0 need_id - no error (1=error) if there is no record */  ) );
  DEBUG_ENDR( r );
}
