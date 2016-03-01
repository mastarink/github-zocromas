#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_ufilter_ref.h"
/* ###################################################################### */

int
duf_ufilter_max_rel_depth( const duf_ufilter_t * pu )
{
  return pu ? pu->max_rel_depth : 0;
}

int
duf_ufilter_max_filter( const duf_ufilter_t * pu, unsigned seq, const duf_items_t * pitems )
{
  DUF_STARTR( r );
  if ( pu->max_seq && seq >= pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pu->maxitems.files && pitems->files >= pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pu->maxitems.dirs && pitems->dirs >= pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pu->maxitems.total && pitems->total >= pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  DUF_ENDR( r );
}
