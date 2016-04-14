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

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_stat.h"

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
/* #include "duf_sccb_row.h"                                            (* datarow_* ✗ *) */

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
#include "duf_sccb_structs.h"

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_mod_defs.h"
/* ########################################################################################## */
#include "duf_mod_types.h"
DUF_MOD_DECLARE_ALL_FUNCS( duf_dummy )
/* ########################################################################################## */
/* ########################################################################################## */
     duf_scan_callbacks_t duf_dummy_callbacks = {
       .title = "dummy",
       .name = "dummy",
       .def_opendir = 0,
       .init_scan = F2ND( dummy_init ),
#if 0
       .beginning_sql_seq = &sql_create_selected,
#else
       .beginning_sql_seq = &sql_update_selected,
#endif

       .node_scan_before2 = F2ND( dummy_node_before2 ),
       .node_scan_before2_deleted = F2ND( dummy_node_before2_del ),

       .node_scan_after2 = F2ND( dummy_node_after2 ),
       .node_scan_after2_deleted = F2ND( dummy_node_after2_del ),

       .node_scan_middle2 = F2ND( dummy_node_middle2 ),
       .node_scan_middle2_deleted = F2ND( dummy_node_middle2_del ),

       .leaf_scan_fd2 = F2ND( dummy_de_content2 ),
       .leaf_scan_fd2_deleted = F2ND( dummy_de_content2_del ),

       .leaf_scan2 = F2ND( dummy_leaf2 ),
       .leaf_scan2_deleted = F2ND( dummy_leaf2_del ),

       .dirent_file_scan_before2 = F2ND( dummy_de_file_before2 ),
       .dirent_dir_scan_before2 = F2ND( dummy_de_dir_before2 ),

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
       .use_std_leaf_set_num = 1,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .use_std_node_set_num = 1,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .std_leaf_set_name = "std-leaf-one",
       .std_node_set_name = "std-node-one",
     };

/* ########################################################################################## */

SR( MOD, dummy_init, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 0, "dummy_init %s", duf_levinfo_path( H_PDI ) );

  ER( MOD, dummy_init, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_de_content2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/* const struct stat *pst_file MAS_UNUSED = duf_levinfo_stat( H_PDI ); */
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );

/* filename from db same as duf_levinfo_itemname( H_PDI ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( H_PDI ) ) );
  assert( duf_levinfo_opened_dh( H_PDI ) > 0 );
  assert( duf_levinfo_stat( H_PDI ) );

#endif

/*
* 2: 0 [MOD    ]  47:dummy_de_content2                 :3.8916 :  dummy de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  MAST_TRACE( mod, 1, "dummy de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( H_PDI ), fname,
              0 == strcmp( duf_levinfo_itemshowname( H_PDI ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( H_PDI ), duf_levinfo_dfd( H_PDI ),
              duf_levinfo_source( H_PDI ), ( unsigned long long ) duf_levinfo_stat_dev( H_PDI ) );

  ER( MOD, dummy_de_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_de_content2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/* const struct stat *pst_file MAS_UNUSED = duf_levinfo_stat( H_PDI ); */
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );

/* filename from db same as duf_levinfo_itemname( H_PDI ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( H_PDI ) ) );
#endif

/*
* 2: 0 [MOD    ]  47:dummy_de_content2                 :3.8916 :  dummy de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  MAST_TRACE( mod, 0, "@dummy de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( H_PDI ), fname,
              0 == strcmp( duf_levinfo_itemshowname( H_PDI ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( H_PDI ), duf_levinfo_dfd( H_PDI ),
              duf_levinfo_source( H_PDI ), ( unsigned long long ) duf_levinfo_stat_dev( H_PDI ) );

  ER( MOD, dummy_de_content2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_leaf2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );

  assert( !duf_levinfo_dfd( H_PDI ) );
/* filename from db same as duf_levinfo_itemname( H_PDI ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( H_PDI ) ) );
  assert( duf_levinfo_dbstat( H_PDI ) );
#endif

  MAST_TRACE( mod, 4, "dummy %s : %s -a-", duf_levinfo_path( H_PDI ), fname );
  MAST_TRACE( mod, 2, "@dummy %s : %s -b- ::  {dfd:%d; source:%d} dev:x%llx; inode:%llu", duf_levinfo_itemshowname( H_PDI ), fname,
              duf_levinfo_dfd( H_PDI ), duf_levinfo_source( H_PDI ), ( unsigned long long ) duf_levinfo_dbstat_dev( H_PDI ),
              ( unsigned long long ) duf_levinfo_dbstat_inode( H_PDI ) );

  ER( MOD, dummy_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_leaf2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@@dummy %s : %s", duf_levinfo_path( H_PDI ), fname );
/* Never called (no deleted flag - didn't try to open !!) */
  assert( 0 );

  ER( MOD, dummy_leaf2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( H_PDI ), fname );
