#define DUF_SQL_PDI_STMT
#include <string.h>

#include <assert.h>
#include <unistd.h>

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

  DUF_TRACE( mod, 3, "dummy scan_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dirent_content2( duf_sqlite_stmt_t * pstmt, int fd_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( filename, duf_levinfo_itemtruename( pdi ) ) );


/*
* 2: 0 [MOD    ]  47:dirent_content2                 :3.8916 :  dummy dirent /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  DUF_TRACE( mod, 2, "dummy dirent %s : %s : %s", duf_levinfo_path( pdi ), filename, duf_levinfo_itemshowname( pdi ) );
  assert( fd_unused == duf_levinfo_dfd( pdi ) );

  DEBUG_ENDR( r );
}

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#endif
/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( filename, duf_levinfo_itemtruename( pdi ) ) );


  DUF_TRACE( mod, 5, "dummy %s -a-", duf_levinfo_path( pdi ) );
  DUF_TRACE( mod, 2, "dummy %s : %s -b-", duf_levinfo_itemshowname( pdi ), filename );

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
scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 2, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#  endif
  DUF_TRACE( mod, 0, "dummy %s : %s", duf_levinfo_path( pdi ), filename );
#endif
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

  DUF_TRACE( mod, 2, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#  endif
  DUF_TRACE( mod, 0, "dummy node middle: %s : %s", duf_levinfo_path( pdi ), filename );
#endif

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

  DUF_TRACE( mod, 2, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( filename );
#  endif
  DUF_TRACE( mod, 0, "dummy node after: %s : %s", duf_levinfo_path( pdi ), filename );
#endif

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
dirent_dir_scan_before2( /* const char *fname_unused, const struct stat *pstat_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( pdi ) ) );

  /* pstat_unused equal to duf_levinfo_stat( pdi ) ? */
  {
    struct stat *st = duf_levinfo_stat( pdi );

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

  DUF_TRACE( scan, 0, "scan dirent - sub-directory scanned here" );

  DUF_TRACE( mod, 2, "dummy dirent dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  DEBUG_ENDR( r );
}

int
dirent_file_scan_before2( /* const char *fname_unused, const struct stat *pstat_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#if 0
  assert( 0 == strcmp( fname_unused, duf_levinfo_itemname( pdi ) ) );
  /* pstat_unused equal to duf_levinfo_stat( pdi ) ? */
  {
    struct stat *st = duf_levinfo_stat( pdi );

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

  DUF_TRACE( mod, 2, "dummy dirent file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

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
