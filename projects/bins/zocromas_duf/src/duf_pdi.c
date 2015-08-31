#include <string.h>
#include <assert.h>

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

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

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
duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( !pdi->inited )
  {
    pdi->inited = 1;
    pdi->depth = -1;
    if ( real_path )
    {
      DORN( r, duf_pathdepth( real_path ) );
      if ( r >= 0 )
        pdi->topdepth = r;
    }
    assert( pdi->depth == -1 );
    DOR( r, duf_levinfo_create( pdi, pdi->topdepth, frecursive, opendir ) ); /* depth = -1 */
    assert( r < 0 || pdi->levinfo );
    /* assert( pdi->depth == -1 ); */
    if ( real_path )
      DOR( r, duf_real_path2db( pdi, caninsert /* caninsert */ , real_path, sql_set /* node_selector2 */  ) );
  }
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */ ,
                   int frecursive, int opendir )
{
  DEBUG_STARTR( r );

  DOR( r, duf_pdi_init( pdi, real_path, caninsert, sql_set /* node_selector2 */ , frecursive, opendir ) );

  DUF_TEST_R( r );
  if ( r >= 0 )
  {
    DUF_TRACE( explain, 1, "converted to real_path: %s", real_path );
    DUF_TRACE( explain, 0, "added path: %s", real_path );
    /* DUF_TRACE( path, 10, "diridpid: %llu", duf_levinfo_dirid( pdi ) ); */
  }
  /* TODO */ assert( pdi->levinfo );
  DEBUG_ENDR( r );
}
#endif

static int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_ufilter_t * pu,
                const duf_sql_set_t * sql_set /* const char *node_selector2 */ , int frecursive,
                int opendir )
{
  DEBUG_STARTR( r );
  int frec = 0;
  int od;

  assert( real_path && *real_path == '/' );
  /* frec = pdi && !frecursive ? duf_pdi_recursive( pdi ) : frecursive; */
  frec = pdi && frecursive < 0 ? duf_pdi_recursive( pdi ) : frecursive;
  od = pdi && pdi->opendir;
  duf_pdi_close( pdi );
  pdi->pu = pu;
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, real_path, caninsert /* caninsert */ , sql_set /* node_selector2 */ , frec /* frecursive */ ,
                                        od /* opendir */  ) );
  /*OR: return duf_pdi_init( pdi, real_path, 0 ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  real_path = duf_realpath( cpath, &r );
  {
    DUF_TRACE( temp, 2, "cpath:%s; real_path:%s", cpath, real_path );
    DOR( r, duf_pdi_reinit( pdi, real_path, caninsert /* caninsert */ , duf_config->pu /* pu */ , sql_set /* node_selector2 */ ,
                            DUF_U_FLAG( recursive ) /* recursive */ ,
                            duf_pdi_opendir( pdi ) ) );
  }
  mas_free( real_path );
  DEBUG_ENDR( r );
}

int
duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set /* const char *node_selector2 */ , int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  /* int frecursive = 0; */

  {
    const char *cpath = NULL;

    cpath = duf_levinfo_path( pdi );
    if ( cpath )
      path = mas_strdup( cpath );
    /* frecursive = pdi->recursive; */
  }
  DOR( r, duf_pdi_reinit_anypath( pdi, path, 0 /* canisert */ , sql_set /* node_selector2 *//*, duf_config->pu, frecursive */  ) );
  mas_free( path );
  DEBUG_ENDR( r );
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
