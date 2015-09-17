#define DUF_SQL_PDI_STMT
#include <string.h>

#include <fcntl.h>              /* Definition of AT_* constants */
#include <errno.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#include <mastar/tools/mas_arg_tools.h>

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
       .name = "final @ ...",
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
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

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

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static int
save_to_init( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 0, "save_to_init %s", duf_levinfo_path( pdi ) );

  DEBUG_ENDR( r );
}

int
duf_copy_to( duf_depthinfo_t * pdi, const char *save_path )
{
  DEBUG_STARTR( r );
  FILE *fw;
  FILE *fr;
  char *fpath = NULL;

  if ( *save_path == '/' )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
  }
  else if ( *save_path == '.' )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
  }
  else
  {
    fpath = mas_strdup( duf_levinfo_path_top( pdi ) );
  }
  fpath = mas_strcat_x( fpath, save_path );
  fpath = mas_strcat_x( fpath, "//" );
  fpath = mas_strcat_x( fpath, duf_levinfo_itemtruename( pdi ) );
  DUF_TRACE( mod, 0, "%s", fpath );
  fr = fopen( fpath, "r" );
  if ( fr )
  {
/* TODO : Compare 2 files; if NOT same - copy with another name */
    fclose( fr );
    /* DUF_MAKE_ERROR( r, DUF_ERROR_PATH ); */
  }
  else
  {
    fw = fopen( fpath, "w" );
    if ( fw )
    {
      int fd;

      DUF_TRACE( mod, 0, "OPENED %s", fpath );

      fd = duf_levinfo_dfd( pdi );
      if ( fd > 0 )
      {
        int ry;

        ry = lseek( fd, 0, SEEK_SET );
        if ( ry >= 0 )
        {
          size_t len = 0;
          size_t lw = 0;

          do
          {
            char buffer[1024 * 1000];

            len = read( fd, buffer, sizeof( buffer ) );
            lw = fwrite( buffer, 1, len, fw );
            DUF_TRACE( mod, 0, "READ %lu of %s", len, fpath );
            if ( lw != len )
              DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
          }
          while ( DUF_NOERROR( r ) && len > 0 );
        }
      }
      fclose( fw );
      if ( DUF_NOERROR( r ) )
      {
        int ry;
        struct timeval times[2];

        memset( times, 0, sizeof( times ) );
        times[0].tv_sec = duf_levinfo_stat_asec( pdi );
        times[1].tv_sec = duf_levinfo_stat_msec( pdi );
        times[0].tv_usec = duf_levinfo_stat_ansec( pdi ) / 1000;
        times[1].tv_usec = duf_levinfo_stat_mnsec( pdi ) / 1000;

        /* chmod(); */
        ry = utimes( fpath, times );
        if ( ry < 0 )
        {
          char serr[1024] = "";
          char *s;

          s = strerror_r( errno, serr, sizeof( serr ) );
          DUF_SHOW_ERROR( "(%d) errno:%d utimes :%s; name:'%s'", ry, errno, s ? s : serr, fpath );
          DUF_MAKE_ERROR( r, DUF_ERROR_STATAT );
        }
      }
      else
      {
        unlink( fpath );
      }
    }
    else
    {
      DUF_TRACE( mod, 0, "NOT OPENED %s", fpath );
    }
  }
  mas_free( fpath );
  DEBUG_ENDR( r );
}

static int
save_to_de_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* const struct stat *pst_file DUF_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( DUF_GET_SFIELD2( filename ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 || duf_levinfo_item_deleted( pdi ) );
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
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    }
    else if ( *save_path == '.' )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
    }
    else
    {
      char *sl;
      int nn = 0;
      int rt = 0;

      sl = save_path;
      assert( duf_levinfo_dfd_top( pdi ) );
      while ( DUF_NOERROR( r ) && sl && *sl )
      {
        struct stat stdir;

        sl = strchr( sl, '/' );
        if ( sl )
          *sl = 0;
        DUF_TRACE( mod, 0, "%d. create => %s", nn, save_path );
        rt = fstatat( duf_levinfo_dfd_top( pdi ), save_path, &stdir, AT_NO_AUTOMOUNT | AT_SYMLINK_NOFOLLOW );
        if ( rt < 0 && errno == ENOENT )
        {
          rt = mkdirat( duf_levinfo_dfd_top( pdi ), save_path, S_IRWXU );
        }
        else
        {
        }
        if ( sl )
          *sl++ = '/';
        if ( rt < 0 )
          DUF_MAKE_ERROR( r, DUF_ERROR_PATH );

        nn++;
      }
      DOR( r, duf_copy_to( pdi, save_path ) );
    }
    mas_free( save_path );
  }

  DEBUG_ENDR( r );
}

static int
save_to_de_content2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
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
save_to_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_leaf2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
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
save_to_node_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_node_before2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_TRACE( mod, 0, "@save_to node before: %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( filename ) );
#endif


  DEBUG_ENDR( r );
}

static int
save_to_node_middle2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
save_to_node_middle2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

static int
save_to_node_after2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DEBUG_ENDR( r );
}

static int
save_to_node_after2_del( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#ifdef MAS_TRACING
  DUF_TRACE( mod, 0, "@save_to node after %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( filename ) );
#endif

  DEBUG_ENDR( r );
}

static int DUF_UNUSED
save_to_de_dir_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "save_to de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
  DEBUG_ENDR( r );
}

static int DUF_UNUSED
save_to_de_file_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "save_to de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

  DEBUG_ENDR( r );
}
