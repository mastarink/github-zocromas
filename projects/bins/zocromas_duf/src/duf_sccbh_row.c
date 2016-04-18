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

#include "duf_mod_handle.h"
/* ###################################################################### */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */
/* ###################################################################### */

#if 1
/* unsigned long long                                                      */
/* duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name ) */
SRP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name )
{
/* unsigned long long n = 0; */

  n = sccbh ? CRP( datarow_get_number, sccbh->current_row ? sccbh->current_row : sccbh->rows, name ) : 0;
/* return n; */
  ERP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name );
}

/* const char *                                                            */
/* duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name ) */
SRP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name )
{
/* const char *s = NULL; */

  s = sccbh ? CRP( datarow_get_string, sccbh->current_row ? sccbh->current_row : sccbh->rows, name ) : NULL;
/* return s; */
  ERP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name );
}
#endif

#if 0
/* void                                             */
/* duf_sccbh_rows_eval( duf_sccb_handle_t * sccbh ) */
SRN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh )
{
  if ( sccbh && sccbh->rows )
  {
    duf_scanner_fun_t pack_cb;

    pack_cb = CRX( load_mod_handler_symbol_find, H_SCCB->name, "pack" );
    if ( pack_cb )
    {
      QT( "@@@%s : %p", H_SCCB->name, pack_cb );
      CRV( pack_cb, H_PDI, sccbh );
    }
    CRX( datarow_list_delete_f, sccbh->rows->prev, 0 );
    MAST_TRACE( temp, 5, "@@@@  =========================================== %s:%p ", H_SCCB->name, pack_cb );
    sccbh->rows->prev = NULL;
  }
  ERN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh );
}
#endif

#if 0
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
#endif

SRN( SCCBH, void, sccbh_row_add, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg )
{
  assert( !pstmt_arg || pstmt_arg == CRX( pdi_each_stmt, H_PDI, 1 ) );
#if 0
  CRX( sccbh_row_open, sccbh, pstmt_arg );
  CRX( sccbh_row_close, sccbh );
#else
  if ( sccbh )
  {
    duf_sccb_data_row_t *new_row = NULL;

    assert( !pstmt_arg || pstmt_arg == CRX( pdi_each_stmt, H_PDI, 1 ) );
    assert( ! /*sccbh-> */ new_row );
    /*sccbh-> */ new_row = CRX( datarow_create, CRX( pdi_each_stmt, H_PDI, 1 ) /* pstmt_arg */ , CRX( pdi_pathinfo, H_PDI ) );
    if (  /*sccbh-> */ new_row )
    {
    /* assert( sccbh->new_row ); */
    /*sccbh-> */ new_row->prev = sccbh->rows;
      sccbh->rows = /*sccbh-> */ new_row;
    /*sccbh-> */ new_row = NULL;
    }
  }
#endif
  ERN( SCCBH, void, sccbh_row_add, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg );
}

SRX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_start_row, duf_sccb_handle_t * sccbh )
{
  assert( sccbh );
  CRX( sccbh_set_current_row, sccbh, sccbh->rows );
  row = CRX( sccbh_current_row, sccbh );
  ERX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_start_row, duf_sccb_handle_t * sccbh );
}

SRN( SCCBH, void, sccbh_end_row, duf_sccb_handle_t * sccbh )
{
  assert( sccbh );
  CRX( sccbh_set_current_row, sccbh, NULL );
  ERN( SCCBH, void, sccbh_end_row, duf_sccb_handle_t * sccbh );
}

SRX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_prev_row, duf_sccb_handle_t * sccbh )
{
  assert( sccbh );
  if ( sccbh->current_row )
    CRX( sccbh_set_current_row, sccbh, sccbh->current_row->prev );

  row = sccbh->current_row;
  ERX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_prev_row, duf_sccb_handle_t * sccbh );
}
