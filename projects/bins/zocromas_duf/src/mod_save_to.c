#define DUF_SQL_PDI_STMT
#include <string.h>

#include <assert.h>

#include <fcntl.h>              /* Definition of AT_* constants */
#include <errno.h>
#include <sys/stat.h>



#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_option_defs.h"

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"

#include "duf_print.h"
#include "duf_fileinfo.h"

#include "duf_tags.h"

/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( save_to )
/* ########################################################################################## */
     static duf_sql_sequence_t final_sql = { /* */
       .done = 0,
       .sql = {


               NULL,
               }
     };

/* ########################################################################################## */

duf_scan_callbacks_t duf_save_to_callbacks = {
  .title = "save it to",
  .name = "save_to",
  .def_opendir = 1,
  .init_scan = save_to_init,
  .beginning_sql_seq = &sql_beginning_selected,

  .node_scan_before2 = save_to_node_before2,
  .node_scan_before2_deleted = save_to_node_before2_del,

  .node_scan_after2 = save_to_node_after2,
  .node_scan_after2_deleted = save_to_node_after2_del,

  .node_scan_middle2 = save_to_node_middle2,
  .node_scan_middle2_deleted = save_to_node_middle2_del,

  .leaf_scan_fd2 = save_to_de_content2,
  .leaf_scan_fd2_deleted = save_to_de_content2_del,

  .leaf_scan2 = save_to_leaf2,
  .leaf_scan2_deleted = save_to_leaf2_del,

  /* .dirent_file_scan_before2 = save_to_de_file_before2, */
  /* .dirent_dir_scan_before2 = save_to_de_dir_before2, */

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
save_to_init( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "save_to_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

static int
save_to_de_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) || duf_levinfo_item_deleted( pdi ) );
  assert( duf_levinfo_stat( pdi ) || duf_levinfo_item_deleted( pdi ) );

#endif
  {
    char *save_path = NULL;
    duf_fileinfo_t fi = { 0 };
#ifdef MAS_TRACING
    /* filename from db same as duf_levinfo_itemname( pdi ) */
    assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
#endif
    DOR( r, duf_fileinfo( pstmt, pdi, &fi ) );
    save_path = duf_sformat_file_info( pdi, &fi, duf_config->save.path, ( duf_pdi_scb_t ) NULL, ( duf_pdi_scb_t ) NULL, NULL /* pr / pok */  );
    DUF_TRACE( mod, 2, "@@@top  %s", duf_levinfo_path_top( pdi ) );
    DUF_TRACE( mod, 2, "@@save  %s%s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
    DUF_TRACE( mod, 2, "@to => %s", save_path );
    if ( *save_path == '/' )
    {
    }
    else if ( *save_path == '.' )
    {
    }
    else
    {
      char *sl;
      int nn = 0;
      int rt = 0;

      sl = save_path;
      assert( duf_levinfo_dfd_top( pdi ) );
      while ( r >= 0 && sl && *sl && ( sl = strchr( sl, '/' ) ) )
      {
        struct stat stdir;

        *sl = 0;
        DUF_TRACE( mod, 2, "%d. create => %s", nn, save_path );
        rt = fstatat( duf_levinfo_dfd_top( pdi ), save_path, &stdir, AT_NO_AUTOMOUNT | AT_SYMLINK_NOFOLLOW );
        if ( rt < 0 && errno == ENOENT )
        {
          rt = mkdirat( duf_levinfo_dfd_top( pdi ), save_path, S_IRWXU );
        }
        else
        {
        }
        *sl++ = '/';
        if ( rt < 0 )
          r = DUF_ERROR_PATH;
        nn++;
      }
    }
    mas_free( save_path );
  }

  DEBUG_ENDR( r );
}

static int
save_to_de_content2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
#endif


/*
* 2: 0 [MOD    ]  47:save_to_de_content2                 :3.8916 :  save_to de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
  DEBUG_ENDR( r );
}

static int
save_to_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_leaf2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_TRACE( mod, 1, "@@save_to %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( filename ) );
#endif
  /* Never called (no deleted flag - didn't try to open !!) */
  assert( 0 );

  DEBUG_ENDR( r );
}

static int
save_to_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_node_before2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_TRACE( mod, 0, "@save_to node before: %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( filename ) );
#endif


  DEBUG_ENDR( r );
}

static int
save_to_node_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
save_to_node_middle2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_node_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
save_to_node_after2_del( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_TRACE( mod, 0, "@save_to node after %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( filename ) );
#endif

  DEBUG_ENDR( r );
}

static int DUF_UNUSED
save_to_de_dir_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "save_to de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  DEBUG_ENDR( r );
}

static int DUF_UNUSED
save_to_de_file_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "save_to de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
