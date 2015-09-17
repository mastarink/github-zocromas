#include "duf_maintenance.h"

#include "duf_sql_prepared.h"
#include "duf_maindb.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */


static duf_idstmt_t *
duf_pdi_find_idstmt( duf_depthinfo_t * pdi, int *pindex )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
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
duf_stmnt_t *
duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex, int *pr )
{
  DEBUG_STARTR( r );
  duf_stmnt_t *pstmt = NULL;
  duf_idstmt_t *is = NULL;

  assert( pdi );
  assert( pdi->inited );
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
  DORF( r, duf_main_db_open );
  DOR( r, duf_sql_prepare( sql, &pstmt ) );
  DUF_TRACE( sql, 2, "@@@@@%d: %s", r, sql );
  assert(r>=0);
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

duf_stmnt_t *
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

  assert( pdi->inited );
  is = duf_pdi_find_idstmt( pdi, pindex );
  if ( is && is->pstmt )
  {
    DOR( r, duf_sql_finalize( is->pstmt ) );
    is->pstmt = NULL;
  }

  DEBUG_ENDR( r );
}
