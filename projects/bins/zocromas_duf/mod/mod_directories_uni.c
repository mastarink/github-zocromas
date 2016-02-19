#include <string.h>

#include "duf_maintenance.h"
#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t */
#include "sql_beginning_types.h"                                     /* duf_sql_sequence_t */

/* #include "duf_config.h" */

#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"

#include "duf_path2db.h"                                             /* duf_levinfo_stat2dirid */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"

/* ########################################################################################## */
static int register_pdidirectory( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi );

/* ########################################################################################## */

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-directories",
  .done = 0,
  .sql = {
          "DELETE FROM path_pairs"                                   /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT COUNT(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fna"           /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fda ON (fna.dataid=fda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_MD5_FULL " AS mda ON (fda.md5id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fdb.md5id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb ON (fdb." DUF_SQL_IDFIELD "=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna." DUF_SQL_FILENAMEFIELD "=fnb." DUF_SQL_FILENAMEFIELD "" /* */
          " GROUP BY Pathid1, Pathid2"                               /* */
          ,

          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_dirs_callbacks = {
  .title = "directories",
  .name = "dirs",
  .init_scan = NULL,
  .def_opendir = 1,

  .dirent_dir_scan_before2 = register_pdidirectory,

  .no_count = 1,
  .no_progress = 1,
  .count_nodes = 1,
#if 0
# if 0
  .beginning_sql_seq = &sql_create_selected,
# else
  .beginning_sql_seq = &sql_update_selected,
# endif
#endif
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-one",
  .std_node_set_name = "std-node-two",
/* filename for debug only */
  .leaf = {                                                          /* */
           .name = "dirs-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* Never used!? */
           NULL},
  .node = {                                                          /* */
           .name = "dirs-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'dirs-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid "                       /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs "                 /* */
           ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles "            /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
#endif
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
           ,
           .filter = NULL                                            /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

/* make sure dir name in db */
static int
register_pdidirectory( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

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
                                  &duf_dirs_callbacks.node /*, 0 need_id - no error (1=error) if there is no record */  ) );
  DUF_ENDR( r );
}
