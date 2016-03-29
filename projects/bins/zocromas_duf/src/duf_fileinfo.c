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
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

/* ###################################################################### */
#include "duf_fileinfo.h"
/* ###################################################################### */

SR( OTHER, fileinfo, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi MAS_UNUSED, duf_fileinfo_t * pfi )
{
/* DUF_STARTR( r ); */
  if ( pfi )
  {
    DUF_UFIELD2( dirid );
    DUF_SFIELD2( fname );
    DUF_UFIELD2( filesize );
    DUF_UFIELD2( filemode );
    DUF_UFIELD2( md5id );
    DUF_UFIELD2( dataid );
    DUF_UFIELD2( md5sum1 );
    DUF_UFIELD2( md5sum2 );
    DUF_UFIELD2( sha1sum1 );
    DUF_UFIELD2( sha1sum2 );
    DUF_UFIELD2( sha1sum3 );
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
  ER( OTHER, fileinfo, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi MAS_UNUSED, duf_fileinfo_t * pfi );
}
