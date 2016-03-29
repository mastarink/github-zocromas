/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_updown.h"
#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_pathinfo.h"                                            /* duf_pi_clear*; duf_pi_levinfo_set; duf_pi_set_max_rel_depth; etc. ✗ */
#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

/* # include "duf_sql_types.h" */
#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
/* ###################################################################### */

SR( PDI, levinfo_set_li, duf_depthinfo_t * pdi, duf_levinfo_t * pli, size_t maxdepth )
{
/* DUF_STARTR( r ); */

  CR( pi_levinfo_set, &pdi->pathinfo, pli, maxdepth );

/* DUF_ENDR( r ); */
  ER( PDI, levinfo_set_li, duf_depthinfo_t * pdi, duf_levinfo_t * pli, size_t maxdepth );
}

/* create level-control array, open 0 level */
SR( PDI, levinfo_create, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */

  assert( pdi );

  assert( duf_levinfo_dirid( pdi ) == 0 );
  duf_pi_levinfo_create( &pdi->pathinfo, pdi->pathinfo.maxdepth );

  assert( pdi->pathinfo.levinfo[pdi->pathinfo.maxdepth + 1].d == 0 );
  assert( pdi->pathinfo.levinfo );
  assert( pdi->pathinfo.depth == -1 );

/* DUF_ENDR( r ); */
  ER( PDI, levinfo_create, duf_depthinfo_t * pdi );
}

/* delete level-control array, close 0 level */
SR( PDI, levinfo_delete, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */

  assert( pdi );
/* assert( pdi->pathinfo.levinfo ); */

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  if ( pdi->pathinfo.levinfo )
  {
    while ( QNOERR && pdi->pathinfo.depth >= 0 )
    {
      assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
      duf_levinfo_goup( pdi );
    }
    assert( pdi->pathinfo.depth == -1 );

    mas_free( pdi->pathinfo.levinfo );
    pdi->pathinfo.levinfo = NULL;
  }
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_delete, duf_depthinfo_t * pdi );
}
