#include <string.h>

#include <unistd.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_pdi_ref.h"

#include "duf_mod_defs.h"

#include "duf_sccb_eval_std.h"


/* #include "duf_path2db.h" */
#include "duf_path2dirid.h"

#include "duf_maindb.h"


#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"

#define DUF_MOD_NAME dumplet
/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( dumplet )
/* ########################################################################################## */
     static duf_sql_sequence_t final_sql = /* */
     {
       .name = "final @ ...",
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
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

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
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
dumplet_init( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dumplet_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 );
  assert( duf_levinfo_stat( pdi ) );
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_content2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  DUF_TRACE( mod, 6, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( !duf_levinfo_dfd( pdi ) );
  assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );

  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_leaf2_del( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* Never called (no deleted flag - didn't try to open !!) */
  DUF_TRACE( mod, 4, "dumplet %s : %s -a-", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_before2_del( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_middle2_del( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node middle %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "dumplet %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_node_after2_del( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 10, "@dumplet node after %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_dir_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 8, "dumplet de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}

static int
dumplet_de_file_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 6, "dumplet de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
