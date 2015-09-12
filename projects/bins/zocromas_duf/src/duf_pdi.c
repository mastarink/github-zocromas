#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */

#include "duf_config_ref.h"
#include "duf_option_defs.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
#include "duf_context.h"
#include "duf_path2dirid.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_pathinfo_ref.h"
#include "duf_pathinfo_credel.h"

#include "duf_ufilter_ref.h"


#include "duf_sql2.h"
#include "duf_maindb.h"
#include "duf_status_ref.h"

#include "duf_path2db.h"
#include "sql_tables_defs.h"
#include "duf_begfin.h"

#include "duf_selector.h"


#include "duf_pdi_attach.h"
/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */


/* 20150904.085609 */
int
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );
  DUF_TRACE( pdi, 0, "@@@frecursive:%d; real_path:%s", frecursive, real_path );
  if ( !pdi->inited )
  {
    /* assert( real_path ); */
    pdi->inited = 1;
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

      max_rel_depth = pdi && pdi->pu ? pdi->pu->max_rel_depth : 20;
      assert( pdi->pathinfo.depth == -1 );
      /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rel_depth ); */
      assert( max_rel_depth /* FIXME */  );
      {
        pdi->pathinfo.maxdepth = max_rel_depth + ( pdi->pathinfo.topdepth ? pdi->pathinfo.topdepth : 20 );
      }
    }
#else

#  if 0
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, pdi->pu ? pdi->pu->max_rel_depth : 0 );
#  else
    duf_pi_set_max_rel_depth( &pdi->pathinfo, real_path, duf_ufilter_max_rel_depth( duf_pdi_pu( pdi ) ) );
#  endif

#endif
    pdi->recursive = frecursive ? 1 : 0;
    pdi->opendir = opendir ? 1 : 0;
#if 0
    DOR( r, duf_levinfo_create( pdi, pdi->pathinfo.topdepth, frecursive, opendir ) ); /* depth = -1 */
#else
    DOR( r, duf_levinfo_create( pdi, duf_pdi_topdepth( pdi ), frecursive, opendir ) ); /* depth = -1 */
#endif
    DUF_TRACE( pdi, 0, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
    assert( r < 0 || pdi->pathinfo.levinfo );

  DORF( r, duf_main_db_open );
#ifdef DUF_ATTACH_SELECTED_PATTERN
    if ( pdi->pdi_name )
      DOR( r, duf_pdi_attach_selected( pdi ) );
#endif
    /* assert( pdi->pathinfo.depth == -1 ); */
    if ( real_path )
      DOR( r, duf_real_path2db( pdi, caninsert, real_path, sql_set ) );

    DUF_TRACE( pdi, 0, "@@@(frecursive:%d/%d) real_path:%s", frecursive, duf_pdi_recursive( pdi ), real_path );
  }

  DEBUG_ENDR( r );
}

/* 20150904.085510 */
int
duf_pdi_init_from_dirid( duf_depthinfo_t * pdi, unsigned long long dirid, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  path = duf_dirid2path( dirid, &r );
  DUF_TRACE( pdi, 8, "%d: %s", r, path );
  DOR( r, duf_pdi_init( pdi, path, caninsert, sql_set, frecursive, opendir ) );
  DUF_TRACE( pdi, 8, "%d: %s / %s", r, path, duf_levinfo_path( pdi ) );
  mas_free( path );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150904.085443 */
int
duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  DOR( r, duf_pdi_init( pdi, real_path, caninsert, sql_set, frecursive, opendir ) );

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

/* 20150904.090827 */
static int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_ufilter_t * pu,
                const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  int frec = 0;

  assert( pdi );
  assert( !real_path || *real_path == '/' );
  frec = frecursive < 0 ? duf_pdi_recursive( pdi ) : frecursive;
  DOR( r, duf_pdi_shut( pdi ) );
  pdi->pu = pu;
  DUF_TRACE( pdi, 0, "@@@frecursive:%d; duf_pdi_recursive( pdi ):%d; frec:%d; reinit real_path:%s", frecursive, duf_pdi_recursive( pdi ), frec,
             real_path );
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, real_path, caninsert, sql_set, frec, opendir ) );
  /*OR: return duf_pdi_init( pdi, real_path, 0 ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit_min( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  const char *rpath;

  rpath = duf_levinfo_path( pdi );
  DOR( r, duf_pdi_reinit( pdi, rpath, 0 /* caninsert */ , duf_pdi_pu( pdi ), NULL /* sql_set */ , -1, pdi->opendir ) );
  DEBUG_ENDR( r );
}

/* 20150904.091522 */
int
duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, int caninsert, const duf_sql_set_t * sql_set, int frecursive )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  if ( cpath )
  {
    real_path = duf_realpath( cpath, &r );
    {
      DUF_TRACE( pdi, 8, "@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
      assert( pdi->pdi_name );
      DUF_TRACE( pdi, 0, "@@@reinit_a real_path:%s", real_path );
      DOR( r, duf_pdi_reinit( pdi, real_path, caninsert, DUF_CONFIGG( pu ), sql_set, frecursive, duf_pdi_opendir( pdi ) ) );
      DUF_TRACE( pdi, 8, "@@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
    }
    mas_free( real_path );
  }
  DEBUG_ENDR( r );
}

/* 20150904.091517 */
int
duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  {
    const char *cpath = NULL;

    cpath = duf_levinfo_path( pdi );
    DUF_TRACE( pdi, 8, "@@(frecursive:%d/%d) cpath:%s", frecursive, duf_pdi_recursive( pdi ), cpath );
    if ( cpath )
      path = mas_strdup( cpath );
  }
  DOR( r, duf_pdi_reinit_anypath( pdi, path, 0 /* canisert */ , sql_set, frecursive ) );
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
    pdi->pu = 0;
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
    assert( !pdi->pu );
  }
  DEBUG_ENDR( r );
}

/* 20150904.085731 */
int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DOR( r, duf_pdi_shut( pdi ) );
  if ( pdi->pdi_name && pdi->db_attached_selected && !pdi->attached_copy )
  {
    assert( 0 == strcmp( pdi->db_attached_selected, pdi->pdi_name ) );
    DOR( r, duf_main_db_close( r ) );
#ifdef DUF_ATTACH_SELECTED_PATTERN
#  if 0
    DOR( r, duf_pdi_detach_selected( pdi ) );
#  endif
    {
      int ry DUF_UNUSED = 0;
      char *selected_db_file;

      selected_db_file = duf_expand_selected_db( DUF_ATTACH_SELECTED_PATTERN, pdi->db_attached_selected );
      T( "@@@@A selected_db_file:%s", selected_db_file );
#  if 0
      ry = unlink( selected_db_file );
      {
        char serr[1024] = "";
        char *s;

        s = strerror_r( errno, serr, sizeof( serr ) );
        DUF_SHOW_ERROR( "(%d) errno:%d unlink :%s;", ry, errno, s ? s : serr );
      }
#  endif
      T( "@@@@@B selected_db_file:%s", selected_db_file );
      mas_free( selected_db_file );
    }
#endif
  }
  mas_free( pdi->db_attached_selected );
  pdi->db_attached_selected = NULL;
  DEBUG_ENDR( r );
}
