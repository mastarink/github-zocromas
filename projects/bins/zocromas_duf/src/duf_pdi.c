#include <string.h>

#include "duf_maintenance.h"

#include "duf_option_defs.h"

#include "duf_config_util.h"
#include "duf_config_defs.h"

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
#include "duf_context.h"
#include "duf_path2dirid.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_pathinfo_credel.h"
#include "duf_ufilter_ref.h"

#include "sql_tables_defs.h"

#include "duf_maindb.h"

#include "duf_path2db.h"

#include "evsql_selector.h"
/* ###################################################################### */
#include "duf_pdi_attach.h"
#include "duf_pdi.h"
/* ###################################################################### */


/* 20150904.085609 */
DUF_WRAPSTATIC int
duf_pdi_init( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path, const duf_sql_set_t * sql_set,
              int caninsert, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );
  DUF_TRACE( pdi, 0, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
  DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  if ( !pdi->inited )
  {
    /* assert( real_path ); */
    pdi->inited = 1;
    pdi->pup = pu;
    DUF_TRACE( pdi, 0, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
#if 0
    pdi->pathinfo.depth = -1;
    if ( real_path )
    {
      DOR( r, duf_pathdepth( real_path ) );
      if ( DUF_NOERROR( r ) )
        pdi->pathinfo.topdepth = r;
    }
    assert( pdi->pathinfo.depth == -1 );
    {
      int max_rel_depth = 0;

      max_rel_depth = pdi && pdi->pup ? pdi->pup->max_rel_depth : 20;
      assert( pdi->pathinfo.depth == -1 );
      /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rel_depth ); */
      assert( max_rel_depth /* FIXME */  );
      {
        pdi->pathinfo.maxdepth = max_rel_depth + ( pdi->pathinfo.topdepth ? pdi->pathinfo.topdepth : 20 );
      }
    }
#else

#  if 0
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, pdi->pup ? pdi->pup->max_rel_depth : 0 );
#  else
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, duf_ufilter_max_rel_depth( duf_pdi_pu( pdi ) ) );
#  endif

#endif
    pdi->recursive = frecursive ? 1 : 0;
    pdi->opendir = opendir ? 1 : 0;

    DOR( r, duf_levinfo_create( pdi ) ); /* depth = -1 */

    DUF_TRACE( pdi, 0, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
    assert( r < 0 || pdi->pathinfo.levinfo );

#ifdef DUF_ATTACH_SELECTED_PATTERN
#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
    if ( pdi->pdi_name )
    {
      DOR( r, duf_pdi_attach_selected( pdi ) );
    }
#else
#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#endif
    /* assert( pdi->pathinfo.depth == -1 ); */
    if ( real_path )
    {
      /* T( "real_path:%p:%s", real_path, real_path ); */
      DOR( r, duf_real_path2db( pdi, caninsert, real_path, sql_set ) );
      if ( !pdi->pyp )
      {
        pdi->pyp = mas_malloc( sizeof( *pdi->pyp ) );
        pdi->pyp_created = 1;
        memset( pdi->pyp, 0, sizeof( *pdi->pyp ) );
        pdi->pyp->topdirid = duf_levinfo_dirid( pdi );
        /* T( "(ci:%d) topdirid:%llu for '%s' - '%s'", caninsert, pdi->pyp->topdirid, real_path, duf_levinfo_path( pdi ) ); */
      }
    }
    DUF_TRACE( pdi, 0, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
  }
  DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150904.085443 */
int
DUF_WRAPPED( duf_pdi_init ) ( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, const char *real_path,
                              const duf_sql_set_t * sql_set, int caninsert, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  DUF_TRACE( pdi, 0, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
  /* T( "real_path:%p:%s", real_path, real_path ); */
  DOR( r, duf_pdi_init( pdi, pu, real_path, sql_set, caninsert, frecursive, opendir ) );

  if ( DUF_NOERROR( r ) )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
    /* DUF_TRACE( path, 10, "diridpid: %llu", duf_levinfo_dirid( pdi ) ); */
  }
  /* TODO */ assert( duf_pdi_levinfo( pdi ) );
  DEBUG_ENDR( r );
}
#endif

/* 20150914.114501 */
int
duf_pdi_init_min( duf_depthinfo_t * pdi, const char *real_path )
{
  DEBUG_STARTR( r );

  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, ( duf_ufilter_t * ) NULL, real_path, NULL /* sql_set */ , 0 /* caninsert */ ,
                                        1 /* recursive */ ,
                                        0 /* opendir */  ) );
  DEBUG_ENDR( r );
}

