#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>



#include "duf_types.h"
#include "duf_errors_headers.h"


#include "duf_utils.h"
#include "duf_utils_path.h"

#include "duf_config_ref.h"

#include "duf_sql2.h"
#include "duf_path.h"


#include "duf_levinfo.h"

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
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path )
{
  int r = 0;
  int pd;

  pdi->inited = 1;
  pd = duf_pathdepth( real_path, &r );
  
  if ( r >= 0 )
    r = duf_levinfo_create( pdi, pd );

  if ( r >= 0 )
    r = duf_real_path2db( pdi, real_path, 0 /* ifadd */  );

  return r;
}

int
duf_pdi_init_msg( duf_depthinfo_t * pdi, const char *real_path )
{
  int r = 0;

  r = duf_pdi_init( pdi, real_path );
  if ( r == DUF_ERROR_NOT_IN_DB )
    DUF_ERROR( "not in db:'%s'", real_path );

  DUF_TRACE( action, 0, "real_path:%s", real_path );
  DUF_TRACE( explain, 0, "converted to real_path: %s", real_path );
  DUF_TRACE( path, 0, " *********** dirid: %llu", duf_levinfo_dirid( pdi ) );
  DUF_TRACE( explain, 0, "added path uni: %s", real_path );

  return r;
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
  int r = 0;

  assert( pdi );
  r = pdi->opendir;
  pdi->opendir = od;
  return r;
}

int
duf_pdi_close( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  duf_clear_context( &pdi->context );
  for ( int i = 0; i < pdi->num_statements; i++ )
  {
    r = duf_pdi_finalize( pdi, i );
  }
  mas_free( pdi->statements );
  pdi->statements = NULL;
  mas_free( pdi->xstatements );
  pdi->xstatements = NULL;
  pdi->num_statements = 0;
  duf_levinfo_delete( pdi );
  /* DUF_ERROR( "clear statements" ); */
  return r;
}

int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  int r = 0;

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
  return r;
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

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->topdepth : 0;
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
  if ( pdi )
    pdi->topdepth = pdi->depth;
}

int
duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex )
{
  int r = -1;

  if ( pdi )
  {
    if ( !pdi->num_statements )
    {
      pdi->statements = mas_malloc( sizeof( duf_sqlite_stmt_t * ) );
      pdi->xstatements = mas_malloc( sizeof( int * ) );
    }
    else
    {
      pdi->statements = mas_realloc( pdi->statements, ( pdi->num_statements + 1 ) * sizeof( duf_sqlite_stmt_t * ) );
      pdi->xstatements = mas_realloc( pdi->xstatements, ( pdi->num_statements + 1 ) * sizeof( int * ) );
    }
    if ( pindex )
      *pindex = pdi->num_statements;
    pdi->xstatements[pdi->num_statements] = pindex;
    r = duf_sql_prepare( sql, &pdi->statements[pdi->num_statements] );
    pdi->num_statements++;
  }
  return r;
}

duf_sqlite_stmt_t *
duf_pdi_statement( duf_depthinfo_t * pdi, int i )
{
  return pdi && pdi->statements ? pdi->statements[i] : NULL;
}

int
duf_pdi_finalize( duf_depthinfo_t * pdi, int i )
{
  int r = 0;
  int *pi;

  assert( pdi );

  if ( pdi->statements[i] )
    r = duf_sql_finalize( pdi->statements[i] );
  if ( pdi->xstatements[i] )
    pi = pdi->xstatements[i];
  if ( pi )
    *pi = -1;
  pdi->statements[i] = NULL;
  return r;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
