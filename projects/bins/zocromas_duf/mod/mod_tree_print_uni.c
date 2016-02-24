/* #undef MAS_TRACING */
#include <string.h>

#include <mastar/error/mas_errors_maintenance.h>

# include "duf_maintenance_z.h"
# include "duf_maintenance_tracen.h"
# include "duf_maintenance_errors.h"

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF */

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_pdi_filters.h"
#include "duf_pdi_depth.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_count.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_option_defs.h"
#include "duf_print.h"
#include "duf_printb.h"

#include "duf_fileinfo.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_selected.h"

/* ########################################################################################## */
/* static int duf_sql_print_tree_prefix_uni( const duf_depthinfo_t * pdi ); */
static int duf_sql_print_tree_sprefix_uni( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, size_t * pwidth );

/* ########################################################################################## */
static int tree_node_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi );
static int tree_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */

duf_scan_callbacks_t duf_tree_callbacks = {
  .title = "tree print",
  .name = "tree",
  .init_scan = NULL,                                                 /* */
  .no_progress = 1,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif
/* .node_scan_before = tree_node_before, */
  .node_scan_before2 = tree_node_before2,
/* .leaf_scan = tree_leaf, */
  .leaf_scan2 = tree_leaf2,

/* for "tree" 1 is much better in following 2 fields; BUT TODO: try 2 and 1 - may be good?! */
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .use_std_node_set_num = 1,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-one",
/* XXX in this case using 1 for nodes for tree only - to calculate 'tree graphics' XXX */
};

/* extern duf_scan_callbacks_t tree_cbs __attribute__ ( ( alias( "duf_print_tree_callbacks" ) ) ); */

/* ########################################################################################## */

static int
tree_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
#if 0
  DUF_UFIELD2( dirid );
  DUF_SFIELD2( fname );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( filemode );
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( sha1id );
  DUF_UFIELD2( dataid );
  DUF_UFIELD2( md5sum1 );
  DUF_UFIELD2( md5sum2 );
  DUF_UFIELD2( mtime );
  DUF_UFIELD2( dev );
  DUF_UFIELD2( uid );
  DUF_UFIELD2( gid );
  DUF_UFIELD2( nlink );
  DUF_UFIELD2( inode );
  DUF_UFIELD2( exifid );
  DUF_UFIELD2( exifdt );
  DUF_SFIELD2( camera );
  DUF_UFIELD2( filenameid );
  DUF_UFIELD2( mimeid );
  DUF_SFIELD2( mime );
  DUF_UFIELD2( nsame );
  DUF_UFIELD2( nsame_md5 );
  DUF_UFIELD2( nsame_sha1 );
  DUF_UFIELD2( nsame_exif );
