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
#include "duf_dir_scan2.h"
/* ###################################################################### */

static int
duf_bind_ufilter( duf_sqlite_stmt_t * pstmt )
{
  DEBUG_STARTR( r );

  if ( r >= 0 && duf_config->pu->size.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minSize, duf_config->pu->size.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxSize, duf_config->pu->size.max, r, pstmt );
    DUF_PRINTF( 0, "binding size %llu .. %llu", duf_config->pu->size.min, duf_config->pu->size.max );
  }
  if ( r >= 0 && duf_config->pu->same.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minSame, duf_config->pu->same.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxSame, duf_config->pu->same.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->exifsame.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minExifSame, duf_config->pu->exifsame.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxExifSame, duf_config->pu->exifsame.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->nameid.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minNameID, duf_config->pu->nameid.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxNameID, duf_config->pu->nameid.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->dirid.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minDirID, duf_config->pu->dirid.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxDirID, duf_config->pu->dirid.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->mtime.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minMTime, duf_config->pu->mtime.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxMTime, duf_config->pu->mtime.max, r, pstmt );
#if 0
    {
      char buf1[512];
      char buf2[512];

      duf_strfgmtime( buf1, sizeof( buf1 ), "%a, %d %b %Y %T %z", ( time_t ) duf_config->pu->mtime.min );
      duf_strfgmtime( buf2, sizeof( buf2 ), "%a, %d %b %Y %T %z", ( time_t ) duf_config->pu->mtime.max );
      DUF_PRINTF( 0, "binding mtime %llu .. %llu: %s .. %s", duf_config->pu->mtime.min, duf_config->pu->mtime.max, buf1, buf2 );
    }
#endif
  }
  if ( r >= 0 && duf_config->pu->exifdt.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minExifDT, duf_config->pu->exifdt.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxExifDT, duf_config->pu->exifdt.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->inode.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minInode, duf_config->pu->inode.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( maxInode, duf_config->pu->inode.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->md5id.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( min5ID, duf_config->pu->md5id.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( max5ID, duf_config->pu->md5id.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->sd5id.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( max2ID, duf_config->pu->sd5id.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( max2ID, duf_config->pu->sd5id.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->mimeid.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minMimeID, duf_config->pu->mimeid.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( minMimeID, duf_config->pu->mimeid.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->exifid.flag )
  {
    DUF_SQL_BIND_LL_NZ_OPT( minExifID, duf_config->pu->exifid.min, r, pstmt );
    DUF_SQL_BIND_LL_NZ_OPT( minExifID, duf_config->pu->exifid.max, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->glob )
  {
    DUF_SQL_BIND_S_OPT( GName, duf_config->pu->glob, r, pstmt );
  }
  if ( r >= 0 && duf_config->pu->same_as )
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

    if ( r >= 0 )
      DUF_SQL_BIND_LL_NZ_OPT( GSamePathID, fp.dirid, r, pstmt );
    if ( r >= 0 )
      DUF_SQL_BIND_S_OPT( GSameAs, fp.name, r, pstmt );
    mas_free( fp.name );
    if ( r >= 0 && !fp.dirid )
      r = DUF_ERROR_NOT_FOUND;
  }

  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );

  DEBUG_ENDR( r );
}

static int
duf_scan_beginning_sql( duf_scan_callbacks_t * sccb )
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



/* duf_scan_dirs_by_pdi:
 *
 * consecutively call various scanner stages with the sccb
 * on current dir from pdi
 * 
 * i.e. [ evaluate sccb for current dir ]
 *
 * args:
 *   pstmt: sql(ite) prepared statement
 *   pdi:  depthinfo structure ptr
 *   sccb: module callbacs structure
 *
 * */
static int
duf_scan_dirs_by_pdi( duf_sqlite_stmt_t * pstmt_selector, /* duf_str_cb2_t str_cb2_unused, */ duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  assert( sccb );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  if ( !global_status.selection_done )
  {
    DOR( r, duf_scan_beginning_sql( sccb ) );
    /* if ( r >= 0 ) */
    global_status.selection_done = 1;
  }
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 * */
  DOR( r, duf_qscan_dirents2( pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  DOR( r, duf_count_db_items2( NULL /* duf_match_leaf2 */ , pdi, sccb, &sccb->leaf ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  DOR( r, duf_qscan_node_scan_before2(  /*        */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  DOR( r, duf_qscan_files_by_dirid2(  /*          */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  DOR( r, duf_qscan_node_scan_middle2(  /*        */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
/* assert( duf_scan_dirs_by_pdi_maxdepth == str_cb2_unused ); */
  /* if ( DUF_U_FLAG( recursive ) ) */
  DOR( r, duf_qscan_dirs_by_dirid2(  /*         */ pstmt_selector, pdi, sccb /* , duf_scan_dirs_by_pdi_maxdepth *//* str_cb2_unused */  ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );
  DOR( r, duf_qscan_node_scan_after2(  /*         */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( pdi ) );

  DEBUG_ENDR( r );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *
 * see duf_scan_dirs_by_pdi
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */

int
duf_scan_dirs_by_pdi_wrap( duf_sqlite_stmt_t * pstmt_selector, /* duf_str_cb2_t str_cb2_unused, */ duf_depthinfo_t * pdi,
                           duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( pdi );

  /* assert( duf_scan_dirs_by_pdi_maxdepth == str_cb2_unused ); */

  DUF_TRACE( scan, 3, "[%llu]", diridpid );
  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, "** depth:%d/%d; diridpid:%llu", duf_pdi_depth( pdi ),
                duf_pdi_reldepth( pdi ), diridpid );

  DUF_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( pdi ) );

  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 1." );

  DOR( r, duf_scan_dirs_by_pdi( pstmt_selector, /* str_cb2_unused, */ pdi, sccb ) );

  DUF_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( pdi ) );
  DEBUG_ENDR( r );
}
