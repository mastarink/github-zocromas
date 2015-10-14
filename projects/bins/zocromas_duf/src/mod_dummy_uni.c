#include <string.h>

#include <unistd.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_levinfo_openclose.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_stat.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( dummy )
/* ########################################################################################## */
     static duf_sql_sequence_t final_sql = { /* */
       .name = "final @ ...",
       .done = 0,
       .sql = {
               NULL,
               }
     };

/* ########################################################################################## */

duf_scan_callbacks_t duf_dummy_callbacks = {
  .title = "dummy",
  .name = "dummy",
  .def_opendir = 0,
  .init_scan = dummy_init,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

  .node_scan_before2 = dummy_node_before2,
  .node_scan_before2_deleted = dummy_node_before2_del,

  .node_scan_after2 = dummy_node_after2,
  .node_scan_after2_deleted = dummy_node_after2_del,

  .node_scan_middle2 = dummy_node_middle2,
  .node_scan_middle2_deleted = dummy_node_middle2_del,

  .leaf_scan_fd2 = dummy_de_content2,
  .leaf_scan_fd2_deleted = dummy_de_content2_del,

  .leaf_scan2 = dummy_leaf2,
  .leaf_scan2_deleted = dummy_leaf2_del,

  .dirent_file_scan_before2 = dummy_de_file_before2,
  .dirent_dir_scan_before2 = dummy_de_dir_before2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
dummy_init( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "dummy_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
dummy_de_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 );
  assert( duf_levinfo_stat( pdi ) );

#endif


/*
* 2: 0 [MOD    ]  47:dummy_de_content2                 :3.8916 :  dummy de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  DUF_TRACE( mod, 1, "dummy de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( pdi ), fname,
             0 == strcmp( duf_levinfo_itemshowname( pdi ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( pdi ), duf_levinfo_dfd( pdi ),
             duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_stat_dev( pdi ) );

  DEBUG_ENDR( r );
}

static int
dummy_de_content2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
#endif


/*
* 2: 0 [MOD    ]  47:dummy_de_content2                 :3.8916 :  dummy de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  DUF_TRACE( mod, 0, "@dummy de %s : %s : %s {%d:%d} x%llx", duf_levinfo_path( pdi ), fname,
             0 == strcmp( duf_levinfo_itemshowname( pdi ), fname ) ? "«SAME»" : duf_levinfo_itemshowname( pdi ), duf_levinfo_dfd( pdi ),
             duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_stat_dev( pdi ) );

  DEBUG_ENDR( r );
}

static int
dummy_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

#ifdef MAS_TRACING
  DUF_SFIELD2( fname );

  assert( !duf_levinfo_dfd( pdi ) );
  /* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( fname, duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_dbstat( pdi ) );
#endif


  DUF_TRACE( mod, 4, "dummy %s : %s -a-", duf_levinfo_path( pdi ), fname );
  DUF_TRACE( mod, 2, "@dummy %s : %s -b- ::  {dfd:%d; source:%d} dev:x%llx; inode:%llu", duf_levinfo_itemshowname( pdi ), fname,
             duf_levinfo_dfd( pdi ), duf_levinfo_source( pdi ), ( unsigned long long ) duf_levinfo_dbstat_dev( pdi ),
             ( unsigned long long ) duf_levinfo_dbstat_inode( pdi ) );

  DEBUG_ENDR( r );
}

static int
dummy_leaf2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#endif
  DUF_TRACE( mod, 0, "@@dummy %s : %s", duf_levinfo_path( pdi ), fname );
  /* Never called (no deleted flag - didn't try to open !!) */
  assert( 0 );

  DEBUG_ENDR( r );
}

static int
dummy_node_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#  endif
  DUF_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( pdi ), fname );
#endif
  DEBUG_ENDR( r );
}

static int
dummy_node_before2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#endif
  DUF_TRACE( mod, 0, "@dummy node before: %s : %s", duf_levinfo_path( pdi ), fname );


  DEBUG_ENDR( r );
}

static int
dummy_node_middle2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#  endif
  DUF_TRACE( mod, 1, "dummy node middle: %s : %s", duf_levinfo_path( pdi ), fname );
#endif

  DEBUG_ENDR( r );
}

static int
dummy_node_middle2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#endif
  DUF_TRACE( mod, 0, "@dummy node middle %s : %s", duf_levinfo_path( pdi ), fname );

  DEBUG_ENDR( r );
}

static int
dummy_node_after2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "dummy %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

#if 0
#  ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#  endif
  DUF_TRACE( mod, 1, "dummy node after: %s : %s", duf_levinfo_path( pdi ), fname );
#endif

  DEBUG_ENDR( r );
}

static int
dummy_node_after2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_SFIELD2( fname );
#endif
  DUF_TRACE( mod, 0, "@dummy node after %s : %s", duf_levinfo_path( pdi ), fname );

  DEBUG_ENDR( r );
}

static int
dummy_de_dir_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
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

  DUF_TRACE( scan, 3, "scan de - sub-directory scanned here %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DUF_TRACE( mod, 1, "dummy de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  DEBUG_ENDR( r );
}

static int
dummy_de_file_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
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

  DUF_TRACE( mod, 1, "dummy de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
