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

#include "std_mod_sets.h"


/* ########################################################################################## */

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* SQL at duf_scan_files_by_dirid */
  DUF_UFIELD2( dirid );
  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( filemode );
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( dataid );
  DUF_UFIELD2( md5sum1 );
  DUF_UFIELD2( md5sum2 );
  DUF_UFIELD2( mtime );
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

  /* if (nsame<2) return r; */

  /* DUF_PRINTF(0, ".> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* DUF_PRINTF(0, ".-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  {
    duf_fileinfo_t fi = { 0 };

    duf_bformat_combo_t bformat = {.v.flag = {
                                              .filename = 1,
                                              .depth = 1,
                                              .seq = 1,
                                              .seq_leaf = 1,
                                              .dirid = 1,
                                              .dirid_space = 1,
                                              .exifid = 1,
                                              .exifdt = 1,
                                              .nameid = 1,
                                              .mime = 1,
                                              .mimeid = 1,
                                              .inode = 1,
                                              .mode = 1,
                                              .nlink = 1,
                                              .user = 1,
                                              .group = 1,
                                              .filesize = 1,
                                              .md5 = 1,
                                              .md5id = 1,
                                              .mtime = 1,
                                              .dataid = 1,
                                              },
    .nsame = 1,
    };
    fi.nsame = nsame;
    fi.dirid = dirid;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
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
      duf_print_bformat_file_info( pdi, &fi, &bformat, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );
      DUF_PUTSL( 0 );
    }
    else
    {
      const char *sformat = NULL;

      sformat = duf_config->cli.output.sformat_files_gen;
      if ( !sformat )
        sformat = duf_config->cli.output.sformat_files_list;

      if ( !sformat )
        sformat = " _%M  =%S %8s%f\n";
      /* sformat = " _%M  =%S %f  %@\n"; */
      duf_print_sformat_file_info( pdi, &fi, sformat, ( duf_pdi_scb_t ) NULL, ( duf_pdi_scb_t ) NULL );
    }
  }

  DEBUG_ENDR( r );
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* static int                                                                                                               */
/* scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )                      */
/* {                                                                                                                        */
/*   DEBUG_STARTR( r );                                                                                                     */
/*                                                                                                                          */
/*   {                                                                                                                      */
/*     const char *real_path = NULL;                                                                                        */
/*                                                                                                                          */
/*     if ( !real_path )                                                                                                    */
/*       real_path = duf_levinfo_path( pdi );                                                                               */
/*                                                                                                                          */
/*     DUF_UFIELD( nfiles );                                                                                                */
/*     DUF_UFIELD( minsize );                                                                                               */
/*     DUF_UFIELD( maxsize );                                                                                               */
/*                                                                                                                          */
/*     if ( nfiles )                                                                                                        */
/*     {                                                                                                                    */
/*       (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good *)               */
/*       if ( duf_config->cli.bformat.seq )                                                                                  */
/*         DUF_PRINTF( 0, ".d%7llu ", pdi->seq );                                                                           */
/*       if ( duf_config->cli.bformat.dirid )                                                                                */
/*         DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                    */
/*       DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" ); */
/*     }                                                                                                                    */
/*     (* mas_free( path ); *)                                                                                              */
/*   }                                                                                                                      */
/*                                                                                                                          */
/*   DEBUG_ENDR( r );                                                                                                       */
/* }                                                                                                                        */

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  /* DUF_UFIELD2( nfiles ); */
  /* DUF_UFIELD2( minsize ); */
  /* DUF_UFIELD2( maxsize ); */
  /* {                                                                                                                      */
  /*   const char *real_path = NULL;                                                                                        */
  /*                                                                                                                        */
  /*   if ( !real_path )                                                                                                    */
  /*     real_path = duf_levinfo_path( pdi );                                                                               */
  /*                                                                                                                        */
  /*   if ( nfiles )                                                                                                        */
  /*   {                                                                                                                    */
  /*     (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good *)               */
  /*     if ( duf_config->cli.bformat.seq )                                                                                  */
  /*       DUF_PRINTF( 0, ".d%7llu ", pdi->seq );                                                                           */
  /*     if ( duf_config->cli.bformat.dirid )                                                                                */
  /*       DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                    */
  /*     DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" ); */
  /*   }                                                                                                                    */
  /*   (* mas_free( path ); *)                                                                                              */
  /* }                                                                                                                      */

  /*
   * it's good to print directories to have all items listed under 'current' directory
   * BUT
   * it's not good to print directories for selected files if there are no files
   * */
  if (  /* 1 || */ duf_levinfo_items_files( pdi ) > 0 )
  {
    duf_fileinfo_t fi = { 0 };

    duf_bformat_combo_t bformat = {.v.flag = {
                                              .filename = 0,
                                              .realpath = 1,
                                              .depth = 1,
                                              .seq = 1,
                                              .seq_node = 1,
                                              .dirid = 1,
                                              .exifid = 0,
                                              .exifdt = 0,
                                              .mimeid = 0,
                                              .inode = 0,
                                              .mode = 0,
                                              .nlink = 0,
                                              .user = 0,
                                              .group = 0,
                                              .filesize = 0,
                                              .md5 = 0,
                                              .md5id = 0,
                                              .dataid = 0,
                                              .mtime = 0,
                                              .prefix = 1,
                                              .suffix = 1,
                                              }
    ,
    .nsame = 0,
    };
    /* fi.nsame = nsame; */
    /* fi.st.st_mode = ( mode_t ) filemode; */
    /* fi.st.st_ino = ( ino_t ) inode;      */
    /* fi.st.st_mtim.tv_sec = mtime;        */
    /* fi.st.st_mtim.tv_nsec = 0;           */
    /* fi.st.st_uid = ( uid_t ) uid;        */
    /* fi.st.st_gid = ( gid_t ) gid;        */
    /* fi.st.st_nlink = ( nlink_t ) nlink;  */
    /* fi.st.st_size = ( off_t ) filesize; */
    fi.name = duf_levinfo_itemname( pdi );
    /* fi.md5id = md5id; */
    /* fi.md5sum1 = md5sum1; */
    /* fi.md5sum2 = md5sum2; */
    DUF_DEBUG( 0, PF( "at module : %llx (%d) :: %llx", bformat.v.bit, bformat.v.flag.seq, duf_config->cli.bformat.v.bit ) );
    if ( DUF_ACT_FLAG( use_binformat ) )
    {
      if ( duf_print_bformat_file_info( pdi, &fi, &bformat, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL ) > 0 )
      {
        /* DUF_PUTSL( 0 ); */
      }
      else
      {
        DUF_PUTS( 0, "????????????" );
      }
      DUF_PUTSL( 0 );
    }
    else
    {
      const char *sformat = NULL;

      sformat = duf_config->cli.output.sformat_dirs_gen;
      if ( !sformat )
        sformat = duf_config->cli.output.sformat_dirs_list;

      if ( !sformat )
        sformat = "%r\n";
      duf_print_sformat_file_info( pdi, &fi, sformat, ( duf_pdi_scb_t ) NULL, ( duf_pdi_scb_t ) NULL );
    }
  }

  DEBUG_ENDR( r );
}