#endif

  ER( MOD, dummy_node_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_before2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@dummy node before: %s : %s", duf_levinfo_path( H_PDI ), fname );

  ER( MOD, dummy_node_before2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_middle2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "dummy node middle: %s : %s", duf_levinfo_path( H_PDI ), fname );
#endif

  ER( MOD, dummy_node_middle2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_middle2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@dummy node middle %s : %s", duf_levinfo_path( H_PDI ), fname );

  ER( MOD, dummy_node_middle2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_after2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  MAST_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

#if 0
# ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
# endif
  MAST_TRACE( mod, 1, "dummy node after: %s : %s", duf_levinfo_path( H_PDI ), fname );
#endif

  ER( MOD, dummy_node_after2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_node_after2_del, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#ifdef MAS_TRACING
  DUF_RSFIELD2( fname );
#endif
  MAST_TRACE( mod, 0, "@dummy node after %s : %s", duf_levinfo_path( H_PDI ), fname );

  ER( MOD, dummy_node_after2_del, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_de_dir_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( H_PDI ) ) );

/* pstat_unused equal to duf_levinfo_stat( H_PDI ) ? */
  {
    struct stat *st = duf_levinfo_stat( H_PDI );

    assert( st->st_dev == pstat_unused->st_dev );
    assert( st->st_ino == pstat_unused->st_ino );
    assert( st->st_mode == pstat_unused->st_mode );
    assert( st->st_nlink == pstat_unused->st_nlink );
    assert( st->st_uid == pstat_unused->st_uid );
    assert( st->st_gid == pstat_unused->st_gid );
    assert( st->st_rdev == pstat_unused->st_rdev );
    assert( st->st_size == pstat_unused->st_size );
    assert( st->st_blksize == pstat_unused->st_blksize );
    assert( st->st_blocks == pstat_unused->st_blocks );
  /* assert( st->st_atim == pstat_unused->st_atim ); */
  /* assert( st->st_mtim == pstat_unused->st_mtim ); */
  /* assert( st->st_ctim == pstat_unused->st_ctim ); */
    assert( 0 == memcmp( st, pstat_unused, sizeof( struct stat ) ) );
    assert( pstat_unused == st );
  }
#endif

  MAST_TRACE( scan, 3, "scan de - sub-directory scanned here %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  MAST_TRACE( mod, 1, "dummy de dir before: %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dummy_de_dir_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, dummy_de_file_before2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( H_PDI ) ) );
/* pstat_unused equal to duf_levinfo_stat( H_PDI ) ? */
  {
    struct stat *st = duf_levinfo_stat( H_PDI );

    assert( st->st_dev == pstat_unused->st_dev );
    assert( st->st_ino == pstat_unused->st_ino );
    assert( st->st_mode == pstat_unused->st_mode );
    assert( st->st_nlink == pstat_unused->st_nlink );
    assert( st->st_uid == pstat_unused->st_uid );
    assert( st->st_gid == pstat_unused->st_gid );
    assert( st->st_rdev == pstat_unused->st_rdev );
    assert( st->st_size == pstat_unused->st_size );
    assert( st->st_blksize == pstat_unused->st_blksize );
    assert( st->st_blocks == pstat_unused->st_blocks );
  /* assert( st->st_atim == pstat_unused->st_atim ); */
  /* assert( st->st_mtim == pstat_unused->st_mtim ); */
  /* assert( st->st_ctim == pstat_unused->st_ctim ); */
    assert( 0 == memcmp( st, pstat_unused, sizeof( struct stat ) ) );
    assert( pstat_unused == st );
  }
#endif

  MAST_TRACE( mod, 1, "dummy de file before: %s : %s", duf_levinfo_path( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );

  ER( MOD, dummy_de_file_before2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
