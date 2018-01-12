#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"
#include "source_flags.h"
/* #include "source.h" */

#include "option_flags.h"

mucs_flags_t
mucs_config_option_flags( const mucs_option_t * opt )
{
  mucs_flags_t osrcflags = opt && opt->d.source ? mucs_source_flags( opt->d.source ) : 0L;

  return opt ? ( osrcflags | opt->s.flags ) : 0L;
}

int
mucs_config_soption_flag( const mucs_option_static_t * sopt, mucs_flags_t mask )
{
  return sopt ? ( sopt->flags & mask ) : 0;
}

int
mucs_config_option_flag( const mucs_option_t * opt, mucs_flags_t mask )
{
  int osrcflag = opt->d.source ? mucs_source_flag( opt->d.source, mask ) : 0L;

  return opt ? ( osrcflag || mucs_config_soption_flag( &opt->s, mask ) ) : 0;
}
