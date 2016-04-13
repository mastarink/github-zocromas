/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccb_structs.h"
#include "duf_sccb_row.h"                                            /* datarow_* ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_structs.h"                                       /* duf_sccb_handle_s (from duf_sccbh_types: duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

#include "duf_pathinfo_ref.h"

/* ###################################################################### */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */
/* ###################################################################### */

#if 1
/* unsigned long long                                                      */
/* duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name ) */
SRP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name )
{
/* unsigned long long n = 0; */

  n = sccbh ? CRP( datarow_get_number, sccbh->rows, name ) : 0;
/* return n; */
  ERP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name );
}

/* const char *                                                            */
/* duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name ) */
SRP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name )
{
/* const char *s = NULL; */

  s = sccbh ? CRP( datarow_get_string, sccbh->rows, name ) : NULL;
/* return s; */
  ERP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name );
}
#endif
/* void                                             */
/* duf_sccbh_rows_eval( duf_sccb_handle_t * sccbh ) */
SRN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh )
{
  if ( sccbh && sccbh->rows )
  {
    int n = 0;

    for ( duf_sccb_data_row_t * trow = sccbh->rows->prev; trow; trow = trow->prev )
    {
      if ( trow && trow->cnt )
      {
        const char *path;
        const char *rpath;
        const char *iname;

        path = CRX( pi_path, &trow->pathinfo );
        rpath = CRX( pi_relpath, &trow->pathinfo );
        iname = CRX( pi_itemname, &trow->pathinfo );
        MAST_TRACE( temp, 5, "@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, path, iname );
        MAST_TRACE( temp, 5, "@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, rpath, iname );
        n++;
      }
    }
    CRX( datarow_list_delete_f, sccbh->rows->prev, 0 );
    MAST_TRACE( temp, 5, "@@@@ ===========================================" );
    sccbh->rows->prev = NULL;
  }
  ERN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh );
}

SRN( SCCBH, void, sccbh_row_open, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg )
{
  if ( sccbh )
  {
    assert( pstmt_arg == CRX( pdi_each_stmt, H_PDI, 1 ) );
    assert( !sccbh->new_row );
    sccbh->new_row = CRX( datarow_create, CRX( pdi_each_stmt, H_PDI, 1 ) /* pstmt_arg */ , CRX( pdi_pathinfo, H_PDI ) );
  }
  ERN( SCCBH, void, sccbh_row_open, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg );
}

SRN( SCCBH, void, sccbh_row_close, duf_sccb_handle_t * sccbh )
{
  if ( sccbh && sccbh->new_row )
  {
  /* assert( sccbh->new_row ); */
    sccbh->new_row->prev = sccbh->rows;
    sccbh->rows = sccbh->new_row;
    sccbh->new_row = NULL;
  }

  ERN( SCCBH, void, sccbh_close_row, duf_sccb_handle_t * sccbh );
}

SRN( SCCBH, void, sccbh_row_next, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg )
{
  assert( pstmt_arg == duf_pdi_each_stmt( H_PDI, 1 ) );
  CRX( sccbh_row_open, sccbh, pstmt_arg );
  CRX( sccbh_row_close, sccbh );

  ERN( SCCBH, void, sccbh_row_next, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg );
}