/* 20150914.114508 */
int
duf_pdi_init_at_config( void )
{
  DEBUG_STARTR( r );
  DUF_TRACE( pdi, 0, "@@@recursive:%d; NO real_path", DUF_UG_FLAG( recursive ) );
#if 0
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( DUF_CONFIGG( scn.pdi ), DUF_CONFIGG( scn.pdi )->pup, NULL /* real_path */ , NULL /* sql_set */ ,
                                        0 /* caninsert */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#else
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( DUF_CONFIGG( scn.pdi ), DUF_CONFIGG( scn.puz ), NULL /* real_path */ , NULL /* sql_set */ ,
                                        0 /* caninsert */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#endif
  assert( DUF_CONFIGX( scn.pdi )->pup == DUF_CONFIGX( scn.puz ) );
  DEBUG_ENDR( r );
}

/* 20150904.085510 */
int
duf_pdi_init_from_dirid( duf_depthinfo_t * pdi, const duf_ufilter_t * pu, unsigned long long dirid,
                         const duf_sql_set_t * sql_set, int caninsert, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  path = duf_dirid2path( dirid, &r );
  DUF_TRACE( pdi, 0, "@@@dirid:%llu; frecursive:%d; real_path:%s", dirid, frecursive, path );
  DOR( r, duf_pdi_init( pdi, pu, path, sql_set, caninsert, frecursive, opendir ) );
  DUF_TRACE( pdi, 8, "%d: %s / %s", r, path, duf_levinfo_path( pdi ) );
  mas_free( path );
  DEBUG_ENDR( r );
}

int
duf_pdi_shut( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->inited )
  {

    assert( pdi->pathinfo.depth + 1 == duf_levinfo_count( pdi ) );

    duf_clear_context( &pdi->context );
    DOR( r, duf_levinfo_delete( pdi ) );

    for ( int i = 0; i < pdi->num_idstatements; r = 0, i++ )
      DOR( r, duf_pdi_finalize_idstmt( pdi, i ) );

    mas_free( pdi->idstatements );
    pdi->idstatements = NULL;

    pdi->num_idstatements = 0;
    pdi->inited = 0;
    pdi->opendir = 0;
#if 0
    pdi->pathinfo.depth = pdi->pathinfo.topdepth = 0;
    pdi->pathinfo.maxdepth = 0;
    assert( !pdi->pathinfo.maxdepth );
    assert( !pdi->pathinfo.topdepth );
    assert( !pdi->pathinfo.depth );
    assert( !pdi->pathinfo.levinfo );
#else
    DOR( r, duf_pi_shut( &pdi->pathinfo ) );
#endif
    pdi->changes = 0;
    pdi->pup = NULL;
    if ( pdi->pyp_created )
    {
      assert( pdi->pyp );
      mas_free( pdi->pyp );
    }
    pdi->pyp_created = 0;
    pdi->pyp = NULL;
    pdi->items.dirs = pdi->items.files = pdi->items.total = 0;
    pdi->seq = pdi->seq_node = pdi->seq_leaf = 0;
    assert( !pdi->inited );
    assert( !pdi->opendir );
    assert( !pdi->items.files );
    assert( !pdi->items.dirs );
    assert( !pdi->items.total );
    assert( !pdi->changes );
    assert( !pdi->seq_node );
    assert( !pdi->seq_leaf );
    assert( !pdi->seq );
    assert( !pdi->context.ptr );
    assert( !pdi->context.destructor );
    assert( !pdi->num_idstatements );
    assert( !pdi->idstatements );
    assert( !pdi->pup );
    assert( !pdi->pyp );
  }
  DEBUG_ENDR( r );
}

/* 20150904.085731 */
int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi->pathinfo.depth + 1 == duf_levinfo_count( pdi ) );

  DOR( r, duf_pdi_shut( pdi ) );
  if ( pdi->pdi_name && pdi->db_attached_selected && !pdi->attached_copy )
  {
    assert( 0 == strcmp( pdi->db_attached_selected, pdi->pdi_name ) );
    DOR( r, duf_main_db_close( r ) );
#ifdef DUF_ATTACH_SELECTED_PATTERN
#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif

#  if 0
    DOR( r, duf_pdi_detach_selected( pdi ) );
#  endif
    {
      int ry DUF_UNUSED = 0;
      char *selected_db_file;

      selected_db_file = duf_expand_sql( DUF_ATTACH_SELECTED_PATTERN, pdi->db_attached_selected );
#  if 0
      ry = unlink( selected_db_file );
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
        _DUF_SHOW_ERROR( "(%d) errno:%d unlink :%s;", ry, errno, s ? s : serr );
      }
#  endif
      mas_free( selected_db_file );
    }
#else
#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#endif
  }
  mas_free( pdi->db_attached_selected );
  pdi->db_attached_selected = NULL;
  DEBUG_ENDR( r );
}
