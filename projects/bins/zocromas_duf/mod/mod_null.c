/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_openclose.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_stat.h"

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_mod_defs.h"

#include "duf_nodetype.h"                                            /* duf_nodetype_name ✗ */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_levinfo_structs.h"

/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( duf_null )
/* ########################################################################################## */
/* ########################################################################################## */
     duf_scan_callbacks_t duf_mod_handler = {
       .title = "null",
       .name = "null",
       .def_opendir = 0,
       .init_scan = F2ND( null_init ),

       .node_scan_before2 = F2ND( null_node_before2 ),
       .node_scan_before2_deleted = F2ND( null_node_before2_del ),

       .node_scan_after2 = F2ND( null_node_after2 ),
       .node_scan_after2_deleted = F2ND( null_node_after2_del ),

       .node_scan_middle2 = F2ND( null_node_middle2 ),
       .node_scan_middle2_deleted = F2ND( null_node_middle2_del ),

       .leaf_scan_fd2 = F2ND( null_de_content2 ),
       .leaf_scan_fd2_deleted = F2ND( null_de_content2_del ),

       .leaf_scan2 = F2ND( null_leaf2 ),
       .leaf_scan2_deleted = F2ND( null_leaf2_del ),

       .dirent_file_scan_before2 = F2ND( null_de_file_before2 ),
       .dirent_dir_scan_before2 = F2ND( null_de_dir_before2 ),

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
       .use_std_leaf_set_num = 2,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .use_std_node_set_num = 2,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .std_leaf_set_name = "std-leaf-no-sel",
       .std_node_set_name = "std-node-two",
     };

/* ########################################################################################## */

SR( MOD, null_init, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 0, "null_init %s", duf_levinfo_path( pdi ) );

  ER( MOD, null_init, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_de_content2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 );
  assert( duf_levinfo_stat( pdi ) );

#endif

/*
* 2: 0 [MOD    ]  47:null_de_content2                 :3.8916 :  null de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  MAST_TRACE( mod, 1, "null de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( pdi ), fname,
              0 == strcmp( duf_levinfo_itemshowname( pdi ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( pdi ), duf_levinfo_dfd( pdi ),
              duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_stat_dev( pdi ) );

  ER( MOD, null_de_content2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_de_content2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
#endif

/*
* 2: 0 [MOD    ]  47:null_de_content2                 :3.8916 :  null de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  MAST_TRACE( mod, 0, "@null de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( pdi ), fname,
              0 == strcmp( duf_levinfo_itemshowname( pdi ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( pdi ), duf_levinfo_dfd( pdi ),
              duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_stat_dev( pdi ) );

  ER( MOD, null_de_content2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );

  assert( !duf_levinfo_dfd( pdi ) );
/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );
#endif

  MAST_TRACE( mod, 4, "null %s : %s -a-", duf_levinfo_path( pdi ), fname );
  MAST_TRACE( mod, 2, "@null %s : %s -b- ::  {dfd:%d; source:%d} dev:x%llx; inode:%llu", duf_levinfo_itemshowname( pdi ), fname,
              duf_levinfo_dfd( pdi ), duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_dbstat_dev( pdi ),
              ( unsigned long long ) duf_levinfo_dbstat_inode( pdi ) );

  ER( MOD, null_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_leaf2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@@null %s : %s", duf_levinfo_path( pdi ), fname );
/* Never called (no deleted flag - didn't try to open !!) */
  assert( 0 );

  ER( MOD, null_leaf2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "null %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  {
    duf_levinfo_t *pli MAS_UNUSED = NULL;

    pli = duf_levinfo_ptr( pdi );

    QT( "@======== %s : %s; %s {%lld:%lld}", duf_levinfo_relpath( pdi ), duf_levinfo_itemtruename( pdi ),
        duf_nodetype_name( duf_levinfo_node_type( pdi ) ), pli ? ( long long ) pli->scanned_childs.nodes : -1,
        pli ? ( long long ) pli->scanned_childs.leaves : -1 );
  }
#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "null %s : %s", duf_levinfo_path( pdi ), fname );
#endif
  ER( MOD, null_node_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_before2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@null node before: %s : %s", duf_levinfo_path( pdi ), fname );

  ER( MOD, null_node_before2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_middle2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "null %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "null node middle: %s : %s", duf_levinfo_path( pdi ), fname );
#endif

  ER( MOD, null_node_middle2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_middle2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@null node middle %s : %s", duf_levinfo_path( pdi ), fname );

  ER( MOD, null_node_middle2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_after2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "null %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "null node after: %s : %s", duf_levinfo_path( pdi ), fname );
#endif

  ER( MOD, null_node_after2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_node_after2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@null node after %s : %s", duf_levinfo_path( pdi ), fname );

  ER( MOD, null_node_after2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_de_dir_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "null de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  ER( MOD, null_de_dir_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, null_de_file_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "null de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, null_de_file_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
