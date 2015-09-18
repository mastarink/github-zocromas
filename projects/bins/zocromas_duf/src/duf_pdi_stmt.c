#include "duf_maintenance.h"

#include "duf_sql_prepared.h"
#include "duf_maindb.h"

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
  DEBUG_STARTR( r );
  duf_stmnt_t *pstmt = NULL;
  duf_idstmt_t *is = NULL;

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
    is = &( pdi->idstatements[pdi->num_idstatements] );
    is->pstmt = NULL;
#if 0
    is->id = NULL;
#else
    is->id = ( duf_stmt_ident_t ) 0;
#endif
    pdi->num_idstatements++;
  }
  DORF( r, duf_main_db_open );
  DOR( r, duf_sql_prepare( sql, &pstmt ) );
  DUF_TRACE( sql, 2, "@@@@@%d: %s", r, sql );
  assert( r >= 0 );
  if ( pstmt )
  {
#if 0
    is->id = pindex;
#else
    is->id = stmtid;
#endif
    is->pstmt = pstmt;
  }

  if ( pr )
    *pr = r;
  return pstmt;
  DEBUG_END(  );
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
duf_pdi_finalize_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  )
{
  DEBUG_STARTR( r );
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
    /* T( "@@@@@@@finalize %p", is->pstmt ); */
    is->pstmt = NULL;
  }

  DEBUG_ENDR( r );
}

int
duf_pdi_finalize_statement_by_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt )
{
  DEBUG_STARTR( r );
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt_by_stmt( pdi, pstmt /* pindex */  );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
    /* T( "@@@@@@@finalize %p", is->pstmt ); */
    is->pstmt = NULL;
  }

  DEBUG_ENDR( r );
}
