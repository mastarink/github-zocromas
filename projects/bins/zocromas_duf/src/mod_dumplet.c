#define DUF_SQL_PDI_STMT
#include <string.h>

#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"

#include "duf_mod_defs.h"

/* #include "duf_path2db.h" */
#include "duf_path2dirid.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( dumplet )
/* ########################################################################################## */
     static duf_sql_sequence_t final_sql = { /* */
       .done = 0,
       .sql = {
               NULL,
               }
     };

/* ########################################################################################## */

duf_scan_callbacks_t duf_dumplet_callbacks = {
  .title = "dumplet",
  .name = "dumplet",
  .def_opendir = 0,
  .init_scan = dumplet_init,
  .beginning_sql_seq = &sql_beginning_selected,

  .node_scan_before2 = dumplet_node_before2,
  .node_scan_before2_deleted = dumplet_node_before2_del,

  .node_scan_after2 = dumplet_node_after2,
  .node_scan_after2_deleted = dumplet_node_after2_del,

  .node_scan_middle2 = dumplet_node_middle2,
  .node_scan_middle2_deleted = dumplet_node_middle2_del,

  .leaf_scan_fd2 = dumplet_de_content2,
  .leaf_scan_fd2_deleted = dumplet_de_content2_del,

  .leaf_scan2 = dumplet_leaf2,
  .leaf_scan2_deleted = dumplet_leaf2_del,

  .dirent_file_scan_before2 = dumplet_de_file_before2,
  .dirent_dir_scan_before2 = dumplet_de_dir_before2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {
           .fieldset = NULL,    /* */
           .selector2 = NULL,   /* */
#if 0
           .selector_total2 = NULL, /* */
#endif
           },
  .node = {
           .fieldset = NULL,    /* */
           .selector2 = NULL    /* */
           ,
#if 0
           .selector_total2 =   /* */
           " /* dum */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
dumplet_init( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dumplet_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) || duf_levinfo_item_deleted( pdi ) );
  assert( duf_levinfo_stat( pdi ) || duf_levinfo_item_deleted( pdi ) );
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  DUF_TRACE( mod, 6, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( !duf_levinfo_dfd( pdi ) );
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );

  {
    duf_depthinfo_t DUF_UNUSED di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .pu = NULL,
      .opendir = 1,
    };
    char *path = NULL;

    DOR( r, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL, 0 /* caninsert */ , NULL /* node_selector2 */ , 1 /* recursive */ , 0 /* opendir */  ) );
    path = duf_dirid2path( &di, duf_levinfo_dirid( pdi ), &r );
    DUF_TRACE( mod, 2, "@@@dumplet       %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
    DUF_TRACE( mod, 2, "@@@dumplet id2p: %s", path );
    mas_free( path );
    path = NULL;
    duf_pdi_shut( &di );
    assert( 0 );
  }

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* Never called (no deleted flag - didn't try to open !!) */
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node middle %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node after %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_dir_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 8, "dumplet de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_file_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 6, "dumplet de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
