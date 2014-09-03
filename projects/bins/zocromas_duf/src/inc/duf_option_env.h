#ifndef MAS_DUF_OPTION_ENV_H
#  define MAS_DUF_OPTION_ENV_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_env_options_at_var( int argc, char *argv[], const char *envvarname );
int duf_env_options( int argc, char *argv[] );



#endif

