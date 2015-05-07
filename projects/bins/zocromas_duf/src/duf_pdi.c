#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */
#include "duf_status_ref.h"

#include "duf_config_ref.h"
#include "duf_option_defs.h"

#include "duf_sql2.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
/* #include "duf_levinfo_context.h" */
#include "duf_context.h"

#include "duf_path2db.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */


duf_depthinfo_t *
duf_pdi_create( void )
{
  duf_depthinfo_t *pdi;

  pdi = mas_malloc( sizeof( duf_depthinfo_t ) );
  memset( pdi, 0, sizeof( duf_depthinfo_t ) );

  return pdi;
}

int
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int tag, int caninsert, const char *node_selector2, int recursive )
{
  DEBUG_STARTR( r );

  if ( !pdi->inited )
  {
    /* memset( pdi, 0, sizeof( duf_depthinfo_t ) ); */
    /* assert( pdi ); */
    pdi->inited = 1;
    pdi->depth = -1;
    DORN( r, duf_pathdepth( real_path ) );
    if ( r >= 0 )
      pdi->topdepth = r;
    assert( pdi->depth == -1 );
    DOR( r, duf_levinfo_create( pdi, r, recursive ) ); /* depth = -1 */
    assert( r < 0 || pdi->levinfo );
    /* assert( pdi->depth == -1 ); */
    DOR( r, duf_real_path2db( pdi, real_path, tag, caninsert /* caninsert */ , node_selector2 ) );
  }
  DEBUG_ENDR( r );
}

int
duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int tag, int caninsert, const char *node_selector2, int recursive )
{
  DEBUG_STARTR( r );

  DOR( r, duf_pdi_init( pdi, real_path, tag, caninsert, node_selector2, recursive ) );
  /*
     if ( r == DUF_ERROR_NOT_IN_DB )
     DUF_SHOW_ERROR( "not in db:'%s'", real_path );
     else if ( r < 0 )
     DUF_SHOW_ERROR( "path:%s", real_path );
   */
  DUF_TEST_R( r );
  if ( r >= 0 )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
    DUF_TRACE( path, 10, "diridpid: %llu", duf_levinfo_dirid( pdi ) );
  }
  /* TODO */ assert( pdi->levinfo );
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu, const char *node_selector2, int recursive )
{
  int rec = 0;

  assert( real_path && *real_path == '/' );
  /* rec = pdi && !recursive ? duf_pdi_recursive( pdi ) : recursive; */
  rec = pdi && recursive < 0 ? duf_pdi_recursive( pdi ) : recursive;
  duf_pdi_close( pdi );
  pdi->pu = pu;
  return duf_pdi_init_wrap( pdi, real_path, 0 /* tag */ , 0 /* caninsert */ , node_selector2, rec );
  /*OR: return duf_pdi_init( pdi, real_path, 0 ); */
}

int
duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const char *node_selector2 /*, const duf_ufilter_t * pu, int recursive */  )
{
  DEBUG_STARTR( r );
  char *path = NULL;
  char *real_path = NULL;

  if ( *cpath == '/' )
    path = mas_strdup( cpath );
  else
  {
    path = duf_levinfo_path_qdup( pdi, NULL );
    if ( !( cpath[0] == '.' && !cpath[1] ) )
    {
      path = mas_strcat_x( path, "/" );
      path = mas_strcat_x( path, cpath );
    }
  }
  real_path = duf_realpath( path, &r );
  if ( r >= 0 )
    duf_pdi_reinit( pdi, real_path, duf_config->pu /* pu */ , node_selector2, DUF_U_FLAG( recursive ) /*recursive */  );
  mas_free( path );
  mas_free( real_path );
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const char *node_selector2, int recursive )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  /* int recursive = 0; */

  {
    const char *cpath = NULL;

    cpath = duf_levinfo_path( pdi );
    if ( cpath )
      path = mas_strdup( cpath );
    /* recursive = pdi->recursive; */
  }
  DOR( r, duf_pdi_reinit_anypath( pdi, path, node_selector2 /*, duf_config->pu, recursive */  ) );
  mas_free( path );
  DEBUG_ENDR( r );
}

void
duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx )
{
  assert( pdi );
  duf_set_context( &pdi->context, ctx );
}

void
duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr )
{
  assert( pdi );
  duf_set_context_destructor( &pdi->context, destr );
}

void *
duf_pdi_context( duf_depthinfo_t * pdi )
{
  assert( pdi );
  return duf_context( &pdi->context );
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  DEBUG_STARTR( r );

  assert( pdi );
  r = pdi->opendir;
  pdi->opendir = od;
  DEBUG_ENDRN( r );
}

