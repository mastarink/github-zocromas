#define DUF_GET_FIELD_FROM_ROW
/* #undef MAS_TRACING */
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

/* #include "duf_sql_field.h"                                           (* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ *) */

/* ###################################################################### */
#include "duf_fileinfo.h"
/* ###################################################################### */

SR( OTHER, fileinfo, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED, duf_fileinfo_t * pfi )
{
/* DUF_STARTR( r ); */
  if ( pfi )
  {
    DUF_RUFIELD2( dirid );
    DUF_RSFIELD2( fname );
    DUF_RUFIELD2( filesize );
    DUF_RUFIELD2( filemode );
    DUF_RUFIELD2( md5id );
    DUF_RUFIELD2( dataid );
    DUF_RUFIELD2( md5sum1 );
    DUF_RUFIELD2( md5sum2 );
    DUF_RUFIELD2( sha1sum1 );
    DUF_RUFIELD2( sha1sum2 );
    DUF_RUFIELD2( sha1sum3 );
    DUF_RUFIELD2( mtime );
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

    memset( pfi, 0, sizeof( duf_fileinfo_t ) );
    pfi->nsame = nsame;
    pfi->nsame_md5 = nsame_md5;
    pfi->nsame_sha1 = nsame_sha1;
    pfi->nsame_exif = nsame_exif;
    pfi->dirid = dirid;
    pfi->st.st_mode = ( mode_t ) filemode;
    pfi->st.st_ino = ( ino_t ) inode;
    pfi->st.st_mtim.tv_sec = mtime;
    pfi->st.st_mtim.tv_nsec = 0;
    pfi->st.st_dev = ( dev_t ) dev;
    pfi->st.st_uid = ( uid_t ) uid;
    pfi->st.st_gid = ( gid_t ) gid;
    pfi->st.st_nlink = ( nlink_t ) nlink;
    pfi->st.st_size = ( off_t ) filesize;
    pfi->name = fname;
    pfi->exifid = exifid;
    pfi->exifdt = exifdt;
    pfi->camera = camera;
    pfi->nameid = filenameid;
    pfi->mime = mime;
    pfi->mimeid = mimeid;
    pfi->md5id = md5id;
    pfi->dataid = dataid;
    pfi->md5sum1 = md5sum1;
    pfi->md5sum2 = md5sum2;
    pfi->sha1sum1 = sha1sum1;
    pfi->sha1sum2 = sha1sum2;
    pfi->sha1sum3 = sha1sum3;
  }
/* DUF_ENDR( r ); */
  ER( OTHER, fileinfo, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi );
}
