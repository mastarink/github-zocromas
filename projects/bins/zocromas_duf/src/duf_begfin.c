#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_path2db.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_ufilter.h"

/* ###################################################################### */
#include "duf_begfin.h"
/* ###################################################################### */


static int
duf_bind_ufilter( duf_sqlite_stmt_t * pstmt )
{
  DEBUG_STARTR( r );
#define BIND_PAIR( _fld, _name ) \
  if ( duf_config->pu->_name.flag ) \
  { \
    DUF_SQL_BIND_LL_NZ_OPT( min ## _fld, duf_config->pu->_name.min, r, pstmt ); \
    DUF_SQL_BIND_LL_NZ_OPT( max ## _fld, duf_config->pu->_name.max, r, pstmt ); \
  }
  BIND_PAIR( Size, size );
  BIND_PAIR( Same, same );
  BIND_PAIR( ExifSame, exifsame );
  BIND_PAIR( NameID, nameid );
  BIND_PAIR( DirID, dirid );
  BIND_PAIR( MTime, mtime );
  BIND_PAIR( ExifDT, exifdt );
  BIND_PAIR( Inode, inode );
  BIND_PAIR( Md5ID, md5id );
  BIND_PAIR( Sd2ID, sd5id );
  BIND_PAIR( MimeID, mimeid );
  BIND_PAIR( ExifID, exifid );
  if ( duf_config->pu->glob )
  {
    DUF_SQL_BIND_S_OPT( GName, duf_config->pu->glob, r, pstmt );
  }
  if ( duf_config->pu->same_md5 )
  {
    duf_filepath_t fp = { 0 };
    {
      char *pathname;
      char *dir;
      char *base;

      pathname = mas_strdup( duf_config->pu->same_md5 );
      base = basename( pathname );
      dir = dirname( pathname );
      fp.dirid = duf_path2db( dir, NULL, &r );
      fp.name = mas_strdup( base );
      mas_free( pathname );
    }

    DUF_SQL_BIND_LL_NZ_OPT( GSamePathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAs, fp.name, r, pstmt );
    mas_free( fp.name );
    if ( r >= 0 && !fp.dirid )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  }

  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );

  DEBUG_ENDR( r );
}

int
duf_scan_beginning_ssql( duf_beginning_t * ssql, int bind, const char *title )
{
  DEBUG_STARTR( r );

  if ( ssql && !ssql->done )
  {
    const char **psql = ssql->sql;
    const char **psql0 = psql;

    while ( r >= 0 && psql && *psql )
    {
      int changes = 0;

      DUF_TRACE( sql, 0, "beginning psql : %s", *psql );
      /* r = duf_sql( *p, &changes ); */

      {
        DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
        if ( bind )
          DOR( r, duf_bind_ufilter( pstmt ) );
        if ( r >= 0 )
        {
          DUF_SQL_STEP( r, pstmt );
          DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
        }
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      DUF_TEST_R( r );
      /* DUF_TRACE( action, 2, "(%d) beginning psql %s; changes:%d", r, *psql, changes ); */
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": beginning SQL %lu; [%s] changes:%d; %s", title ? title : "no-title",
                 psql - psql0, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
    ssql->done++;

  }
  DEBUG_ENDR( r );
}
