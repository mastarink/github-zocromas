/* #undef MAS_TRACING */
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */


#include "duf_levinfo_ref.h"

#include "duf_config_ref.h"
#include "duf_config_util.h"

#include "duf_pdi.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

/* ###################################################################### */
#include "duf_fileinfo.h"
/* ###################################################################### */

int
duf_fileinfo( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi MAS_UNUSED, duf_fileinfo_t * pfi )
{
  DUF_STARTR( r );
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
  DUF_ENDR( r );
}
