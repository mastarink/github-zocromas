#define DUF_SQL_PDI_STMT
#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ########################################################################################## */

static int
scan_init( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dummy scan_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi );

  DUF_TRACE( mod, 0, "dummy dirent %s : %s", duf_levinfo_path( pdi ), filename );
  assert( fd == duf_levinfo_dfd( pdi ) );

  DEBUG_ENDR( r );
}

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_leaf2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_before2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy node before: %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy node middle: %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_middle2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy node middle %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

static int
scan_node_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy node after: %s : %s", duf_levinfo_path( pdi ), filename );


  DEBUG_ENDR( r );
}

static int
scan_node_after2_deleted( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
  DUF_TRACE( mod, 0, "dummy node after %s : %s", duf_levinfo_path( pdi ), filename );

  DEBUG_ENDR( r );
}

int
dirent_dir_scan_before2( const char *fname, const struct stat *pstat, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 0, "dummy dirent dir before: %s : %s", duf_levinfo_path( pdi ), fname );


  DEBUG_ENDR( r );
}

int
dirent_file_scan_before2( const char *fname, const struct stat *pstat, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 0, "dummy dirent file before: %s : %s", duf_levinfo_path( pdi ), fname );


  DEBUG_ENDR( r );
}

static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {


          NULL,
          }
};


duf_scan_callbacks_t duf_dummy_callbacks = {
  .title = "dummy",
  .name = "dummy",
  .def_opendir = 0,
  .init_scan = scan_init,
  .beginning_sql_seq = &sql_beginning_selected,

  .node_scan_before2 = scan_node_before2,
  .node_scan_before2_deleted = scan_node_before2_deleted,

  .node_scan_after2 = scan_node_after2,
  .node_scan_after2_deleted = scan_node_after2_deleted,

  .node_scan_middle2 = scan_node_middle2,
  .node_scan_middle2_deleted = scan_node_middle2_deleted,

  .leaf_scan_fd2 = dirent_content2,

  .leaf_scan2 = scan_leaf2,
  .leaf_scan2_deleted = scan_leaf2_deleted,

  .dirent_file_scan_before2 = dirent_file_scan_before2,
  .dirent_dir_scan_before2 = dirent_dir_scan_before2,

  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {
           .fieldset = NULL,    /* */
           .selector2 = NULL,   /* */
           .selector_total2 = NULL, /* */
           },
  .node = {
           .fieldset = NULL,    /* */
           .selector2 = NULL    /* */
           ,
           .selector_total2 =   /* */
           " /* dum */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
           },
  .final_sql_seq = &final_sql,
};
