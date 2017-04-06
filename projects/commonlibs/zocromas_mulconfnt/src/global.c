#include "global.h"

static mucs_flags_t mucs_flags = 0;

mucs_flags_t
mucs_global_flags( void )
{
  return mucs_flags;
}

mucs_flags_t
mucs_global_flag( mucs_flags_t flag )
{
  return mucs_global_flags(  ) & flag;
}

void
mucs_set_global_flag( mucs_flags_t flag )
{
  mucs_flags |= flag;
}

void mucs_set_global_flags( mucs_flags_t flags ) __attribute__ ( ( alias( "mucs_set_global_flag" ) ) );
