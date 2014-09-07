#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_pdi.h"

#include "duf_sql_field.h"


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
}

const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", _duf_uni_scan_action_title( sccb ) );
  return tbuf;
}
