/* #undef MAS_TRACING */
#include <assert.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_sql_prepared.h"
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ♠ */

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */

static duf_idstmt_t *
duf_pdi_find_idstmt( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /*, const int *pindex */  )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  if ( pdi->idstatements )
  {
    for ( int i = 0; i < pdi->num_idstatements; i++ )
    {
    /* T( "@@@%d ? %d = %d", pdi->idstatements[i].id, stmtid (* pindex *) , pdi->idstatements[i].id == stmtid (* pindex *)  ); */
      if ( pdi->idstatements[i].id == stmtid /* pindex */  )
      {
        is = &pdi->idstatements[i];
      /* T( "@@@@is:%p -> %p", is, is ? is->pstmt : NULL ); */
        break;
      }
    }
  }
  return is;
}

static duf_idstmt_t *
duf_pdi_find_idstmt_by_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  if ( pdi->idstatements )
  {
    for ( int i = 0; i < pdi->num_idstatements; i++ )
    {
      if ( pdi->idstatements[i].pstmt == pstmt )
      {
        is = &pdi->idstatements[i];
        break;
      }
    }
  }
  return is;
}

/* must be checked it's absent */
duf_stmnt_t *
duf_pdi_prepare_statement_by_id( duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid /*, const int *pindex */ , int *pr )
{
  duf_stmnt_t *pstmt = NULL;

  DUF_START(  );

  int rpr = 0;
  duf_idstmt_t *is = NULL;
  duf_idstmt_t *chkids = NULL;

  assert( pdi );
  DUF_TRACE( sql, 4, "@@@@@(%d:%s): %s", rpr, mas_error_name_i( rpr ), sql );
  if ( !pdi->attached_copy )
    DORF( rpr, duf_main_db_open, pdi );
  DUF_TRACE( sql, 4, "@@@@@(%d:%s): %s", rpr, mas_error_name_i( rpr ), sql );
  DUF_TEST_R( rpr );
  chkids = pdi->idstatements;

  assert( pdi );
  assert( pdi->inited );
  if ( stmtid )
    is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  if ( !is )
  {
    if ( !pdi->num_idstatements )
      pdi->idstatements = mas_malloc( sizeof( duf_idstmt_t ) );
    else
      pdi->idstatements = mas_realloc( pdi->idstatements, ( pdi->num_idstatements + 1 ) * sizeof( duf_idstmt_t ) );
    chkids = pdi->idstatements;
    is = &( pdi->idstatements[pdi->num_idstatements] );
    is->pstmt = NULL;
#if 0
    is->id = NULL;
#else
    is->id = ( duf_stmt_ident_t ) 0;
#endif
    pdi->num_idstatements++;
  }
/* duf_main_db_open can't be called after setting 'is' and before using 'is'
 *   - it may reallocate pdi->idstatements 
 *   - is will become invalid XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
 */
  DUF_TRACE( sql, 4, "@@@@@(%d:%s): %s", rpr, mas_error_name_i( rpr ), sql );

  DOR( rpr, duf_sql_prepare( sql, &pstmt ) );
  DUF_TEST_R( rpr );

  DUF_TRACE( sql, 4, "@@@@@(%d:%s): %s", rpr, mas_error_name_i( rpr ), sql );
/* assert( rpr >= 0 ); */
  if ( pstmt )
  {
#if 0
    is->id = pindex;
#else
    is->id = stmtid;
#endif
    is->pstmt = pstmt;
  }
  assert( chkids == pdi->idstatements );

  if ( pr )
    *pr = rpr;
  DUF_END(  );
  return pstmt;
}

duf_stmnt_t *
duf_pdi_find_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  )
{
  duf_idstmt_t *is = NULL;

  is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  return is ? is->pstmt : NULL;
}

int
duf_pdi_finalize_idstmt( duf_depthinfo_t * pdi, int i )
{
  DUF_STARTR( r );
/* int *pi; */

  assert( pdi );

  if ( pdi->idstatements[i].pstmt )
    DOR( r, duf_sql_finalize( pdi->idstatements[i].pstmt ) );
/* if ( pdi->xstatements[i] )  */
/*   pi = pdi->xstatements[i]; */
/* if ( pi )                   */
/*   *pi = -1;                 */
  pdi->idstatements[i].pstmt = NULL;
  DUF_ENDR( r );
}

int
duf_pdi_finalize_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  )
{
  DUF_STARTR( r );
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
  /* T( "@@@@@@@finalize %p", is->pstmt ); */
    is->pstmt = NULL;
  }

  DUF_ENDR( r );
}

int
duf_pdi_finalize_statement_by_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt )
{
  DUF_STARTR( r );
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt_by_stmt( pdi, pstmt /* pindex */  );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
  /* T( "@@@@@@@finalize %p", is->pstmt ); */
    is->pstmt = NULL;
  }

  DUF_ENDR( r );
}
