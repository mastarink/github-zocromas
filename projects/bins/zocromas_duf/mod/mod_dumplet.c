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
#include "duf_sccb_eval_std.h"

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_pdi_ref.h"

#include "duf_mod_defs.h"

#include "duf_path2dirid.h"                                          /* duf_dirid2path; etc. ✗ */

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#define DUF_MOD_NAME dumplet
/* ########################################################################################## */
#include "duf_mod_types.h"
DUF_MOD_DECLARE_ALL_FUNCS( duf_dumplet )
/* ########################################################################################## */
/* ########################################################################################## */
     static duf_scan_callbacks_t duf_sccb_dispatch;

     const duf_mod_handler_t duf_mod_handler_uni[] = {
       {"sccb", &duf_sccb_dispatch},
       {NULL, NULL}
     };

/* ########################################################################################## */
static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "dumplet",
  .name = "dumplet",
  .def_opendir = 0,
  .init_scan = F2ND( dumplet_init ),
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = F2ND( dumplet_node_before2 ),
  .node_scan_before2_deleted = F2ND( dumplet_node_before2_del ),

  .node_scan_after2 = F2ND( dumplet_node_after2 ),
  .node_scan_after2_deleted = F2ND( dumplet_node_after2_del ),

  .node_scan_middle2 = F2ND( dumplet_node_middle2 ),
  .node_scan_middle2_deleted = F2ND( dumplet_node_middle2_del ),

  .leaf_scan_fd2 = F2ND( dumplet_de_content2 ),
  .leaf_scan_fd2_deleted = F2ND( dumplet_de_content2_del ),

  .leaf_scan2 = F2ND( dumplet_leaf2 ),
  .leaf_scan2_deleted = F2ND( dumplet_leaf2_del ),

  .dirent_file_scan_before2 = F2ND( dumplet_de_file_before2 ),
  .dirent_dir_scan_before2 = F2ND( dumplet_de_dir_before2 ),

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-one",
  .std_node_set_name = "std-node-one",
};

/* ########################################################################################## */

SR( MOD, dumplet_init, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 0, "dumplet_init %s", duf_levinfo_path( pdi ) );

  ER( MOD, dumplet_init, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_de_content2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 );
  assert( duf_levinfo_stat( pdi ) );
  MAST_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  ER( MOD, dumplet_de_content2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_de_content2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  MAST_TRACE( mod, 6, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  ER( MOD, dumplet_de_content2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  assert( !duf_levinfo_dfd( pdi ) );
  assert( 0 == strcmp( DUF_GET_RSFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );

  MAST_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  ER( MOD, dumplet_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_leaf2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/* Never called (no deleted flag - didn't try to open !!) */
  MAST_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  ER( MOD, dumplet_leaf2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_before2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dumplet node before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_before2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_middle2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_middle2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_middle2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dumplet node middle %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_middle2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_after2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_after2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_node_after2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 10, "@dumplet node after %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_node_after2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_de_dir_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 8, "dumplet de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_de_dir_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dumplet_de_file_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 6, "dumplet de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  ER( MOD, dumplet_de_file_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