/* NOTES */
 /* " , DATETIME(mtim, 'unixepoch') AS mtimef " */
 /* ", strftime('%Y-%m-%d %H:%M:%S',mtim,'unixepoch') AS mtimef " */
 /* ", case cast (strftime('%w', mtim,'unixepoch') AS integer) "                                                                   */
 /* " when 0 then 'Sun' when 1 then 'Mon' when 2 then 'Tue' when 3 then 'Wed' "                                                    */
 /* " when 4 then 'Thu' when 5 then 'Fri' else 'Sat' end AS dowmtime, " "case cast (strftime('%m', mtim,'unixepoch') AS integer) " */
 /* " when 1 then 'Jan' when 2 then 'Feb' when 3 then 'Mar' when 4 then 'Apr' when 5 then 'May' when 6 then "                      */
 /* " 'Jun' when 7 then 'Jul' when 8 then 'Aug' when 9 then 'Sep' when 10 then 'Oct' when 11 then 'Nov' when 12 then 'Dec' "       */
 /* " else 'Wow' end AS monthmtime "                                                                                               */
/* /NOTES */


duf_scan_callbacks_t duf_print_dir_callbacks = {
  .title = "listing print",
  .name = "listing",
  .init_scan = NULL,            /* */
  .beginning_sql_seq = &sql_beginning_selected,
  /* .node_scan_before = scan_node_before, */
  .node_scan_before2 = scan_node_before2,
  /* .leaf_scan = scan_leaf, */
  .leaf_scan2 = scan_leaf2,
  .use_std_leaf = 1, /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 1, /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
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
