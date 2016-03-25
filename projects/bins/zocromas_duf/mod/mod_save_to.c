/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include <fcntl.h>                                                   /* Definition of AT_* constants */
#include <errno.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/*  #include "duf_tracen_defs.h"  (*  T; TT; TR ♠  *) */
/*  #include "duf_errorn_defs.h"  (*  DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠  *) */

/*  #include "duf_start_end.h"  (*  DUF_STARTR ; DUF_ENDR ♠  *) */
/*  #include "duf_dodefs.h"  (*  DOR ♠  *) */

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t ✗ */
#include "duf_sccb_structs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

/* #include "duf_option_defs.h" */

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_print.h"
#include "duf_fileinfo.h"

#include "duf_tags.h"

/* ########################################################################################## */
DUF_MOD_DECLARE_ALL_FUNCS( duf_save_to )
/* ########################################################################################## */
/* ########################################################################################## */
     duf_scan_callbacks_t duf_save_to_callbacks = {
       .title = "save it to",
       .name = "save_to",
       .def_opendir = 1,
       .init_scan = duf_save_to_init,
#if 0
       .beginning_sql_seq = &sql_create_selected,
#else
       .beginning_sql_seq = &sql_update_selected,
#endif

       .node_scan_before2 = duf_save_to_node_before2,
       .node_scan_before2_deleted = duf_save_to_node_before2_del,

       .node_scan_after2 = duf_save_to_node_after2,
       .node_scan_after2_deleted = duf_save_to_node_after2_del,

       .node_scan_middle2 = duf_save_to_node_middle2,
       .node_scan_middle2_deleted = duf_save_to_node_middle2_del,

       .leaf_scan_fd2 = duf_save_to_de_content2,
       .leaf_scan_fd2_deleted = duf_save_to_de_content2_del,

       .leaf_scan2 = duf_save_to_leaf2,
       .leaf_scan2_deleted = duf_save_to_leaf2_del,

     /* .dirent_file_scan_before2 = duf_save_to_de_file_before2, */
     /* .dirent_dir_scan_before2 = duf_save_to_de_dir_before2, */

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
       .use_std_leaf_set_num = 1,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .use_std_node_set_num = 1,                                    /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
       .std_leaf_set_name = "std-leaf-one",
       .std_node_set_name = "std-node-one",
     };

/* ########################################################################################## */

static
SR( MOD, save_to_init, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

  MAST_TRACE( mod, 0, "save_to_init %s", duf_levinfo_path( pdi ) );

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_init, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

SR( MOD, copy_to, duf_depthinfo_t * pdi, const char *save_path )
{
/*   DUF_STARTR( r ) */ ;
  FILE *fw;
  FILE *fr;
  char *fpath = NULL;

  if ( *save_path == '/' )
  {
    ERRMAKE( PATH );
  }
  else if ( *save_path == '.' )
  {
    ERRMAKE( PATH );
  }
  else
  {
    fpath = mas_strdup( duf_levinfo_path_top( pdi ) );
  }
  fpath = mas_strcat_x( fpath, save_path );
  fpath = mas_strcat_x( fpath, "//" );
  fpath = mas_strcat_x( fpath, duf_levinfo_itemtruename( pdi ) );
  MAST_TRACE( mod, 0, "%s", fpath );
  fr = fopen( fpath, "r" );
  if ( fr )
  {
/* TODO : Compare 2 files; if NOT same - copy with another name */
    fclose( fr );
  /* ERRMAKE(PATH ); */
  }
  else
  {
    fw = fopen( fpath, "w" );
    if ( fw )
    {
      int fd;

      MAST_TRACE( mod, 0, "OPENED %s", fpath );

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
            MAST_TRACE( mod, 0, "READ %lu of %s", len, fpath );
            if ( lw != len )
              ERRMAKE( PATH );
          }
          while ( QNOERR && len > 0 );
        }
      }
      fclose( fw );
      if ( QNOERR )
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
        /* DUF_SHOW_ERROR( "(%d) errno:%d utimes :%s; name:'%s'", ry, errno, s ? s : serr, fpath ); */
          ERRMAKE_M( STATAT, "(%d) errno:%d utimes :%s; name:'%s'", ry, errno, s ? s : serr, fpath );
        }
      }
      else
      {
        unlink( fpath );
      }
    }
    else
    {
      MAST_TRACE( mod, 0, "NOT OPENED %s", fpath );
    }
  }
  mas_free( fpath );
