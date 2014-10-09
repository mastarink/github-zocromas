#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_pdi.h"
/* #include "duf_levinfo.h" */
#include "duf_levinfo_ref.h"

#include "duf_item_scan2.h"

/* #include "duf_db2path.h" */
#include "duf_path2db.h"

#include "duf_sccb.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dir_scan2_stages.h"

#include "duf_ufilter.h"


/* #include "duf_item_match2.h" */

/* #include "duf_uni_scan2.h" */
/* ###################################################################### */
#include "duf_sccb_begfin.h"
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
  /* if ( duf_config->pu->size.flag )                                         */
  /* {                                                                        */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minSize, duf_config->pu->size.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxSize, duf_config->pu->size.max, r, pstmt ); */
  /* }                                                                        */
  BIND_PAIR( Same, same );
  /* if ( duf_config->pu->same.flag )                                         */
  /* {                                                                        */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minSame, duf_config->pu->same.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxSame, duf_config->pu->same.max, r, pstmt ); */
  /* }                                                                        */
  BIND_PAIR( ExifSame, exifsame );
  /* if ( duf_config->pu->exifsame.flag )                                             */
  /* {                                                                                */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minExifSame, duf_config->pu->exifsame.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxExifSame, duf_config->pu->exifsame.max, r, pstmt ); */
  /* }                                                                                */
  BIND_PAIR( NameID, nameid );
  /* if ( duf_config->pu->nameid.flag )                                           */
  /* {                                                                            */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minNameID, duf_config->pu->nameid.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxNameID, duf_config->pu->nameid.max, r, pstmt ); */
  /* }                                                                            */
  BIND_PAIR( DirID, dirid );
  /* if ( duf_config->pu->dirid.flag )                                          */
  /* {                                                                          */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minDirID, duf_config->pu->dirid.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxDirID, duf_config->pu->dirid.max, r, pstmt ); */
  /* }                                                                          */
  BIND_PAIR( MTime, mtime );
  /* if ( duf_config->pu->mtime.flag )                                          */
  /* {                                                                          */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minMTime, duf_config->pu->mtime.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxMTime, duf_config->pu->mtime.max, r, pstmt ); */
  /* }                                                                          */
  BIND_PAIR( ExifDT, exifdt );
  /* if ( duf_config->pu->exifdt.flag )                                           */
  /* {                                                                            */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minExifDT, duf_config->pu->exifdt.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxExifDT, duf_config->pu->exifdt.max, r, pstmt ); */
  /* }                                                                            */
  BIND_PAIR( Inode, inode );
  /* if ( duf_config->pu->inode.flag )                                          */
  /* {                                                                          */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minInode, duf_config->pu->inode.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( maxInode, duf_config->pu->inode.max, r, pstmt ); */
  /* }                                                                          */
  BIND_PAIR( 5 ID, md5id );
  /* if ( duf_config->pu->md5id.flag )                                        */
  /* {                                                                        */
  /*   DUF_SQL_BIND_LL_NZ_OPT( min5ID, duf_config->pu->md5id.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( max5ID, duf_config->pu->md5id.max, r, pstmt ); */
  /* }                                                                        */
  BIND_PAIR( 2 ID, sd5id );
  /* if ( duf_config->pu->sd5id.flag )                                        */
  /* {                                                                        */
  /*   DUF_SQL_BIND_LL_NZ_OPT( max2ID, duf_config->pu->sd5id.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( max2ID, duf_config->pu->sd5id.max, r, pstmt ); */
  /* }                                                                        */
  BIND_PAIR( MimeID, mimeid );
  /* if ( duf_config->pu->mimeid.flag )                                           */
  /* {                                                                            */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minMimeID, duf_config->pu->mimeid.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minMimeID, duf_config->pu->mimeid.max, r, pstmt ); */
  /* }                                                                            */
  BIND_PAIR( ExifID, exifid );
  /* if ( duf_config->pu->exifid.flag )                                           */
  /* {                                                                            */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minExifID, duf_config->pu->exifid.min, r, pstmt ); */
  /*   DUF_SQL_BIND_LL_NZ_OPT( minExifID, duf_config->pu->exifid.max, r, pstmt ); */
  /* }                                                                            */
  if ( duf_config->pu->glob )
  {
    DUF_SQL_BIND_S_OPT( GName, duf_config->pu->glob, r, pstmt );
  }
  if ( duf_config->pu->same_as )
  {
    duf_filepath_t fp = { 0 };
    {
      char *pathname;
      char *dir;
      char *base;

      pathname = mas_strdup( duf_config->pu->same_as );
      base = basename( pathname );
      dir = dirname( pathname );
      fp.dirid = duf_path2db( dir, &r );
      fp.name = mas_strdup( base );
      mas_free( pathname );
    }

    DUF_SQL_BIND_LL_NZ_OPT( GSamePathID, fp.dirid, r, pstmt );
    DUF_SQL_BIND_S_OPT( GSameAs, fp.name, r, pstmt );
    mas_free( fp.name );
    if ( r >= 0 && !fp.dirid )
      DOR( r, DUF_ERROR_NOT_FOUND );
  }

  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );

  DEBUG_ENDR( r );
}

int
duf_scan_beginning_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  const char **psql = sccb->beginning_sql_argv;

  while ( r >= 0 && psql && *psql )
  {
    int changes = 0;

    DUF_TRACE( sql, 0, "beginning psql : %s", *psql );
    /* r = duf_sql( *p, &changes ); */

    {
      DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
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
    DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": beginning SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
               psql - sccb->beginning_sql_argv, *psql, changes, r < 0 ? "FAIL" : "OK" );
    psql++;
  }
  DEBUG_ENDR( r );
}

int
duf_scan_final_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  /* if ( changes ) */
  {
    const char **psql = sccb->final_sql_argv;

    while ( r >= 0 && psql && *psql )
    {
      int changes = 0;

      DUF_TRACE( action, 2, "final psql : %s", *psql );
      /* r = duf_sql( *p, &changes ); */

      {
        DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
        /* r = duf_sql_exec( *psql, &changes ); */
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      DUF_TEST_R( r );
      /* DUF_TRACE( action, 2, "(%d) final psql %s; changes:%d", r, *psql, changes ); */
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": final SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
                 psql - sccb->final_sql_argv, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
  }
  DEBUG_ENDR( r );
}
