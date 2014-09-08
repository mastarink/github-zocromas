#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */
#include "duf_status_ref.h"

#include "duf_config_ref.h"

#include "duf_sql2.h"

#include "duf_levinfo.h"

#include "duf_path2db.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */

void
duf_clear_context( duf_context_t * pcontext )
{
  if ( pcontext )
  {
    if ( pcontext->destructor )
      ( pcontext->destructor ) ( pcontext->ptr );
    else
      mas_free( pcontext->ptr );
  }

  pcontext->ptr = NULL;
}

void *
duf_context( duf_context_t * pcontext )
{
  return pcontext ? pcontext->ptr : NULL;
}

void
duf_set_context( duf_context_t * pcontext, void *ptr )
{
  duf_clear_context( pcontext );
  assert( pcontext );
  pcontext->ptr = ptr;
}

void
duf_set_context_destructor( duf_context_t * pcontext, duf_void_voidp_t destr )
{
  assert( pcontext );
  pcontext->destructor = destr;
}

int
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int ifadd )
{
  DEBUG_STARTR( r );

  if ( !pdi->inited )
  {
    int pd;

    /* assert( pdi ); */
    pdi->inited = 1;

    pd = duf_pathdepth( real_path, &r );
    DUF_TEST_R( r );

    DOR( r, duf_levinfo_create( pdi, pd ) ); /* depth = -1 */
    assert( r < 0 || pdi->levinfo );
    /* assert( pdi->depth == -1 ); */
    DOR( r, duf_real_path2db( pdi, real_path, ifadd /* ifadd */  ) );
  }
  DEBUG_ENDR( r );
}

int
duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int ifadd )
{
  DEBUG_STARTR( r );

  r = duf_pdi_init( pdi, real_path, ifadd );
  if ( r == DUF_ERROR_NOT_IN_DB )
    DUF_ERROR( "not in db:'%s'", real_path );
  else if ( r < 0 )
    DUF_ERROR( "path:%s", real_path );
  DUF_TEST_R( r );
  if ( r >= 0 )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
    DUF_TRACE( path, 0, " *********** diridpid: %llu", duf_levinfo_dirid( pdi ) );
  }
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, duf_ufilter_t * pu )
{
  duf_pdi_close( pdi );
  pdi->u = *pu;
  pdi->depth = -1;
  return duf_pdi_init_wrap( pdi, real_path, 0 );
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

    for ( int i = 0; i < pdi->num_idstatements; i++ )
      r = duf_pdi_finalize_idstmt( pdi, i );

    mas_free( pdi->idstatements );
    pdi->idstatements = NULL;

    /* mas_free( pdi->xstatements ); */
    /* pdi->xstatements = NULL;      */

    pdi->num_idstatements = 0;
    duf_levinfo_delete( pdi );
    global_status.changes += pdi->changes;
    pdi->inited = 0;
  }
  /* DUF_ERROR( "clear idstatements" ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->u.max_seq && pdi->seq >= pdi->u.max_seq )
    r = DUF_ERROR_MAX_SEQ_REACHED;
  else if ( pdi->u.maxitems.files && pdi->items.files >= pdi->u.maxitems.files )
    r = DUF_ERROR_MAX_REACHED;
  else if ( pdi->u.maxitems.dirs && pdi->items.dirs >= pdi->u.maxitems.dirs )
    r = DUF_ERROR_MAX_REACHED;
  else if ( pdi->u.maxitems.total && pdi->items.total >= pdi->u.maxitems.total )
    r = DUF_ERROR_MAX_REACHED;

  /* rv = ( ( !pdi->u.max_seq || pdi->seq <= pdi->u.max_seq )                                  */
  /*        && ( !pdi->u.maxitems.files || ( pdi->items.files ) < pdi->u.maxitems.files )    */
  /*        && ( !pdi->u.maxitems.dirs || ( pdi->items.dirs ) < pdi->u.maxitems.dirs )       */
  /*        && ( !pdi->u.maxitems.total || ( pdi->items.total ) < pdi->u.maxitems.total ) ); */
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
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->depth : 0;
}

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

int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi )
{
  return duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi );
}

duf_idstmt_t *
duf_pdi_find_idstmt( duf_depthinfo_t * pdi, int *pindex )
{
  duf_idstmt_t *is = NULL;

  if ( pdi->idstatements )
  {
    for ( int i = 0; i < pdi->num_idstatements; i++ )
    {
      /* T( "i:%d %p : %p", i, pdi->idstatements[i].id, pindex ); */
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
    r = duf_sql_finalize( pdi->idstatements[i].pstmt );
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
    r = duf_sql_finalize( is->pstmt );
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
