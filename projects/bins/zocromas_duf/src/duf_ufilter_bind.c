#include <string.h>
#include <libgen.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_util.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"
#include "duf_option_defs.h"
#include "duf_options_enum.h" /* DUF_OPTION_VAL_... */

#include "duf_path2dirid.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_bind.h"


/* ###################################################################### */
#include "duf_ufilter_bind.h"
/* ###################################################################### */

int
duf_init_filepath( duf_filepath_t * pfp, const char *filepath )
{
  DEBUG_STARTR( r );
  char *pathname;

  memset( pfp, 0, sizeof( duf_filepath_t ) );
  pathname = mas_strdup( filepath );
  {
    char *dir;
    char *base;

    base = basename( pathname );
    dir = dirname( pathname );
    pfp->dirid = duf_path2dirid( dir, &r );
    pfp->name = mas_strdup( base );
  }
  mas_free( pathname );
  DEBUG_ENDR( r );
}

static void
duf_clear_filepath( duf_filepath_t * pfp )
{
  if ( pfp )
    mas_free( pfp->name );
}

static int
_duf_bind_ufilter_uni( duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused DUF_UNUSED )
{
  DEBUG_STARTR( r );
#if 0
#  define DUF_SQL_BIND_PAIR( _fld, _name ) \
  if ( DUF_CONFIGG(scn.puz)->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, DUF_CONFIGG(scn.puz)->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, DUF_CONFIGG(scn.puz)->_name.max, r, pstmt ); \
  }
  assert( DUF_CONFIGX( scn.puz ) );
#else
#  define DUF_SQL_BIND_PAIR( _fld, _name ) \
  if ( pu->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, pu->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, pu->_name.max, r, pstmt ); \
    DUF_TRACE(sql, 3, "@@@bind %s: %llu-%llu", # _name, pu->_name.min, pu->_name.max ); \
  }
#endif
  DUF_TRACE( sql, 3, "py:%d - %llu", py ? 1 : 0, py ? py->topdirid : 0 );
  if ( py )
  {
    DUF_SQL_BIND_LL_NZ_OPT( topDirID, py->topdirid, r, pstmt );
    /* T( "(%d) BIND topdirid:%llu", r , py->topdirid ); */
    /* assert( py->topdirid ); */
  }
  else if ( 0 == strncmp( "WITH", sqlite3_sql( pstmt ), 4 ) )
  {
    assert( 0 );
  }


  DUF_SQL_BIND_PAIR( Size, size );
  DUF_SQL_BIND_PAIR( Md5Same, same.md5 );
  DUF_SQL_BIND_PAIR( Sha1Same, same.sha1 );
  DUF_SQL_BIND_PAIR( ExifSame, same.exif );
  DUF_SQL_BIND_PAIR( SizeSame, same.size );
  DUF_SQL_BIND_PAIR( DataSame, same.data );
  DUF_SQL_BIND_PAIR( MimeSame, same.mime );

  DUF_SQL_BIND_PAIR( NameID, nameid );
  DUF_SQL_BIND_PAIR( Md5ID, md5id );
  DUF_SQL_BIND_PAIR( Sha1ID, sha1id );
  DUF_SQL_BIND_PAIR( Sd2ID, sd5id );
  DUF_SQL_BIND_PAIR( DataID, dataid );
  DUF_SQL_BIND_PAIR( DirID, dirid );

  DUF_SQL_BIND_PAIR( MTime, mtime );
  DUF_SQL_BIND_PAIR( ExifDT, exifdt );
  DUF_SQL_BIND_PAIR( Inode, inode );
  DUF_SQL_BIND_PAIR( MimeID, mimeid );
  DUF_SQL_BIND_PAIR( ExifID, exifid );
  if ( pu->mime.type )
  {
    DUF_SQL_BIND_S_OPT( MimeType, pu->mime.type, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind MimeType: %s", pu->mime.type );
  }
  if ( pu->filename )
  {
    DUF_SQL_BIND_S_OPT( Name, pu->filename, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db )
  {
    DUF_SQL_BIND_S_OPT( GName, pu->glob_db, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db_include )
  {
    DUF_SQL_BIND_S_OPT( GNameI, pu->glob_db_include, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->glob_db_exclude )
  {
    DUF_SQL_BIND_S_OPT( GNameX, pu->glob_db_exclude, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->exif.camera )
  {
    char *t;

    t = mas_strdup( "%" );
    t = mas_strcat_x( t, pu->exif.camera );
    t = mas_strcat_x( t, "%" );
    DUF_SQL_BIND_S_OPT( Camera, t, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
    mas_free( t );
  }

#define DUF_SQL_BIND_SAME(_u, _l) \
	if ( pu->same_as._l ) \
	{ \
	  duf_filepath_t fp; \
 \
	  DOR( r, duf_init_filepath( &fp, pu->same_as._l ) ); \
	  DUF_SQL_BIND_LL_NZ_OPT( GSame ## _u ## PathID, fp.dirid, r, pstmt ); \
	  DUF_SQL_BIND_S_OPT( GSameAs ## _u, fp.name, r, pstmt ); \
	  DUF_TRACE( sql, 0, "@@@bind GSame" # _u "PathID:%llu", fp.dirid ); \
	  duf_clear_filepath( &fp ); \
	  if ( DUF_NOERROR( r ) && !fp.dirid ) \
	    DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB ); \
	}
#if 0
  if ( pu->same_as.md5 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_as.md5 ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSameMd5PathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAsMd5, fp.name, r, pstmt );
    DUF_TRACE( sql, 0, "@@@bind GSameMd5PathID:%llu", fp.dirid );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
  if ( pu->same_as.sha1 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_as.sha1 ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSameSha1PathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAsSha1, fp.name, r, pstmt );
    DUF_TRACE( sql, 0, "@@@bind GSameSha1PathID:%llu", fp.dirid );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
  if ( pu->same_as.exif )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_as.exif ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSameExifPathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAsExif, fp.name, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
#else
  DUF_SQL_BIND_SAME( Md5, md5 );
  DUF_SQL_BIND_SAME( Sha1, sha1 );
  DUF_SQL_BIND_SAME( Exif, exif );
#endif

#if 1
  if ( pu->tag.file )
  {
    DUF_SQL_BIND_S_OPT( TagFile, pu->tag.file, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  if ( pu->tag.dir )
  {
    DUF_SQL_BIND_S_OPT( TagDir, pu->tag.dir, r, pstmt );
    DUF_TRACE( sql, 3, "@@@bind " );
  }
  DUF_SQL_BIND_I_NZ_OPT( Option_Val_With_Tag_File, DUF_OPTION_VAL_FILTER_WITH_TAG_FILE, r, pstmt );
  DUF_SQL_BIND_I_NZ_OPT( fFast, DUF_ACTG_FLAG( fast ), r, pstmt );
  DUF_TRACE( sql, 3, "@@@bind Option_Val_With_Tag_File='%d'", DUF_OPTION_VAL_FILTER_WITH_TAG_FILE );
#endif
  DEBUG_ENDR( r );
}

int
duf_bind_ufilter_uni( duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused )
{
  DEBUG_STARTR( r );
  if ( !pu )
  {
    DUF_TRACE( sql, 3, "@@@bind - no pu!" );
    return 0;
  }
  if ( pu )
    _duf_bind_ufilter_uni( pstmt, pu, py, ttarg_unused );
  DEBUG_ENDR( r );
}
