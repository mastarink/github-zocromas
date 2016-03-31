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

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

/* ########################################################################################## */
/* DUF_MOD_DECLARE_ALL_FUNCS( set_dir_priority ) */
static int duf_set_dir_priority_node_before2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_set_dir_priority_node_middle2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_set_dir_priority_node_after2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */

duf_scan_callbacks_t duf_mod_handler = {
  .title = "set dir priority",
  .name = "set_dir_priority",
  .def_opendir = 0,                                                  /* ?? */

  .node_scan_before2 = duf_set_dir_priority_node_before2,
  .node_scan_after2 = duf_set_dir_priority_node_after2,
  .node_scan_middle2 = duf_set_dir_priority_node_middle2,

  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* extern duf_scan_callbacks_t set_dir_priority_cbs __attribute__ ( ( alias( "duf_set_dir_priority_callbacks" ) ) ); */

/* ########################################################################################## */

static
SR( MOD, set_dir_priority_node_before2, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
  MAST_TRACE( mod, 1, "before %d dirid:%llu %s : %s: %s", DUF_CONFIGG( vars.dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
              duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );

  {
    int changes = 0;
    const char *sql = "UPDATE " DUF_SQL_TABLES_PATHS_FULL " SET priority=:priority WHERE " DUF_SQL_IDFIELD " =:dirId ";

    DUF_SQL_SE_START_STMT( pdi, update_priority, sql, pstmt );
    MAST_TRACE( mod, 3, "S:%s", sql );
    DUF_SQL_SE_BIND_LL( dirId, duf_levinfo_dirid( pdi ), pstmt );
    DUF_SQL_SE_BIND_LL( priority, DUF_CONFIGG( vars.dir_priority ), pstmt );
    DUF_SQL_SE_STEPC( pstmt );
    DUF_SQL_SE_CHANGES( changes, pstmt );
    DUF_SQL_SE_END_STMT( pdi, update_priority, pstmt );
    duf_pdi_reg_changes( pdi, changes );
/* DUF_TEST_R( r ); */
  }
/*  DUF_ENDR( r );*/
  ER( MOD, set_dir_priority_node_before2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, set_dir_priority_node_middle2, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

  MAST_TRACE( mod, 1, "middle %d dirid:%llu %s : %s: %s", DUF_CONFIGG( vars.dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
              duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
/*  DUF_ENDR( r );*/
  ER( MOD, set_dir_priority_node_middle2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, set_dir_priority_node_after2, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

  MAST_TRACE( mod, 1, "after  %d dirid:%llu %s : %s: %s", DUF_CONFIGG( vars.dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
              duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
/*  DUF_ENDR( r );*/
  ER( MOD, set_dir_priority_node_after2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