/* DUF_SFIELD( mtimef ); */
/* DUF_SFIELD( dowmtime ); */
/* DUF_SFIELD( monthmtime ); */
#endif
  {
#if 0
    duf_bformat_combo_t bformat = {                                  /* */
      .v.flag = {
                 .filename = 1,
                 .short_filename = 1,
                 .depth = 1,
                 .seq = 1,
               /* .seq_leaf = 1, */
               /* .dirid = 1, */
                 .dirid_space = 1,
                 .exifid = 1,
                 .exifdt = 1,
                 .camera = 1,
               /* .nameid = 1, */
                 .mime = 1,
                 .mimeid = 0,
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
                 .nfiles_space = 1,
                 .ndirs_space = 1,
#endif
                 .inode = 0,
                 .mode = 1,
                 .nlink = 1,
                 .user = 1,
                 .group = 1,
                 .filesize = 1,
                 .md5 = 1,
                 .md5id = 1,
               /* .sha1id = 1, */
                 .mtime = 1,
                 .dataid = 1,
                 .prefix = 1,
                 .suffix = 1,
                 },
      .nsame = 1,
      .nsame_md5 = 1,
      .nsame_sha1 = 1,
      .nsame_exif = 1,
    };
#endif

    duf_fileinfo_t fi = { 0 };
#if 0
    fi.nsame = nsame;
    fi.nsame_md5 = nsame_md5;
    fi.nsame_sha1 = nsame_sha1;
    fi.nsame_exif = nsame_exif;
    fi.dirid = dirid;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
    fi.st.st_dev = ( dev_t ) dev;
    fi.st.st_uid = ( uid_t ) uid;
    fi.st.st_gid = ( gid_t ) gid;
    fi.st.st_nlink = ( nlink_t ) nlink;
    fi.st.st_size = ( off_t ) filesize;
    fi.name = filename;
    fi.exifid = exifid;
    fi.exifdt = exifdt;
    fi.camera = camera;
    fi.nameid = filenameid;
    fi.mime = mime;
    fi.mimeid = mimeid;
    fi.md5id = md5id;
    fi.sha1id = sha1id;
    fi.dataid = dataid;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;
#else
    DOR( r, duf_fileinfo( pstmt, pdi, &fi ) );
#endif

#if 0
    if ( duf_output_use_binformat(  ) )
    {
      if ( duf_print_bformat_file_info( pdi, &fi, &bformat, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
        DUF_PUTSL( 0 );
      else
        DUF_PUTS( 0, "????????????" );
    }
    else
#endif
    {
      const char *sformat_pref = NULL;
      const char *sformat = NULL;
      size_t slen = 0;
      size_t rwidth = 0;
      int over = 0;

      {
        sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_gen_tree );
        if ( !sformat_pref )
          sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_files_tree );

        if ( !sformat_pref )
          sformat_pref = "_%-6M =%-4S%P";
#if 0
        {
          int c1 DUF_UNUSED;
          int c2 DUF_UNUSED;

          c1 = duf_output_force_color(  );
          c2 = duf_output_nocolor(  );
          T( "%d:%d", c1, c2 );
          assert( 0 );
        }
#endif
        if ( duf_config->opt.output.max_width == 0 || duf_config->opt.output.max_width > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat_pref, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL,
                                              duf_config->opt.output.max_width, duf_output_force_color(  ), duf_output_nocolor(  ), &rwidth, &over );
      }
      if ( !over )
      {
        {
          int use;
          duf_filedirformat_t *fmt;

#if 0
          use = DUF_CONFIGG( opt.output.as_formats.use ) - 1;
#else
          use = duf_pdi_pu( pdi )->use_format - 1;
#endif
          fmt = DUF_CONFIGA( opt.output.as_formats.tree );
          DUF_TRACE( temp, 5, "use:%d; files.argc:%d", use, fmt->files.argc );
          if ( use >= 0 && use < fmt->files.argc && !sformat )
            sformat = fmt->files.argv[use];
          DUF_TRACE( temp, 5, "sformat A: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.files_gen );
          DUF_TRACE( temp, 5, "sformat B: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.files_tree );
          DUF_TRACE( temp, 5, "sformat C: %s", sformat );
        }
        if ( !sformat )
          sformat = "%f\n";

        if ( duf_config->opt.output.max_width == 0 || duf_config->opt.output.max_width > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL,
                                              duf_config->opt.output.max_width, duf_output_force_color(  ), duf_output_nocolor(  ), &rwidth, &over );
      }
      DUF_PUTSL( 0 );
    }
  }

/* SQL at duf_scan_files_by_dirid */

/* DUF_PRINTF( 0, "%s", filename ); */

  DUF_ENDR( r );
}

static int
tree_node_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
/* if ( duf_levinfo_count_gfiles( pdi ) ) */
  {
#if 0
    duf_bformat_combo_t bformat = {                                  /* */
      .v.flag = {
                 .filename = 1,
                 .short_filename = 1,
                 .depth = 1,
                 .seq = 1,
                 .dirid = 1,
                 .exifid = 0,
                 .exifdt = 0,
                 .camera = 0,
                 .mimeid = 0,
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
                 .nfiles = 1,
                 .ndirs = 1,
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
                 .rnfiles = 1,
                 .rndirs = 1,
#endif
                 .inode = 0,
                 .mode = 0,
                 .nlink = 0,
                 .user = 0,
                 .group = 0,
                 .filesize = 0,
                 .md5 = 0,
                 .md5id = 1,
               /* .sha1id = 1, */
                 .mtime = 0,
                 .prefix = 1,
                 .suffix = 1,
                 }
      ,
      .nsame = 1,
      .nsame_md5 = 1,
      .nsame_sha1 = 1,
      .nsame_exif = 1,
    };
#endif
    duf_fileinfo_t fi = { 0 };
  /* fi.nsame = nsame; */
  /* fi.nsame_md5 = nsame_md5;   */
  /* fi.nsame_sha1 = nsame_sha1; */
  /* fi.nsame_exif = nsame_exif; */
  /* fi.st.st_mode = ( mode_t ) filemode; */
  /* fi.st.st_ino = ( ino_t ) inode;      */
  /* fi.st.st_mtim.tv_sec = mtime;        */
  /* fi.st.st_mtim.tv_nsec = 0;           */
  /* fi.st.st_uid = ( uid_t ) uid;        */
  /* fi.st.st_gid = ( gid_t ) gid;        */
  /* fi.st.st_nlink = ( nlink_t ) nlink;  */
  /* fi.st.st_size = ( off_t ) filesize; */
    fi.name = duf_levinfo_itemshowname( pdi );
  /* fi.md5id = md5id; */
  /* fi.sha1id = sha1id; */
  /* fi.md5sum1 = md5sum1; */
  /* fi.md5sum2 = md5sum2; */

#if 0
    if ( duf_output_use_binformat(  ) )
    {
      if ( duf_print_bformat_file_info( pdi, &fi, &bformat, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
        DUF_PUTSL( 0 );
      else
        DUF_PUTS( 0, "????????????" );
    }
    else
#endif
    {
      const char *sformat_pref = NULL;
      const char *sformat = NULL;
      size_t slen = 0;
      size_t rwidth = 0;
      int over = 0;

    /* if ( duf_levinfo_count_gfiles( pdi ) )                  */
    /*   T( "@gfiles:%llu", duf_levinfo_count_gfiles( pdi ) ); */

      {
        sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_gen_tree );
        if ( !sformat_pref )
          sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_dirs_tree );

        if ( !sformat_pref )
          sformat_pref = " %6s  %4s%P";
        if ( duf_config->opt.output.max_width == 0 || duf_config->opt.output.max_width > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat_pref, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL,
                                              duf_config->opt.output.max_width, duf_output_force_color(  ), duf_output_nocolor(  ), &rwidth, &over );
      }
      if ( !over )
      {
        {
          int use;
          duf_filedirformat_t *fmt;

#if 0
          use = DUF_CONFIGG( opt.output.as_formats.use ) - 1;
#else
          use = duf_pdi_pu( pdi )->use_format - 1;
#endif
          fmt = DUF_CONFIGA( opt.output.as_formats.tree );
          DUF_TRACE( temp, 5, "use:%d; dirs.argc:%d", use, fmt->dirs.argc );
          if ( use >= 0 && use < fmt->dirs.argc && !sformat )
            sformat = fmt->dirs.argv[use];
          DUF_TRACE( temp, 5, "sformat A: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.dirs_gen );
          DUF_TRACE( temp, 5, "sformat B: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.dirs_tree );
          DUF_TRACE( temp, 5, "sformat C: %s", sformat );
        }

        if ( !sformat )
          sformat = "%f\n";
        if ( duf_config->opt.output.max_width == 0 || duf_config->opt.output.max_width > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL,
                                              duf_config->opt.output.max_width, duf_output_force_color(  ), duf_output_nocolor(  ), &rwidth, &over );
      }
      DUF_PUTSL( 0 );
    }
  }

  DUF_ENDR( r );
}

/* 20151113.132638 */
static int
duf_sql_print_tree_sprefix_uni_d( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, int d0, int maxd, int d )
{
  DUF_STARTR( r );
/* ━ │ ┃ ┄ ┅ ┆ ┇ ┈ ┉ ┊ ┋ ┌ ┍ ┎ ┏ ┐ ┑ ┒ ┓ └ ┕ ┖ ┗ ┘ ┙                                 */
/* ┚ ┛ ├ ┝ ┞ ┟ ┠ ┡ ┢ ┣ ┤ ┥ ┦ ┧ ┨ ┩ ┪ ┫ ┬ ┭ ┮ ┯ ┰ ┱ ┲                                 */
/* ┳ ┴ ┵ ┶ ┷ ┸ ┹ ┺ ┻ ┼ ┽ ┾ ┿ ╀ ╁ ╂ ╃ ╄ ╅ ╆ ╇ ╈ ╉ ╊ ╋                                 */
/* ╌ ╍ ╎ ╏ ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟ ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ ╬ */
/* ╭ ╮ ╯ ╰ ╱ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿                                                         */
/* ₴ ☑ ♒ */
/* ⬅⬇⬆➡🔜  🆘  🐰 */
/* ╔═╦══╤╗ ┏━┳━━┯┓ ┌─┰──┬┐ ╲    ╲   ╱    ╱   ╭─────╮
 * ║ ║  │║ ┃ ┃  │┃ │ ┃  ││  ╲    ╲ ╱    ╱    │     │       ╷
 * ╠═╬══╪╣ ┣━╋━━┿┫ ┝━╋━━┿┥   ╲    ╳    ╱     │     │      ╶┼╴
 * ╟─╫──┼╢ ┠─╂──┼┨ ├─╂──┼┤    ╲  ╱ ╲  ╱      ╰─────╯       ╵
 * ╚═╩══╧╝ ┗━┻━━┷┛ └─┸──┴┘     ╲╱   ╲╱              
 * */

  unsigned flags = 0;
  int du = d - 1;

  long ndu = 0;
  long nchild = 0;
  int is_leaf = duf_levinfo_is_leaf_d( pdi, d );

#define DUF_TREE_SPACE     " "
#define DUF_TREE_HLINE     "─"
#define DUF_TREE_VRLINE2   "╞"
#define DUF_TREE_VLINE     "│"
#define DUF_TREE_VRLINE    "├"
#define DUF_TREE_URLINE    "└"
#define DUF_TREE_PRE_NODE DUF_TREE_SPACE
#define DUF_TREE_PRE_LEAF DUF_TREE_SPACE
#define DUF_TREE_PRE_INTER DUF_TREE_SPACE

#if 0
# define DUF_TREE_FLAG_NONE 0x0
# define DUF_TREE_FLAG_CONTINUE 0x1
# define DUF_TREE_FLAG_HERE 0x10
# define DUF_TREE_FLAG_LEAF 0x4
# define DUF_TREE_FLAG_TOO_DEEP 0x20
#else
# define DUF_TREE_FLAG_NONE 0x0
# define DUF_TREE_FLAG_CONTINUE 0x1
# define DUF_TREE_FLAG_HERE 0x2
# define DUF_TREE_FLAG_LEAF 0x4
# define DUF_TREE_FLAG_TOO_DEEP 0x8
#endif

#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
  ndu = duf_levinfo_numdir_d( pdi, du );
#else
  ndu = duf_levinfo_count_childs_d( pdi, du );
  nchild = duf_levinfo_numchild_d( pdi, du );
  ndu -= nchild;
#endif
#if 0
  if ( ndu > 0 && d > d0 )
    flags |= DUF_TREE_FLAG_CONTINUE;                                 /* set continue */
# if 0
  if ( ndu < 0 && d > d0 )
  {
    flags |= 0x2;
    assert( 0 );
  }
# endif
  if ( is_leaf )
    flags |= DUF_TREE_FLAG_LEAF;                                     /* set isleaf */
  if ( d == maxd )
    flags |= DUF_TREE_FLAG_HERE;                                     /* set node */

  if ( !duf_pdi_is_good_depth_d( pdi, is_leaf ? 1 : 0, d ) )
    flags |= DUF_TREE_FLAG_TOO_DEEP;
#else
  flags = ( d == maxd ? DUF_TREE_FLAG_HERE : 0 ) |                   /* */
          ( is_leaf ? DUF_TREE_FLAG_LEAF : 0 ) |                     /* */
          ( ndu > 0 && d > d0 ? DUF_TREE_FLAG_CONTINUE : 0 ) |       /* */
          ( duf_pdi_is_good_depth_d( pdi, is_leaf, d ) ? 0 : DUF_TREE_FLAG_TOO_DEEP );
#endif

  DUF_PRINTF( 1, ".@@%1ld/%1ld*%d;", ndu, nchild, flags & DUF_TREE_FLAG_CONTINUE ? 1 : 0 ); /* */

#if 0
  {
    int l;

    snprintf( pbuffer, bfsz, "%03lu", ndu );

    l = strlen( pbuffer );
    pbuffer += l;
    bfsz -= l;
  }
#endif
/*
 * 1.
 *   ' ' 
 *   '│'  DUF_TREE_FLAG_CONTINUE ; !DUF_TREE_FLAG_HERE
 *   '│'  DUF_TREE_FLAG_CONTINUE ;  DUF_TREE_FLAG_HERE ;  DUF_TREE_FLAG_LEAF
 *   '├'  DUF_TREE_FLAG_CONTINUE ;  DUF_TREE_FLAG_HERE ; !DUF_TREE_FLAG_LEAF
 *   '└' !DUF_TREE_FLAG_CONTINUE ;  DUF_TREE_FLAG_HERE
 * 2,3,4.
 *   ' ' !DUF_TREE_FLAG_HERE
 *   ' '  DUF_TREE_FLAG_HERE ;  DUF_TREE_FLAG_LEAF
 *   '─'  DUF_TREE_FLAG_HERE ; !DUF_TREE_FLAG_LEAF
 *
 * */
#if 1
  {
    const char *s1 = NULL;
    const char *s2 = NULL;

    if ( flags & DUF_TREE_FLAG_HERE )
    {
# if 1
      if ( flags & DUF_TREE_FLAG_CONTINUE )
        s1 = flags & DUF_TREE_FLAG_LEAF ? DUF_TREE_PRE_LEAF DUF_TREE_VLINE : DUF_TREE_PRE_NODE DUF_TREE_VRLINE;
      else
        s1 = flags & DUF_TREE_FLAG_LEAF ? DUF_TREE_PRE_LEAF DUF_TREE_SPACE : DUF_TREE_PRE_NODE DUF_TREE_URLINE;
# else
      if ( flags & DUF_TREE_FLAG_CONTINUE )
        s1 = flags & DUF_TREE_FLAG_LEAF ? DUF_TREE_PRE_LEAF DUF_TREE_VRLINE2 : DUF_TREE_PRE_NODE DUF_TREE_VRLINE;
      else
        s1 = flags & DUF_TREE_FLAG_LEAF ? DUF_TREE_PRE_LEAF DUF_TREE_SPACE : DUF_TREE_PRE_NODE DUF_TREE_URLINE;
# endif
    }
    else
    {
      if ( flags & DUF_TREE_FLAG_CONTINUE )
        s1 = DUF_TREE_PRE_INTER DUF_TREE_VLINE;
      else
        s1 = DUF_TREE_PRE_INTER DUF_TREE_SPACE;
    }

    if ( flags & DUF_TREE_FLAG_LEAF )
      s2 = DUF_TREE_SPACE;
    else if ( ( flags & DUF_TREE_FLAG_HERE ) )
      s2 = DUF_TREE_HLINE DUF_TREE_HLINE DUF_TREE_SPACE;
    else
      s2 = DUF_TREE_SPACE DUF_TREE_SPACE DUF_TREE_SPACE;
    strncpy( pbuffer, s1, bfsz );
    strncat( pbuffer, s2, bfsz );
  }
#elif 0
  if ( flags & DUF_TREE_FLAG_CONTINUE )
  {
    if ( ( flags & DUF_TREE_FLAG_HERE ) && !( flags & DUF_TREE_FLAG_LEAF ) )
      strncpy( pbuffer, "├", bfsz );
    else
      strncpy( pbuffer, "│", bfsz );
  }
  else
  {
    if ( ( flags & DUF_TREE_FLAG_HERE ) && !( flags & DUF_TREE_FLAG_LEAF ) )
      strncpy( pbuffer, "└", bfsz );
    else
      strncpy( pbuffer, " ", bfsz );
  }
  if ( ( flags & DUF_TREE_FLAG_HERE ) && !( flags & DUF_TREE_FLAG_LEAF ) )
    strncat( pbuffer, "─── ", bfsz );
  else if ( flags & DUF_TREE_FLAG_LEAF )
    strncat( pbuffer, "  ", bfsz );
  else
    strncat( pbuffer, "    ", bfsz );
#else
  switch ( flags )
  {
  case DUF_TREE_FLAG_NONE:
    strncpy( pbuffer, "     ", bfsz );
    break;
  case DUF_TREE_FLAG_CONTINUE /*0x01 */ :
    strncpy( pbuffer, "│    ", bfsz );
    break;
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_LEAF /*0x14 */ :
  /* here: most right; leaf: file */
    strncpy( pbuffer, "  ", bfsz );
    break;
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_LEAF | DUF_TREE_FLAG_CONTINUE /*0x15 */ :
  /* continue + !node + isleaf */
  /* strncpy(pbuffer, "│ → ", bfsz ); */
    strncpy( pbuffer, "│ ", bfsz );
    break;
  case DUF_TREE_FLAG_HERE /*0x10 */ :
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_TOO_DEEP /*0x30 */ :
    strncpy( pbuffer, "└─── ", bfsz );
    break;
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_CONTINUE /*0x11 */ :
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_CONTINUE | DUF_TREE_FLAG_TOO_DEEP /*0x31 */ :
    strncpy( pbuffer, "├─── ", bfsz );
    break;
# if 0
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_LEAF | DUF_TREE_FLAG_TOO_DEEP /*0x34 */ :
    strncpy( pbuffer, "  ", bfsz );
    assert( 0 );
    break;
  case DUF_TREE_FLAG_HERE | DUF_TREE_FLAG_LEAF | DUF_TREE_FLAG_TOO_DEEP | DUF_TREE_FLAG_CONTINUE /*0x35 */ :
  /* !continue + !node + !isleaf */
  /* strncpy(pbuffer, "  → ", bfsz ); */
    strncpy( pbuffer, "  ", bfsz );
    assert( 0 );
    break;
  case DUF_TREE_FLAG_CONTINUE | DUF_TREE_FLAG_TOO_DEEP /*0x21 */ :
    strncpy( pbuffer, "│    ", bfsz );
    assert( 0 );
    break;
  case DUF_TREE_FLAG_TOO_DEEP /*0x20 */ :
    strncpy( pbuffer, "     ", bfsz );
    assert( 0 );
    break;
# endif
  default:
    snprintf( pbuffer, bfsz, " [%02x]", flags );
    break;
  }
#endif

  DUF_ENDR( r );
}

/* 20151113.132643 */
static int
duf_sql_print_tree_sprefix_uni( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, size_t * pwidth DUF_UNUSED )
{
  DUF_STARTR( r );

  int d0 = duf_pdi_topdepth( pdi );
  int maxd = duf_pdi_depth( pdi );

  if ( d0 == 0 )
    d0 = 1;
  for ( int d = d0; DUF_NOERROR( r ) && d <= maxd; d++ )
  {
    DOR( r, duf_sql_print_tree_sprefix_uni_d( pbuffer, bfsz, pdi, d0, maxd, d ) );
    {
      size_t len = strlen( pbuffer );

      pbuffer += len;
      bfsz -= len;
    }
  }
  *pbuffer = 0;
  DUF_PRINTF( 1, "@%03d", r );

  DUF_ENDR( r );
}

#if 0
static int
duf_sql_print_tree_prefix_uni( const duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
  char buffer[1024];

  duf_sql_print_tree_sprefix_uni( buffer, sizeof( buffer ), pdi, NULL /* pwidth */  );
  DUF_WRITES( 0, buffer );
  DUF_ENDR( r );
}
#endif
