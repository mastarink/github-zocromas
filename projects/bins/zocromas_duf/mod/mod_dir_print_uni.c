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

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"                                  /* mas_output_force_color ; mas_output_nocolor */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */

#include "duf_sccb_structs.h"                                        /* duf_scan_callbacks_s; duf_sccb_data_row_s; duf_scanner_fun_s; ✗ */
#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sccbh.h"
#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_pathinfo_ref.h"

#include "duf_print.h"

#include "duf_ufilter_ref.h"
#include "duf_format_structs.h"

/* ########################################################################################## */
#include "duf_mod_types.h"
static DR( MOD, print_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */

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

/* ########################################################################################## */
static int use_format_once = 0;
static duf_scan_callbacks_t duf_sccb_dispatch;
static DR( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );
static DR( MOD, pack_leaf2, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"sccb", &duf_sccb_dispatch},
/* {"pack", F2ND( pack_leaf1 )}, */
  {NULL, NULL}
};

/* ########################################################################################## */
static duf_scanner_set_t scanners[] = {
  {
   .name = "traditional",
   .flags = DUF_SCANNER_SET_FLAG_DB | DUF_SCANNER_SET_FLAG_DISABLED, /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( print_leaf2 ),                                       /* */
   },
  {
   .name = "packed1",
   .flags = DUF_SCANNER_SET_FLAG_DB | DUF_SCANNER_SET_FLAG_PACK /* | DUF_SCANNER_SET_FLAG_DISABLED */ , /* */
   .type = DUF_NODE_LEAF,                                            /* */
 /* .scanstage = DUF_SCANSTAGE_DB_LEAVES_PACK,                        (* *) */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( pack_leaf1 ),                                        /* */
   },
  {
   .name = "packed2",
   .flags = DUF_SCANNER_SET_FLAG_DB | DUF_SCANNER_SET_FLAG_PACK /* | DUF_SCANNER_SET_FLAG_DISABLED */ , /* */
   .type = DUF_NODE_LEAF,                                            /* */
 /* .scanstage = DUF_SCANSTAGE_DB_LEAVES_PACK,                        (* *) */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( pack_leaf2 ),                                        /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "listing print",
  .name = "listing",
  .init_scan = NULL,                                                 /* */
  .no_progress = 1,
#if 0
# if 0
  .beginning_sql_seq = &sql_create_selected,
# else
  .beginning_sql_seq = &sql_update_selected,
# endif
#endif

/* 20160406.125422 */
#if 0
  .leaf_scan2 = F2ND( print_leaf2 ),
#else
  .scanners = scanners,
#endif

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* ########################################################################################## */

static MAS_UNUSED
SR( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused MAS_UNUSED, struct duf_sccb_handle_s *sccbh )
{
  int n = 0;
  const duf_sccb_data_row_t *trow = NULL;

  trow = duf_sccbh_start_first_row( sccbh );
  n = 0;
  while ( trow && trow != duf_sccbh_get_last_row( sccbh ) /* last: don't! */  )
  {
    if ( trow && trow->nfields )
    {
      n++;
      const char *path MAS_UNUSED;
      const char *rpath MAS_UNUSED;
      const char *iname MAS_UNUSED;

      path = CRX( pi_path, &trow->pathinfo );
      rpath = CRX( pi_relpath, &trow->pathinfo );
      iname = CRX( pi_itemname, &trow->pathinfo );

      MAST_TRACE( temp, 5, "@@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, path, iname );
      if ( 1 )
        MAST_TRACE( temp, 5, "@@@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, rpath, iname );

      if ( 1 )
      {
        use_format_once = 4;
        CR( print_leaf2, pdi_unused, sccbh );
      }
    }
  /* CRX( sccbh_set_current_row, sccbh, NULL ); */
    trow = duf_sccbh_next_row( sccbh );
  }
  duf_sccbh_end_row( sccbh );

  fprintf( stderr, "\n** %d **\n\n", n );
/* assert( n > 1 ); */
  ER( MOD, pack_leaf1, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );
}

static MAS_UNUSED
SR( MOD, pack_leaf2, duf_depthinfo_t * pdi_unused MAS_UNUSED, struct duf_sccb_handle_s *sccbh )
{
  int n = 0;
  const duf_sccb_data_row_t *trow = NULL;

  ( void ) duf_sccbh_start_last_row( sccbh );
  n = 0;
/* for ( duf_sccb_data_row_t * trow = rows->prev; trow; trow = trow->prev ) */
  while ( ( trow = duf_sccbh_prev_row( sccbh ) ) /* && trow!= duf_sccbh_get_last_row( sccbh ) */  )
  {
    if ( trow && trow->nfields )
    {
      n++;
      const char *path MAS_UNUSED;
      const char *rpath MAS_UNUSED;
      const char *iname MAS_UNUSED;

      path = CRX( pi_path, &trow->pathinfo );
      rpath = CRX( pi_relpath, &trow->pathinfo );
      iname = CRX( pi_itemname, &trow->pathinfo );

      MAST_TRACE( temp, 5, "@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, path, iname );
      if ( 1 )
        MAST_TRACE( temp, 5, "@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, rpath, iname );

      if ( 1 )
      {
        use_format_once = 1;
        CR( print_leaf2, pdi_unused, sccbh );
      }
    }
  /* CRX( sccbh_set_current_row, sccbh, NULL ); */
  }
  duf_sccbh_end_row( sccbh );

  fprintf( stderr, "\n** %d **\n\n", n );
/* assert( n > 1 ); */
  ER( MOD, pack_leaf2, duf_depthinfo_t * pdi_unused, struct duf_sccb_handle_s *sccbh );
}

static
SR( MOD, print_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh )
{
  DUF_RUFIELD2( dirid );
  DUF_RSFIELD2( fname );
  DUF_RUFIELD2( filesize );
  DUF_RUFIELD2( filemode );
  DUF_RUFIELD2( md5id );
  DUF_RUFIELD2( sha1id );
  DUF_RUFIELD2( dataid );
  DUF_RUFIELD2( md5sum1 );
  DUF_RUFIELD2( md5sum2 );
  DUF_RUFIELD2( sha1sum1 );
  DUF_RUFIELD2( sha1sum2 );
  DUF_RUFIELD2( sha1sum3 );
  DUF_RUFIELD2( mtime );
  assert( mtime == CRP( sccbh_row_get_number, sccbh, "mtime" ) );
  DUF_RUFIELD2( atime );
  assert( atime == CRP( sccbh_row_get_number, sccbh, "atime" ) );
  DUF_RUFIELD2( ctime );
  assert( ctime == CRP( sccbh_row_get_number, sccbh, "ctime" ) );
  DUF_RUFIELD2( dev );
  DUF_RUFIELD2( uid );
  DUF_RUFIELD2( gid );
  DUF_RUFIELD2( nlink );
  DUF_RUFIELD2( inode );
  DUF_RUFIELD2( exifid );
  DUF_RUFIELD2( exifdt );
  DUF_RSFIELD2( camera );
  DUF_RUFIELD2( filenameid );
  DUF_RUFIELD2( mimeid );
  DUF_RSFIELD2( mime );
  DUF_RUFIELD2( nsame );
  DUF_RUFIELD2( nsame_md5 );
  DUF_RUFIELD2( nsame_sha1 );
  DUF_RUFIELD2( nsame_exif );
  DUF_RUFIELD2( seq_gen );
  DUF_RUFIELD2( seq_node );
  DUF_RUFIELD2( seq_leaf );
  DUF_RUFIELD2( seq_row );
/* DUF_SFIELD( mtimef ); */
/* DUF_SFIELD( dowmtime ); */
/* DUF_SFIELD( monthmtime ); */

  assert( H_PDI == pdi_unused );

  {
    duf_fileinfo_t fi MAS_UNUSED = { 0 };
    fi.nsame = nsame;
    fi.nsame_md5 = nsame_md5;
    fi.nsame_sha1 = nsame_sha1;
    fi.nsame_exif = nsame_exif;
    fi.dirid = dirid;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
    fi.st.st_atim.tv_sec = atime;
    fi.st.st_atim.tv_nsec = 0;
    fi.st.st_ctim.tv_sec = ctime;
    fi.st.st_ctim.tv_nsec = 0;
    fi.st.st_dev = ( dev_t ) dev;
    fi.st.st_uid = ( uid_t ) uid;
    fi.st.st_gid = ( gid_t ) gid;
    fi.st.st_nlink = ( nlink_t ) nlink;
    fi.st.st_size = ( off_t ) filesize;
#if 1
    fi.name = fname;
#else
    {
      const char *prevs;

      prevs = duf_sccbh_prevrow_get_string( sccbh, "fname" );
      if ( !prevs || 0 != strcmp( fname, prevs ) )
        fi.name = fname;
    }
#endif
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
    fi.sha1sum1 = sha1sum1;
    fi.sha1sum2 = sha1sum2;
    fi.sha1sum3 = sha1sum3;
    fi.seqq.gen = seq_gen;
    fi.seqq.node = seq_node;
    fi.seqq.leaf = seq_leaf;
    fi.seqq.row = seq_row;

    {
      const char *sformat = NULL;
      size_t slen = 0;
      size_t rwidth = 0;
      int over = 0;

#if 0
      {
        int use;
        const duf_filedirformat_t *fmt;

# if 0
        use = duf_pdi_pu( H_PDI )->use_format - 1;
# else
        if ( use_format_once > 0 )
        {
          use = use_format_once - 1;
          use_format_once = 0;
        }
        else
          use = CRX( ufilter_use_format, CRX( pdi_pu, H_PDI ) ) - 1;
# endif
# if 0
        fmt = DUF_CONFIGA( opt.output.as_formats.list );
# else
        fmt = mas_get_config_output_asformat_list(  );
# endif
        if ( use >= 0 && use < fmt->files.argc && !sformat )
          sformat = fmt->files.argv[use];
# if 0
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_gen );
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_list );
# else
        if ( !sformat )
          sformat = mas_get_config_output_sformat_gen(  );
      /* if ( !sformat )                                     */
      /*   sformat = mas_get_config_output_sformat_list(  ); */
# endif
      }

      if ( !sformat )
        sformat = " _%M  =%S %8s%f\n";
#else
      sformat = CRX( get_item_format, sccbh, DUF_FORMAT_NAME_ID_LIST, 0, &use_format_once );
#endif
      {
#if 0
        unsigned max_width = DUF_CONFIGG( opt.output.max_width );
#else
        unsigned max_width = mas_get_config_output_max_width(  );
#endif
        if ( max_width == 0 || max_width > slen )
          slen = CRX( print_sformat_file_info, H_PDI, 1 /* from row */ , CRX( sccbh_row_current, sccbh ), ( const duf_sccb_data_row_t * ) NULL, sccbh,
                      &fi, sformat, ( duf_sccb_print_cb_t ) NULL, ( duf_sccb_print_cb_t ) NULL, max_width, mas_output_force_color(  ),
                      mas_output_nocolor(  ), &rwidth, &over );
      }
    }
  }

  ER( MOD, print_leaf2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );
}
