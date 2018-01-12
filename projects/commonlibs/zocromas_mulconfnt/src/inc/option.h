#ifndef MUCS_OPTION_H
# define MUCS_OPTION_H

# include "mulconfnt_types.h"


int mucs_config_option_lookup_options( const mucs_option_t * options, unsigned count, const char *arg, const char *eq, mucs_optscanner_t * optscan );

int mucs_config_option_evaluate( mucs_option_t * opt, mucs_optscanner_t * optscan, void *userdata );

#endif
