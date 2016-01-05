#include "duf_maintenance_options.h"


/* ###################################################################### */
#include "duf_option_stage.h"
/* ###################################################################### */

const char *
duf_optstage_name( duf_option_stage_t istage )
{
  static const char *tail[] = {
    [DUF_OPTION_STAGE_PRESETUP] = "presetup",
    [DUF_OPTION_STAGE_SETUP] = "setup",
    [DUF_OPTION_STAGE_FIRST] = "first",
    [DUF_OPTION_STAGE_LOOP] = "loop",
    [DUF_OPTION_STAGE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( istage >= 0 && istage < DUF_OPTION_STAGE_MAX ) ? tail[istage] : ( istage == DUF_OPTION_STAGE_ANY ? "any" : NULL );
}
