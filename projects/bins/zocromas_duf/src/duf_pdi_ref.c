/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace.h>

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */

duf_depthinfo_t *
duf_pdi_root( duf_depthinfo_t * pdi )
{
  return pdi->root_pdi ? pdi->root_pdi : pdi;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_allow_dirs( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->allow_dirs ? 1 : 0 ) : 0;
}

int
duf_pdi_linear( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->linear ? 1 : 0 ) : 0;
}

void
duf_pdi_set_each_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg )
{
  if ( duf_pdi_linear( pdi ) )
  {
    pdi->each_pstmt_linear = pstmt_arg;
  }
  else
  {
    CRX( levinfo_set_each_stmt, pdi, pstmt_arg );
  }
}

duf_stmnt_t *
duf_pdi_each_stmt( const duf_depthinfo_t * pdi, int up )
{
  duf_stmnt_t *pstmt_val = NULL;

  if ( CRX( pdi_linear, pdi ) )
  {
    pstmt_val = pdi->each_pstmt_linear;
  }
  else if ( up )
  {
    pstmt_val = CRX( levinfo_each_stmt_up, pdi );
  }
  else
  {
    pstmt_val = CRX( levinfo_each_stmt, pdi );
  }
  return pstmt_val;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}

int
duf_pdi_opendir( const duf_depthinfo_t * pdi )
{
  return pdi && pdi->opendir ? 1 : 0;
}

const char *
duf_pdi_name( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pdi_name : NULL;
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  int rd = 0;

  assert( pdi );
  rd = pdi->opendir;
  pdi->opendir = od;
/* QT( "@SET OPENDIR: %d", od ); */
  MAST_TRACE( fs, 3, "set opendir:%d", od );

  return rd;
}

unsigned long long
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

unsigned long long
duf_pdi_seq_leaf( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq_leaf : 0;
}

void
duf_pdi_seq_leaf_plus( duf_depthinfo_t * pdi )
{
  if ( pdi )
  {
    pdi->seq++;
    pdi->seq_leaf++;
  }
}

unsigned long long
duf_pdi_seq_node( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq_node : 0;
}

void
duf_pdi_seq_node_plus( duf_depthinfo_t * pdi )
{
  if ( pdi )
  {
    pdi->seq++;
    pdi->seq_node++;
  }
}