int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->inited )
  {
    duf_clear_context( &pdi->context );

    for ( int i = 0; /* r>=0 && */ i < pdi->num_idstatements; r = 0, i++ )
      DOR( r, duf_pdi_finalize_idstmt( pdi, i ) );

    mas_free( pdi->idstatements );
    pdi->idstatements = NULL;

    /* mas_free( pdi->xstatements ); */
    /* pdi->xstatements = NULL;      */

    pdi->num_idstatements = 0;
    duf_levinfo_delete( pdi );
    /* global_status.changes += pdi->changes; */
    pdi->inited = 0;
    pdi->opendir = 0;
    pdi->depth = pdi->topdepth = 0;
    pdi->maxdepth = 0;
    pdi->changes = 0;
    pdi->pu = 0;
    pdi->items.dirs = pdi->items.files = pdi->items.total = 0;
    pdi->seq = pdi->seq_node = pdi->seq_leaf = 0;
    assert( !pdi->inited );
    assert( !pdi->opendir );
    assert( !pdi->items.files );
    assert( !pdi->items.dirs );
    assert( !pdi->items.total );
    assert( !pdi->maxdepth );
    assert( !pdi->topdepth );
    assert( !pdi->depth );
    assert( !pdi->levinfo );
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
  /* DUF_SHOW_ERROR( "clear idstatements" ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->pu->max_seq && pdi->seq >= pdi->pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pu->maxitems.files && pdi->items.files >= pdi->pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.dirs && pdi->items.dirs >= pdi->pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.total && pdi->items.total >= pdi->pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );

  /* rv = ( ( !pdi->pu->max_seq || pdi->seq <= pdi->pu->max_seq )                                  */
  /*        && ( !pdi->pu->maxitems.files || ( pdi->items.files ) < pdi->pu->maxitems.files )    */
  /*        && ( !pdi->pu->maxitems.dirs || ( pdi->items.dirs ) < pdi->pu->maxitems.dirs )       */
  /*        && ( !pdi->pu->maxitems.total || ( pdi->items.total ) < pdi->pu->maxitems.total ) ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

int
duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? d - pdi->topdepth : 0;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->depth : 0;
}

/* pdi->depth - pdi->topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pdi_deltadepth( pdi, pdi->depth ) : 0;
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
  if ( pdi )
    pdi->topdepth = pdi->depth;
}

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->topdepth : 0;
}

int
duf_pdi_maxdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->maxdepth : 0;
}

/* pdi->topdepth + pdi->depth - pdi->topdepth === pdi->depth */
int
duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d )
{
  int r = 0;

  if ( duf_pdi_recursive( pdi ) )
    r = d - duf_pdi_maxdepth( pdi ) < delta; /* d - maxdepth < delta */
  else
    r = duf_pdi_deltadepth( pdi, d ) <= delta; /* d - topdepth <= delta */
  /* r= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return r;
}

int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta )
{
  return duf_pdi_is_good_depth_d( pdi, delta, pdi->depth );
}

duf_idstmt_t *
duf_pdi_find_idstmt( duf_depthinfo_t * pdi, int *pindex )
{
  duf_idstmt_t *is = NULL;

  if ( pdi->idstatements )
  {
    for ( int i = 0; i < pdi->num_idstatements; i++ )
    {
      if ( pdi->idstatements[i].id == pindex )
      {
        is = &pdi->idstatements[i];
        break;
      }
    }
  }
  return is;
}

/* must be checked it's absent */
duf_sqlite_stmt_t *
duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex, int *pr )
{
  DEBUG_STARTR( r );
  duf_sqlite_stmt_t *pstmt = NULL;
  duf_idstmt_t *is = NULL;

  assert( pdi );
  is = duf_pdi_find_idstmt( pdi, pindex );
  if ( !is )
  {
    if ( !pdi->num_idstatements )
    {
      pdi->idstatements = mas_malloc( sizeof( duf_idstmt_t ) );
      /* pdi->xstatements = mas_malloc( sizeof( int * ) ); */
    }
    else
    {
      pdi->idstatements = mas_realloc( pdi->idstatements, ( pdi->num_idstatements + 1 ) * sizeof( duf_idstmt_t ) );
      /* pdi->xstatements = mas_realloc( pdi->xstatements, ( pdi->num_idstatements + 1 ) * sizeof( int * ) ); */
    }
    pdi->idstatements[pdi->num_idstatements].pstmt = NULL;
    pdi->idstatements[pdi->num_idstatements].id = NULL;
    /* if ( pindex )                      */
    /*   *pindex = pdi->num_idstatements; */
    /* pdi->xstatements[pdi->num_idstatements] = pindex; */
    is = &( pdi->idstatements[pdi->num_idstatements] );
    pdi->num_idstatements++;
  }
  DOR( r, duf_sql_prepare( sql, &pstmt ) );
  if ( pstmt )
  {
    is->id = pindex;
    is->pstmt = pstmt;
  }

  if ( pr )
    *pr = r;
  return pstmt;
  DEBUG_END(  );
}

duf_sqlite_stmt_t *
duf_pdi_find_statement( duf_depthinfo_t * pdi, int *pindex )
{
  duf_idstmt_t *is = NULL;

  if ( pdi && pdi->idstatements )
    is = duf_pdi_find_idstmt( pdi, pindex );
  return is ? is->pstmt : NULL;
}

int
duf_pdi_finalize_idstmt( duf_depthinfo_t * pdi, int i )
{
  DEBUG_STARTR( r );
  /* int *pi; */

  assert( pdi );

  if ( pdi->idstatements[i].pstmt )
    DOR( r, duf_sql_finalize( pdi->idstatements[i].pstmt ) );
  /* if ( pdi->xstatements[i] )  */
  /*   pi = pdi->xstatements[i]; */
  /* if ( pi )                   */
  /*   *pi = -1;                 */
  pdi->idstatements[i].pstmt = NULL;
  DEBUG_ENDR( r );
}

int
duf_pdi_finalize_statement( duf_depthinfo_t * pdi, int *pindex )
{
  DEBUG_STARTR( r );
  duf_idstmt_t *is = NULL;

  is = duf_pdi_find_idstmt( pdi, pindex );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
    is->pstmt = NULL;
  }

  DEBUG_ENDR( r );
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