/*  DUF_ENDR( r );*/
  ER( MOD, copy_to, duf_depthinfo_t * pdi, const char *save_path );
}

static
SR( MOD, save_to_de_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

/* const struct stat *pst_file MAS_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
  assert( duf_levinfo_opened_dh( pdi ) > 0 );
  assert( duf_levinfo_stat( pdi ) );

#endif
  {
    char *save_path = NULL;
    duf_fileinfo_t fi = { 0 };
#ifdef MAS_TRACING
  /* filename from db same as duf_levinfo_itemname( pdi ) */
    assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
#endif
    CR( fileinfo, pstmt, pdi, &fi );
    save_path =
            duf_sformat_file_info( pdi, &fi, 0 /* is_atty // color */ , DUF_CONFIGG( save.path ) /* duf_config->save.path */ , ( duf_pdi_scb_t ) NULL,
                                   ( duf_pdi_scb_t ) NULL, 0 /* max_width */ , NULL /* pslen pr / pok */ , NULL /* pwidth */ , NULL /* pover */  );
    MAST_TRACE( mod, 2, "@@@top  %s", duf_levinfo_path_top( pdi ) );
    MAST_TRACE( mod, 2, "@@save  %s%s", duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
    MAST_TRACE( mod, 2, "@to => %s", save_path );
    if ( *save_path == '/' )
    {
      ERRMAKE( PATH );
    }
    else if ( *save_path == '.' )
    {
      ERRMAKE( PATH );
    }
    else
    {
      char *sl;
      int nnsavp = 0;
      int rt = 0;

      sl = save_path;
      assert( duf_levinfo_dfd_top( pdi ) );
      while ( QNOERR && sl && *sl )
      {
        struct stat stdir;

        sl = strchr( sl, '/' );
        if ( sl )
          *sl = 0;
        MAST_TRACE( mod, 0, "%d. create => %s", nnsavp, save_path );
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
          ERRMAKE( PATH );

        nnsavp++;
      }
      CR( copy_to, pdi, save_path );
    }
    mas_free( save_path );
  }

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_de_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_de_content2_del, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

/* const struct stat *pst_file MAS_UNUSED = duf_levinfo_stat( pdi ); */
#ifdef MAS_TRACING

/* filename from db same as duf_levinfo_itemname( pdi ) */
  assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), duf_levinfo_itemtruename( pdi ) ) );
#endif

/*
* 2: 0 [MOD    ]  47:save_to_de_content2                 :3.8916 :  save_to de /home/mastar/big/misc/media/video/startrek-ng/log/ : 25060543.log
*/
/*  DUF_ENDR( r );*/
  ER( MOD, save_to_de_content2_del, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
/*  DUF_ENDR( r );*/
  ER( MOD, save_to_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_leaf2_del, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
#ifdef MAS_TRACING
  MAST_TRACE( mod, 1, "@@save_to %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( fname ) );
#endif
/* Never called (no deleted flag - didn't try to open !!) */
  assert( 0 );

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_leaf2_del, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_before2_del, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
#ifdef MAS_TRACING
  MAST_TRACE( mod, 0, "@save_to node before: %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( fname ) );
#endif

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_before2_del, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_middle2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_middle2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_middle2_del, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused MAS_UNUSED,
    duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_middle2_del, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_after2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_after2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_node_after2_del, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
#ifdef MAS_TRACING
  MAST_TRACE( mod, 0, "@save_to node after %s : %s", duf_levinfo_path( pdi ), DUF_GET_SFIELD2( fname ) );
#endif

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_node_after2_del, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_de_dir_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

  MAST_TRACE( mod, 1, "save_to de dir before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );
/*  DUF_ENDR( r );*/
  ER( MOD, save_to_de_dir_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, save_to_de_file_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;

  MAST_TRACE( mod, 1, "save_to de file before: %s : %s", duf_levinfo_path( pdi ), duf_levinfo_itemshowname( pdi ) );

/*  DUF_ENDR( r );*/
  ER( MOD, save_to_de_file_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
