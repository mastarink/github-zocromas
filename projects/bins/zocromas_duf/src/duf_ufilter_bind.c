#include <string.h>
#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_config_ref.h"
#include "duf_option_defs.h"

#include "duf_path2db.h"
#include "duf_sql2.h"


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
    pfp->dirid = duf_path2dirid( dir, NULL, &r );
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
duf_bind_ufilter_uni( duf_sqlite_stmt_t * pstmt )
{
  DEBUG_STARTR( r );
#define DUF_SQL_BIND_PAIR( _fld, _name ) \
  if ( duf_config->pu->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, duf_config->pu->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, duf_config->pu->_name.max, r, pstmt ); \
  }
  DUF_SQL_BIND_PAIR( Size, size );
  DUF_SQL_BIND_PAIR( Same, same );
  DUF_SQL_BIND_PAIR( ExifSame, exifsame );
  DUF_SQL_BIND_PAIR( NameID, nameid );
  DUF_SQL_BIND_PAIR( DirID, dirid );
  DUF_SQL_BIND_PAIR( MTime, mtime );
  DUF_SQL_BIND_PAIR( ExifDT, exifdt );
  DUF_SQL_BIND_PAIR( Inode, inode );
  DUF_SQL_BIND_PAIR( Md5ID, md5id );
  DUF_SQL_BIND_PAIR( Sd2ID, sd5id );
  DUF_SQL_BIND_PAIR( MimeID, mimeid );
  DUF_SQL_BIND_PAIR( ExifID, exifid );
  if ( duf_config->pu->mime.type )
  {
    DUF_SQL_BIND_S_OPT( MimeType, duf_config->pu->mime.type, r, pstmt );
  }
  if ( duf_config->pu->glob_db )
  {
    DUF_SQL_BIND_S_OPT( GName, duf_config->pu->glob_db, r, pstmt );
  }
  if ( duf_config->pu->glob_db_include )
  {
    DUF_SQL_BIND_S_OPT( GNameI, duf_config->pu->glob_db_include, r, pstmt );
  }
  if ( duf_config->pu->glob_db_exclude )
  {
    DUF_SQL_BIND_S_OPT( GNameX, duf_config->pu->glob_db_exclude, r, pstmt );
  }

  DUF_SQL_BIND_LL_NZ_OPT( fFast, DUF_ACT_FLAG( fast ), r, pstmt );


  if ( duf_config->pu->same_md5 )
  {
    duf_filepath_t fp;

    DOR( r, duf_init_filepath( &fp, duf_config->pu->same_md5 ) );
    DUF_SQL_BIND_LL_NZ_OPT( GSamePathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAs, fp.name, r, pstmt );
    duf_clear_filepath( &fp );
    if ( r >= 0 && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }
#if 1
  if ( duf_config->pu->tag.file )
  {
    DUF_SQL_BIND_S_OPT( TagFile, duf_config->pu->tag.file, r, pstmt );
  }
  if ( duf_config->pu->tag.dir )
  {
    DUF_SQL_BIND_S_OPT( TagDir, duf_config->pu->tag.dir, r, pstmt );
  }
#endif
  DEBUG_ENDR( r );
}
