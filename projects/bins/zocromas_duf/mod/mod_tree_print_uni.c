#define DUF_GET_FIELD_FROM_ROW
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>


#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"                                  /* mas_output_force_color ; mas_output_nocolor */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_depth.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_count.h"

#include "duf_sccb_structs.h"
/* #include "duf_sccb_row_field_defs.h"                                 (* DUF_*FIELD2* ✗ *)                                    */
/* #include "duf_sccb_row.h"                                            (* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ *) */
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_print.h"

#include "duf_fileinfo.h"

#include "sql_beginning_selected.h"

/* ########################################################################################## */
/* static int duf_sql_print_tree_prefix_uni( const duf_depthinfo_t * pdi ); */
static int duf_sql_print_tree_sprefix_uni( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, size_t * pwidth );

/* ########################################################################################## */
static int duf_tree_node_before2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );
static int duf_tree_leaf2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */

static duf_scanner_set_t scanners[] = {
  {
   .disabled = 0,                                                    /* */
   .type = DUF_NODE_NODE,                                            /* */
   .scanstage = DUF_SCANSTAGE_NODE_BEFORE | DUF_SCANSTAGE_DB_LEAVES, /* */
   .to_open = 0,                                                     /* */
   .dirent = 0,                                                      /* */
   .db = 1,                                                          /* */
   .fun = F2ND( tree_node_before2 ),                                 /* */
   },
  {
   .disabled = 0,                                                    /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .to_open = 0,                                                     /* */
   .dirent = 0,                                                      /* */
   .db = 1,                                                          /* */
   .fun = F2ND( tree_leaf2 ),                                        /* */
   },

  {.fun = NULL}
};

duf_scan_callbacks_t duf_mod_handler = {
  .title = "tree print",
  .name = "tree",
  .init_scan = NULL,                                                 /* */
  .no_progress = 1,
#if 0
  .beginning_sql_seq = &sql_create_selected,
#else
  .beginning_sql_seq = &sql_update_selected,
#endif

  .scanners = scanners,
/* 20160406.124048 */
#if 0
  .node_scan_before2 = F2ND( tree_node_before2 ),
  .leaf_scan2 = F2ND( tree_leaf2 ),
#endif

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

static
SR( MOD, tree_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  duf_fileinfo_t fi = { 0 };
  assert( H_PDI == pdi_unused );
  CR( fileinfo, /* pstmt_unused, pdi, */ sccbh, &fi );

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
      if ( DUF_CONFIGG( opt.output.max_width ) == 0 || DUF_CONFIGG( opt.output.max_width ) > slen )
        slen = duf_print_sformat_file_info( H_PDI, &fi, sformat_pref, F2ND( sql_print_tree_sprefix_uni ), ( duf_pdi_scb_t ) NULL,
                                            DUF_CONFIGG( opt.output.max_width ), mas_output_force_color(  ), mas_output_nocolor(  ), &rwidth, &over );
    }
    if ( !over )
    {
      {
        int use;
        duf_filedirformat_t *fmt;

        use = duf_pdi_pu( H_PDI )->use_format - 1;

        fmt = DUF_CONFIGA( opt.output.as_formats.tree );
        MAST_TRACE( temp, 5, "use:%d; files.argc:%d", use, fmt->files.argc );
        if ( use >= 0 && use < fmt->files.argc && !sformat )
          sformat = fmt->files.argv[use];
        MAST_TRACE( temp, 5, "sformat A: %s", sformat );
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_gen );
        MAST_TRACE( temp, 5, "sformat B: %s", sformat );
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_tree );
        MAST_TRACE( temp, 5, "sformat C: %s", sformat );
      }
      if ( !sformat )
        sformat = "%f\n";

      if ( DUF_CONFIGG( opt.output.max_width ) == 0 || DUF_CONFIGG( opt.output.max_width ) > slen )
        slen = duf_print_sformat_file_info( H_PDI, &fi, sformat, F2ND( sql_print_tree_sprefix_uni ), ( duf_pdi_scb_t ) NULL,
                                            DUF_CONFIGG( opt.output.max_width ), mas_output_force_color(  ), mas_output_nocolor(  ), &rwidth, &over );
    }
    DUF_PUTSL( 0 );
  }

