/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
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
      if ( pdi->idstatements[i].id == stmtid /* pindex */  )
      {
        is = &pdi->idstatements[i];
        break;
      }
    }
  }
  return is;
}

static duf_idstmt_t *
duf_pdi_find_idstmt_by_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_pdi )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  if ( pdi->idstatements )
  {
    for ( int i = 0; i < pdi->num_idstatements; i++ )
    {
      if ( pdi->idstatements[i].statement == pstmt_pdi )
      {
        is = &pdi->idstatements[i];
        break;
      }
    }
  }
  return is;
}

/* must be checked it's absent */
/* duf_stmnt_t *                                                                                                                         */
/* duf_pdi_prepare_statement_by_id( duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid (*, const int *pindex *) , int *pr ) */
SRP( PDI, duf_stmnt_t *, pstmt_pdi, NULL, pdi_prepare_statement_by_id, duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid )
{
  duf_idstmt_t *is = NULL;
  duf_idstmt_t *chkids = NULL;

  assert( pdi );
  MAST_TRACE( sql, 4, "@@@@@(%d:%s): %s", QERRIND, QERRNAME, sql );
  if ( !pdi->attached_copy )
    CR( main_db_open, pdi );
  MAST_TRACE( sql, 4, "@@@@@(%d:%s): %s", QERRIND, QERRNAME, sql );
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
    is->statement = NULL;
    is->id = ( duf_stmt_ident_t ) 0;
    pdi->num_idstatements++;
  }
/* duf_main_db_open can't be called after setting 'is' and before using 'is'
 *   - it may reallocate pdi->idstatements 
 *   - is will become invalid XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
 */
  MAST_TRACE( sql, 4, "@@@@@(%d:%s): %s", QERRIND, QERRNAME, sql );

  CR( sql_prepare, sql, &pstmt_pdi );

  MAST_TRACE( sql, 4, "@@@@@(%d:%s): %s", QERRIND, QERRNAME, sql );
  if ( pstmt_pdi )
  {
    is->id = stmtid;
    is->statement = pstmt_pdi;
  }
  assert( chkids == pdi->idstatements );

  ERP( PDI, duf_stmnt_t *, pstmt_pdi, NULL, pdi_prepare_statement_by_id, duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid );
}

duf_stmnt_t *
duf_pdi_find_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  )
{
  duf_idstmt_t *is = NULL;

  is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  return is ? is->statement : NULL;
}

SR( PDI, pdi_finalize_idstmt, duf_depthinfo_t * pdi, int i )
{
  assert( pdi );

  if ( pdi->idstatements[i].statement )
    CR( sql_finalize, pdi->idstatements[i].statement );
  pdi->idstatements[i].statement = NULL;
  ER( PDI, pdi_finalize_idstmt, duf_depthinfo_t * pdi, int i );
}

SR( PDI, pdi_finalize_statement_by_id, duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt( pdi, stmtid /* pindex */  );
  if ( is && is->statement )
  {
    CR( sql_finalize, is->statement );
    is->statement = NULL;
  }

  ER( PDI, pdi_finalize_statement_by_id, duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  );
}

SR( PDI, pdi_finalize_statement_by_stmt, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg )
{
  duf_idstmt_t *is = NULL;

  assert( pdi->inited );
  is = duf_pdi_find_idstmt_by_stmt( pdi, pstmt_arg /* pindex */  );
  if ( is && is->statement )
  {
    CR( sql_finalize, is->statement );
    is->statement = NULL;
  }

  ER( PDI, pdi_finalize_statement_by_stmt, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_pdi );
}
