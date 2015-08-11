#define DUF_SQL_PDI_STMT

#include <string.h>





/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>



#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"


#include "duf_option_defs.h"
#include "duf_print.h"


/* #include "duf_dbg.h" */

#include "sql_beginning_selected.h"


/* ########################################################################################## */
static int duf_sql_print_tree_prefix_uni( duf_depthinfo_t * pdi );
static int duf_sql_print_tree_sprefix_uni( char *pbuffer, size_t bfsz, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static int scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );
static int scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */

duf_scan_callbacks_t duf_print_tree_callbacks = {
  .title = "tree print",
  .name = "tree",
  .init_scan = NULL,            /* */
  .no_progress = 1,
  .beginning_sql_seq = &sql_beginning_selected,
  /* .node_scan_before = tree_scan_node_before, */
  .node_scan_before2 = scan_node_before2,
  /* .leaf_scan = tree_scan_leaf, */
  .leaf_scan2 = scan_leaf2,

  /* for "tree" 1 is much better in following 2 fields; BUT TODO: try 2 and 1 - may be good?! */
  .use_std_leaf = 2,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
#if 0
  .leaf = {
           .fieldset = NULL,    /* */
           .selector2 = NULL,   /* */
           .selector_total2 = NULL /* */
           }
  ,
  .node = {
           .fieldset = NULL,    /* */
           .selector2 = NULL    /* */
           }
#endif
  /* , .final_sql_seq = &final_sql, */
};

/* ########################################################################################## */

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_UFIELD2( dirid );
  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( filemode );
  DUF_UFIELD2( md5id );
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
  DUF_UFIELD2( filenameid );
  DUF_UFIELD2( mimeid );
  DUF_SFIELD2( mime );
  DUF_UFIELD2( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  {
    duf_bformat_combo_t bformat = { /* */
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
                 /* .nameid = 1, */
                 .mime = 1,
                 .mimeid = 0,
                 .nfiles_space = 1,
                 .ndirs_space = 1,
                 .inode = 0,
                 .mode = 1,
                 .nlink = 1,
                 .user = 1,
                 .group = 1,
                 .filesize = 1,
                 .md5 = 1,
                 .md5id = 1,
                 .mtime = 1,
                 .dataid = 1,
                 .prefix = 1,
                 .suffix = 1,
                 },
      .nsame = 1,
    };

    duf_fileinfo_t fi = { 0 };
    fi.nsame = nsame;
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
    fi.nameid = filenameid;
    fi.mime = mime;
    fi.mimeid = mimeid;
    fi.md5id = md5id;
    fi.dataid = dataid;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;

    if ( DUF_ACT_FLAG( use_binformat ) )
    {
      if ( duf_print_bformat_file_info( pdi, &fi, &bformat, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
        DUF_PUTSL( 0 );
      else
        DUF_PUTS( 0, "????????????" );
    }
    else
    {
      const char *sformat = NULL;

      sformat = duf_config->cli.output.sformat_prefix_gen_tree;
      if ( !sformat )
        sformat = duf_config->cli.output.sformat_prefix_files_tree;

      if ( !sformat )
        sformat = "_%-6M =%-4S%P";
      duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL );

      sformat = duf_config->cli.output.sformat_files_gen;
      if ( !sformat )
        sformat = duf_config->cli.output.sformat_files_tree;

      if ( !sformat )
        sformat = "%f\n";
      duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL );
    }
  }

  /* r = duf_sql_print_tree_prefix_uni( pdi ); */

  /* SQL at duf_scan_files_by_dirid */

  /* DUF_PRINTF( 0, "%s", filename ); */

  DEBUG_ENDR( r );
}

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  duf_bformat_combo_t bformat = { /* */
    .v.flag = {
               .filename = 1,
               .short_filename = 1,
               .depth = 1,
               .seq = 1,
               .dirid = 1,
               .exifid = 0,
               .exifdt = 0,
               .mimeid = 0,
               .nfiles = 1,
               .ndirs = 1,
               .inode = 0,
               .mode = 0,
               .nlink = 0,
               .user = 0,
               .group = 0,
               .filesize = 0,
               .md5 = 0,
               .md5id = 1,
               .mtime = 0,
               .prefix = 1,
               .suffix = 1,
               },
    .nsame = 1,
  };

  duf_fileinfo_t fi = { 0 };
  /* fi.nsame = nsame; */
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
  /* fi.md5sum1 = md5sum1; */
  /* fi.md5sum2 = md5sum2; */
  if ( DUF_ACT_FLAG( use_binformat ) )
  {
    if ( duf_print_bformat_file_info( pdi, &fi, &bformat, duf_sql_print_tree_prefix_uni, ( duf_pdi_cb_t ) NULL ) > 0 )
      DUF_PUTSL( 0 );
    else
      DUF_PUTS( 0, "????????????" );
  }
  else
  {
    const char *sformat = NULL;

    sformat = duf_config->cli.output.sformat_prefix_gen_tree;
    if ( !sformat )
      sformat = duf_config->cli.output.sformat_prefix_dirs_tree;

    if ( !sformat )
      sformat = " %6s  %4s%P";
    duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL );

    sformat = duf_config->cli.output.sformat_dirs_gen;
    if ( !sformat )
      sformat = duf_config->cli.output.sformat_dirs_tree;

    if ( !sformat )
      sformat = "%f\n";
    duf_print_sformat_file_info( pdi, &fi, sformat, duf_sql_print_tree_sprefix_uni, ( duf_pdi_scb_t ) NULL );
  }


  DEBUG_ENDR( r );
}





