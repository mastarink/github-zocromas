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

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
#include "duf_sccb_structs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_tags.h"

/* ########################################################################################## */
#include "duf_mod_types.h"
/* DUF_MOD_DECLARE_ALL_FUNCS( tagit ) */
static int duf_tagit_init( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_tagit_leaf2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_tagit_node_before2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_tagit_node_middle2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
static int duf_tagit_node_after2( duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */

/* ########################################################################################## */

static duf_scan_callbacks_t duf_sccb_dispatch;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"sccb", &duf_sccb_dispatch},
  {NULL, NULL}
};

/* ########################################################################################## */
static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "tag it",
  .name = "tagit",
  .def_opendir = 0,
  .init_scan = F2ND( tagit_init ),
#if 0
  .beginning_sql_seq = &sql_create_selected,
#elif 0
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = F2ND( tagit_node_before2 ),
/* .node_scan_before2_deleted = F2ND(tagit_node_before2_del), */

  .node_scan_after2 = F2ND( tagit_node_after2 ),
/* .node_scan_after2_deleted = F2ND(tagit_node_after2_del), */

  .node_scan_middle2 = F2ND( tagit_node_middle2 ),
/* .node_scan_middle2_deleted = F2ND(tagit_node_middle2_del), */

/* .leaf_scan_fd2 = F2ND(tagit_de_content2), */
/* .leaf_scan_fd2_deleted = F2ND(tagit_de_content2_del), */

  .leaf_scan2 = F2ND( tagit_leaf2 ),
/* .leaf_scan2_deleted = F2ND(tagit_leaf2_del), */

/* .dirent_file_scan_before2 = F2ND(tagit_de_file_before2), */
/* .dirent_dir_scan_before2 = F2ND(tagit_de_dir_before2), */

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* ########################################################################################## */

static
SR( MOD, tagit_init, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 0, "tagit_init %s", duf_levinfo_path( pdi ) );

  ER( MOD, tagit_init, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, tagit_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  if ( DUF_CONFIGG( vars.tag.file ) )
  {
#ifdef MAS_TRACING
    assert( !duf_levinfo_dfd( pdi ) );
  /* filename from db same as duf_levinfo_itemname( pdi ) */
    {
      const char *fn1;
      const char *fn2;

      fn1 = DUF_GET_RSFIELD2( fname );
      fn2 = duf_levinfo_itemtruename( pdi );
      assert( fn1 );
      assert( fn2 );
      assert( 0 == strcmp( fn1, fn2 ) );
    }
    assert( duf_levinfo_dbstat( pdi ) );
#endif

    duf_add_tag( pdi, "filename", duf_levinfo_nameid( pdi ), DUF_CONFIGG( vars.tag.file ) ? DUF_CONFIGG( vars.tag.file ) : "NONE", QPERRIND );

    MAST_TRACE( mod, 2, "@@tagit %s", duf_levinfo_path( pdi ) );
  }

  ER( MOD, tagit_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, tagit_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_RSFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }

  ER( MOD, tagit_node_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, tagit_node_middle2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_RSFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit node middle: %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }

  ER( MOD, tagit_node_middle2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, tagit_node_after2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  if ( DUF_CONFIGG( vars.tag.dir ) )
  {
    MAST_TRACE( mod, 1, "(%s:%s)tagit %s : %s", DUF_CONFIGG( vars.tag.dir ), DUF_CONFIGG( vars.tag.file ), duf_levinfo_path( pdi ),
                duf_levinfo_itemshowname( pdi ) );

#if 0
# ifdef MAS_TRACING
    DUF_RSFIELD2( fname );
# endif
    MAST_TRACE( mod, 1, "tagit node after: %s : %s", duf_levinfo_path( pdi ), filename );
#endif
  }

  ER( MOD, tagit_node_after2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
