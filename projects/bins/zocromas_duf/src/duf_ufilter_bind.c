#include <string.h>
#include <libgen.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_config_ref.h"
#include "duf_option_defs.h"

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

int
duf_bind_ufilter_uni( duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_argvc_t * ttarg_unused )
{
  DEBUG_STARTR( r );
#if 0
#  define DUF_SQL_BIND_PAIR( _fld, _name ) \
  if ( DUF_CONFIGG(puz)->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, DUF_CONFIGG(puz)->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, DUF_CONFIGG(puz)->_name.max, r, pstmt ); \
  }
  assert( DUF_CONFIGX( puz ) );
#else
#  define DUF_SQL_BIND_PAIR( _fld, _name ) \
  if ( pu->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, pu->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, pu->_name.max, r, pstmt ); \
  }
  if ( !pu )
    return 0;
#endif
  DUF_SQL_BIND_PAIR( Size, size );
  DUF_SQL_BIND_PAIR( Md5Same, md5same );
  DUF_SQL_BIND_PAIR( Sha1Same, sha1same );
  DUF_SQL_BIND_PAIR( ExifSame, exifsame );
  DUF_SQL_BIND_PAIR( SizeSame, sizesame );
  DUF_SQL_BIND_PAIR( DataSame, datasame );
  DUF_SQL_BIND_PAIR( MimeSame, mimesame );
  DUF_SQL_BIND_PAIR( NameID, nameid );
  DUF_SQL_BIND_PAIR( DataID, dataid );
  DUF_SQL_BIND_PAIR( DirID, dirid );
  DUF_SQL_BIND_PAIR( MTime, mtime );
  DUF_SQL_BIND_PAIR( ExifDT, exifdt );
  DUF_SQL_BIND_PAIR( Inode, inode );
  DUF_SQL_BIND_PAIR( Md5ID, md5id );
  DUF_SQL_BIND_PAIR( Sd2ID, sd5id );
  DUF_SQL_BIND_PAIR( MimeID, mimeid );
  DUF_SQL_BIND_PAIR( ExifID, exifid );
  if ( pu->mime.type )
  {
    DUF_SQL_BIND_S_OPT( MimeType, pu->mime.type, r, pstmt );
  }
  if ( pu->filename )
  {
    DUF_SQL_BIND_S_OPT( Name, pu->filename, r, pstmt );
  }
  if ( pu->glob_db )
  {
    DUF_SQL_BIND_S_OPT( GName, pu->glob_db, r, pstmt );
  }
  if ( pu->glob_db_include )
  {
    DUF_SQL_BIND_S_OPT( GNameI, pu->glob_db_include, r, pstmt );
  }
  if ( pu->glob_db_exclude )
  {
    DUF_SQL_BIND_S_OPT( GNameX, pu->glob_db_exclude, r, pstmt );
  }
  if ( pu->exif.camera )
  {
    char *t;

    t = mas_strdup( "%" );
    t = mas_strcat_x( t, pu->exif.camera );
    t = mas_strcat_x( t, "%" );
    DUF_SQL_BIND_S_OPT( Camera, t, r, pstmt );
    mas_free( t );
  }

  DUF_SQL_BIND_LL_NZ_OPT( fFast, DUF_ACTG_FLAG( fast ), r, pstmt );


  if ( pu->same_md5 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_md5 ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSamePathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAs, fp.name, r, pstmt );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
  if ( pu->same_sha1 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, pu->same_sha1 ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSameSha1PathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAsSha1, fp.name, r, pstmt );
    duf_clear_filepath( &fp );
    if ( DUF_NOERROR( r ) && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }

#if 1
  if ( pu->tag.file )
  {
    DUF_SQL_BIND_S_OPT( TagFile, pu->tag.file, r, pstmt );
  }
  if ( pu->tag.dir )
  {
    DUF_SQL_BIND_S_OPT( TagDir, pu->tag.dir, r, pstmt );
  }
  DUF_SQL_BIND_LL_NZ_OPT( Option_Val_With_Tag_File, DUF_OPTION_VAL_FILTER_WITH_TAG_FILE, r, pstmt );
#endif
  DEBUG_ENDR( r );
}