/* SQL at duf_scan_files_by_dirid */

/* DUF_PRINTF( 0, "%s", filename ); */

  ER( MOD, tree_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

static
SR( MOD, tree_node_before2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/* if ( duf_levinfo_count_gfiles( pdi ) ) */
  {
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

    {
      const char *sformat_pref = NULL;
      const char *sformat = NULL;
      size_t slen = 0;
      size_t rwidth = 0;
      int over = 0;

    /* if ( duf_levinfo_count_gfiles( pdi ) )                  */
    /*   QT( "@gfiles:%llu", duf_levinfo_count_gfiles( pdi ) ); */

      {
        sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_gen_tree );
        if ( !sformat_pref )
          sformat_pref = DUF_CONFIGG( opt.output.sformat.prefix_dirs_tree );

        if ( !sformat_pref )
          sformat_pref = " %6s  %4s%P";
        if ( DUF_CONFIGG( opt.output.max_width ) == 0 || DUF_CONFIGG( opt.output.max_width ) > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat_pref, F2ND( sql_print_tree_sprefix_uni ), ( duf_pdi_scb_t ) NULL,
                                              DUF_CONFIGG( opt.output.max_width ), mas_output_force_color(  ), mas_output_nocolor(  ), &rwidth,
                                              &over );
      }
      if ( !over )
      {
        {
          int use;
          duf_filedirformat_t *fmt;

          use = duf_pdi_pu( pdi )->use_format - 1;

          fmt = DUF_CONFIGA( opt.output.as_formats.tree );
          MAST_TRACE( temp, 5, "use:%d; dirs.argc:%d", use, fmt->dirs.argc );
          if ( use >= 0 && use < fmt->dirs.argc && !sformat )
            sformat = fmt->dirs.argv[use];
          MAST_TRACE( temp, 5, "sformat A: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.dirs_gen );
          MAST_TRACE( temp, 5, "sformat B: %s", sformat );
          if ( !sformat )
            sformat = DUF_CONFIGG( opt.output.sformat.dirs_tree );
          MAST_TRACE( temp, 5, "sformat C: %s", sformat );
        }

        if ( !sformat )
          sformat = "%f\n";
        if ( DUF_CONFIGG( opt.output.max_width ) == 0 || DUF_CONFIGG( opt.output.max_width ) > slen )
          slen = duf_print_sformat_file_info( pdi, &fi, sformat, F2ND( sql_print_tree_sprefix_uni ), ( duf_pdi_scb_t ) NULL,
                                              DUF_CONFIGG( opt.output.max_width ), mas_output_force_color(  ), mas_output_nocolor(  ), &rwidth,
                                              &over );
      }
      DUF_PUTSL( 0 );
    }
  }

  ER( MOD, tree_node_before2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}

/* 20151113.132638 */
static
SR( MOD, sql_print_tree_sprefix_uni_d, char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, int d0, int maxd, int d )
{
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

  ndu = duf_levinfo_count_childs_d( pdi, du );                       /* beginning_sql_seq->set_selected_db */
  nchild = duf_levinfo_numchild_d( pdi, du );
  ndu -= nchild;
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

  ER( MOD, sql_print_tree_sprefix_uni_d, char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, int d0, int maxd, int d );
}

/* 20151113.132643 */
static
SR( MOD, sql_print_tree_sprefix_uni, char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, size_t * pwidth MAS_UNUSED )
{
  int d0 = duf_pdi_topdepth( pdi );
  int maxd = duf_pdi_depth( pdi );

  if ( d0 == 0 )
    d0 = 1;
  for ( int d = d0; QNOERR && d <= maxd; d++ )
  {
    CR( sql_print_tree_sprefix_uni_d, pbuffer, bfsz, pdi, d0, maxd, d );
    {
      size_t len = strlen( pbuffer );

      pbuffer += len;
      bfsz -= len;
    }
  }
  *pbuffer = 0;
  DUF_PRINTF( 1, "@%03d", QERRIND );

  ER( MOD, sql_print_tree_sprefix_uni, char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, size_t * pwidth );
}
