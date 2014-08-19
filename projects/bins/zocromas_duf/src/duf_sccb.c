#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_pdi.h"

/* #include "duf_cli_options.h" */

#include "duf_sql_field.h"
#include "duf_item_scan1.h"
#include "duf_item_scan2.h"
#include "duf_dirent_scan.h"
#include "duf_file_pathid.h"

#include "duf_dbg.h"


/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */


const char *
_duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  const char *stitle;

  DEBUG_START(  );
  if ( sccb && sccb->title )
  {
    stitle = strrchr( sccb->title, '/' );
    if ( stitle )
      stitle++;
    else
      stitle = sccb->title;
  }
  else
  {
    stitle = "";
  }
  DEBUG_ENDS( stitle );

  return stitle;
}

const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", _duf_uni_scan_action_title( sccb ) );
  return tbuf;
}