static int
duf_sql_print_tree_sprefix_uni( char *pbuffer, size_t bfsz, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );


  int d0 = duf_pdi_topdepth( pdi );
  int max = duf_pdi_depth( pdi );

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

  if ( d0 == 0 )
    d0 = 1;
  for ( int d = d0; d <= max; d++ )
  {
    int du = d - 1;
    unsigned flags = 0;
    long ndu = duf_levinfo_numdir_d( pdi, du );
    char nduc = ndu > 0 ? '+' : ( ndu < 0 ? '-' : 'o' );
    int leaf = duf_levinfo_is_leaf_d( pdi, d );
    char leafc = leaf ? 'L' : 'D';

    /* int eod = duf_levinfo_eod_d( pdi, d ); */
    /* char eodc = eod ? '.' : '~';           */

    /* char is_filec = is_file ? 'F' : '-'; */

    if ( ndu > 0 && d > d0 )
      flags |= 0x1;
    if ( ndu < 0 && d > d0 )
      flags |= 0x2;
    if ( leaf )
      flags |= 0x4;
    /* if ( eod )      */
    /*   flags |= 0x8; */
    if ( d == max )
      flags |= 0x10;

    /* if ( d >= duf_pdi_maxdepth( pdi ) ) */
    if ( !duf_pdi_is_good_depth_d( pdi, leaf ? 1 : 0, d ) )
      flags |= 0x20;

    /* if ( is_file )   */
    /*   flags |= 0x40; */
    DUF_DEBUG( 1,               /* */
               DUF_PRINTF( 0, ".[L%-2d", d ); /* */
               DUF_PRINTF( 0, ".M%-2d", duf_pdi_maxdepth( pdi ) );
               /* DUF_PRINTF( 0, ".rd%d", duf_pdi_reldepth( pdi ) ); */
               DUF_PRINTF( 0, ".@%-3ld", ndu ); /* */
               DUF_PRINTF( 0, ".%c%c", nduc, leafc ); /* */
               DUF_PRINTF( 0, ".0x%02x]", flags );
           );
    {
#if 0
      {
        int l;

        snprintf( pbuffer, bfsz, "%03lu", ndu );

        l = strlen( pbuffer );
        pbuffer += l;
        bfsz -= l;
      }
#endif
      /* DUF_PRINTF( 0, ".%05ld", ndu ); */
      /* if ( duf_levinfo_is_leaf_d( pdi, d ) ) */
      /*   DUF_PRINTF( 0, ".[  ◇ ]" );        */
      /* else                                   */
      switch ( flags )
      {
      case 0x14:
      case 0x34:
      case 0x35:
        /* strncpy(pbuffer, "  → ", bfsz ); */
        strncpy( pbuffer, "  ", bfsz );
        break;
      case 0x15:
        /* strncpy(pbuffer, "│ → ", bfsz ); */
        strncpy( pbuffer, "│ ", bfsz );
        break;
      case 0x10:
      case 0x30:
        strncpy( pbuffer, "└─── ", bfsz );
        break;
      case 0x11:
      case 0x31:
        strncpy( pbuffer, "├─── ", bfsz );
        break;
      case 0x01:
      case 0x21:
        strncpy( pbuffer, "│    ", bfsz );
        break;
      case 0x12:
        /* strncpy(pbuffer, "┣━━━ " , bfsz); */
        break;
      case 0x28:
      case 0x8:
        strncpy( pbuffer, "     ", bfsz );
        break;
      case 0x20:
      case 0x00:
        strncpy( pbuffer, "     ", bfsz );
        break;
      case 0x02:
        /* strncpy(pbuffer, "┃    " , bfsz); */
        break;
      default:
        snprintf( pbuffer, bfsz, " [x%02x]", flags );
        break;
      }
    }

    {
      size_t len = strlen( pbuffer );

      pbuffer += len;
      bfsz -= len;
    }
  }
  *pbuffer = 0;

  DEBUG_ENDR( r );
}

static int
duf_sql_print_tree_prefix_uni( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  char buffer[1024];

  duf_sql_print_tree_sprefix_uni( buffer, sizeof( buffer ), pdi );
  DUF_WRITES( 0, buffer );
  DEBUG_ENDR( r );
}
