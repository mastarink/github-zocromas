#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
/* #include <string.h> */
/* #include <unistd.h> */
/* #include <libgen.h> */

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_arg_tools.h> */
/* #include <mastar/tools/mas_argvc_tools.h> */

#include <mastar/minierr/minierr.h>
/* #include <mastar/exam/masexam.h> */

#include "mulconfnt_structs.h"

#include "global.h"

/* #include "option_base.h" */
/* #include "option.h" */
/* #include "option_flags.h" */
/* #include "option_tablist.h" */

/* #include "optscanner_base.h" */
/* #include "optscanner.h" */

/* #include "source_defaults.h" */
/* #include "source_base.h" */

#include "source_flags.h"

mucs_flags_t
mucs_source_flags( mucs_source_t * osrc )
{
  return osrc ? osrc->flags : 0L;
}

int
mucs_source_flag( mucs_source_t * osrc, mucs_flags_t mask )
{
  return osrc && ( ( osrc->flags & mask ) ? 1 : 0 );
}
