/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>


#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */

#include "duf_li_credel.h"
#include "duf_li.h"

#include "duf_dh.h"                                                  /* duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc. ✗ */

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */
#include "duf_pathinfo_ref.h"
#include "duf_pathinfo_structs.h"

#include "duf_sccb_scanstage.h"                                      /* duf_nodetype_name; duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_levinfo_structs.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo.h"                                            /* duf_pi_clear*; duf_pi_levinfo_set; duf_pi_set_max_rel_depth; etc. ✗ */
/* ###################################################################### */

SR( PI, pi_levinfo_set, duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t maxdepth )
{
  assert( pi );

  if ( maxdepth )
  {
    duf_pi_levinfo_delete( pi );
    assert( !pi->levinfo );
    pi->maxdepth = maxdepth;
    pi->levinfo = pli;
    pi->depth = duf_li_calc_depth( pli );
    assert( pi->levinfo );
  }

  ER( PI, pi_levinfo_set, duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t maxdepth );
}

SR( PI, pi_set_max_rel_depth, duf_pathinfo_t * pi, const char *real_path, int max_rd )
{

  pi->depth = -1;

  if ( real_path )
  {
    CRV( mas_pathdepth, real_path );
    if ( QNOERR )
      pi->topdepth = QERRIND;
  }

  assert( pi->depth == -1 );
/* MAST_TRACE( temp, 0, "@@@@@@@ %u", max_rd ); */
  pi->maxdepth = max_rd ? max_rd : 20 + ( pi->topdepth ? pi->topdepth : 20 ); /* FIXME ??? */

  ER( PI, pi_set_max_rel_depth, duf_pathinfo_t * pi, const char *real_path, int max_rd );
}

void
duf_pi_clear_d( duf_pathinfo_t * pi, int d )
{
  assert( pi );
  assert( pi->levinfo );
  duf_li_clear( &pi->levinfo[d] );
}
/* *INDENT-OFF*  */
DUF_PATHINFO_VF( void, clear )
DUF_PATHINFO_VF_UP( void, clear )
/* *INDENT-ON*  */

void
duf_pi_clear_all( duf_pathinfo_t * pi )
{
  assert( pi );
  duf_li_clear_array( pi->levinfo, pi->maxdepth );
}
