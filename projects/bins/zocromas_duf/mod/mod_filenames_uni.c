/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_structs.h"

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "duf_filedata.h"

#include "duf_mod_defs.h"

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ########################################################################################## */
#include "duf_mod_types.h"

static int duf_filenames_leaf2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_filenames_leaf2_deleted( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_filenames_de_file_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {                              /* */
  .name = "final-filenames",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET dupdatacnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "           /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_FILEDATAS_FULL "." DUF_SQL_IDFIELD "=fd." DUF_SQL_IDFIELD ")" /* */
          ,
          NULL}
};

/* ########################################################################################## */
static duf_scanner_set_t scanners[] MAS_UNUSED = {
  {
   .flags = DUF_SCANNER_SET_FLAG_DIRENT,                             /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_FS_ITEMS,                              /* */
   .fun = F2ND( filenames_de_file_before2 ),                         /* */
   },
 {
   .flags = DUF_SCANNER_SET_FLAG_DB,                                 /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                              /* */
   .fun = F2ND( filenames_leaf2 ),                         /* */
   },

  {.fun = NULL}
};

duf_scan_callbacks_t duf_mod_sccb_handler = {
  .title = "file names",
  .name = "filenames",
  .init_scan = NULL,
  .def_opendir = 1,

/* .dirent_file_scan_before = F2ND(filenames_entry_reg), */
#if 0
  .dirent_file_scan_before2 = F2ND( filenames_de_file_before2 ),
  .leaf_scan2 = F2ND( filenames_leaf2 ),
#else
  .scanners = scanners,
#endif
  .leaf_scan2_deleted = F2ND( filenames_leaf2_deleted ),

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
  .count_nodes = 1,
  .leaf = {                                                          /* */
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           NULL},
  .node = {                                                          /* */
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
           "'filenames-node' AS fieldset_id, "                       /* */
           " pt." DUF_SQL_IDFIELD " AS dirid"                        /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,

/* 
 * TODO 20160118.164635
 *  1. filedata need to put file counter into paths table
 *  2. filenames to use it as counter!
 *  currently filenames counts childs dir, therefore doesn't scan files is there are no child dirs
 * */
#ifdef DUF_USE_CTE
           .cte =                                                    /* */
           "W? ITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
           " ( "                                                     /* */
           "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths " /* */
           "   WHERE parentid=:topDirID "                            /* */
           "  UNION "                                                /* */
           "   SELECT paths." DUF_SQL_IDFIELD ",paths.parentid "     /* */
           "    FROM cte_paths "                                     /* */
           "    JOIN paths ON(paths.parentid=cte_paths." DUF_SQL_IDFIELD ") " /* */
           " ) ",
/*
 WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS 
   (SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths 
      WHERE parentid=15 --------- matcher
     UNION 
     SELECT paths." DUF_SQL_IDFIELD ",paths.parentid 
        FROM cte_paths 
        JOIN paths ON(paths.parentid=cte_paths." DUF_SQL_IDFIELD ")
   )
   SELECT *
      FROM cte_paths AS pte
      LEFT JOIN paths AS pt ON(pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ")
      LEFT JOIN t_common_pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ")
      LEFT JOIN t_common_pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") */
           .selector2_cte =                                          /* */
           " FROM cte_paths " /*                                  */ " AS pte " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*             */ " AS pt ON (pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ") " /* */
           ,
#endif
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL /*                  */ " AS pt " /* */
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
           ,
           .filter = NULL                                            /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
static
SR( MOD, filenames_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  if ( 0 == strcmp( duf_levinfo_itemtruename( pdi ), "paths-with-jpg" ) )
  {
    QT( "@[%d] %s :: %s", duf_pdi_depth( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
  }

  ER( MOD, filenames_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, filenames_leaf2_deleted, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( todo, 0, "@@@@@@@[%d] %s%s", duf_pdi_depth( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
/* TODO remove or mark name from DB */
  assert( 0 );
  ER( MOD, filenames_leaf2_deleted, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, filenames_de_file_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  const char *fname = duf_levinfo_itemtruename( pdi );

  unsigned long long dataid;

  if ( QNOERR )
#if 0
    dataid = duf_pdistat2file_dataid_existed( pdi, sccbh, QPERRIND );
#else
    dataid = CRP( pdistat2file_dataid_existed, pdi, sccbh );
#endif
  assert( dataid > 0 );
  if ( QNOERR && fname && duf_levinfo_dirid_up( pdi ) )
  {
    int changes = 0;

    const char *sql =
            "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILENAMES_FULL " (pathID, " DUF_SQL_FILENAMEFIELD ", dataid) VALUES (:pathID, :Name, :dataID)";

    DUF_SQL_SE_START_STMT( pdi, insert_filename, sql, pstmt_local );
    MAST_TRACE( mod, 3, "S:%s", sql );
    DUF_SQL_SE_BIND_LL( pathID, duf_levinfo_dirid_up( pdi ), pstmt_local );
    DUF_SQL_SE_BIND_S( Name, fname, pstmt_local );
    DUF_SQL_SE_BIND_LL( dataID, dataid, pstmt_local );
    DUF_SQL_SE_STEPC( pstmt_local );
    DUF_SQL_SE_CHANGES( changes, pstmt_local );
    DUF_SQL_SE_END_STMT( pdi, insert_filename, pstmt_local );        /* clears SQL_ROW / SQL_DONE */
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) ); */
    ERRMAKE_M( DATA, "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) );

  }
/* MAST_TRACE( mod, 0, "%llu : %s @ %llu", dirid, fname, dirid ); */

  ER( MOD, filenames_de_file_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
