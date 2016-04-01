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

#include "duf_sccb_structs.h"

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"                                  /* mas_output_force_color ; mas_output_nocolor */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_print.h"

#include "duf_fileinfo.h"

#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "sql_beginning_selected.h"

/* ########################################################################################## */
static int duf_print_leaf2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

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

duf_scan_callbacks_t duf_mod_handler = {
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
  .leaf_scan2 = duf_print_leaf2,
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) ; XXX index in std_leaf_sets */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
};

/* ########################################################################################## */

static
SR( MOD, print_leaf2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
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
  assert( mtime == duf_sccbh_row_get_number( sccbh, "mtime" ) );
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
/* DUF_SFIELD( mtimef ); */
/* DUF_SFIELD( dowmtime ); */
/* DUF_SFIELD( monthmtime ); */

  {
    duf_fileinfo_t fi = { 0 };
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

    {
      const char *sformat = NULL;
      size_t slen = 0;
      size_t rwidth = 0;
      int over = 0;

      {
        int use;
        duf_filedirformat_t *fmt;

        use = duf_pdi_pu( pdi )->use_format - 1;

        fmt = DUF_CONFIGA( opt.output.as_formats.list );
        if ( use >= 0 && use < fmt->files.argc && !sformat )
          sformat = fmt->files.argv[use];
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_gen );
        if ( !sformat )
          sformat = DUF_CONFIGG( opt.output.sformat.files_list );
      }

      if ( !sformat )
        sformat = " _%M  =%S %8s%f\n";
      if ( DUF_CONFIGG( opt.output.max_width ) == 0 || DUF_CONFIGG( opt.output.max_width ) > slen )
        slen = duf_print_sformat_file_info( pdi, &fi, sformat, ( duf_pdi_scb_t ) NULL, ( duf_pdi_scb_t ) NULL, DUF_CONFIGG( opt.output.max_width ),
                                            mas_output_force_color(  ), mas_output_nocolor(  ), &rwidth, &over );
      DUF_PUTSL( 0 );
    }
  }

  ER( MOD, print_leaf2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
