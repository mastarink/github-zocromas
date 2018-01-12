#ifndef MUCS_OPTSCANNER_H
# define MUCS_OPTSCANNER_H

# include "mulconfnt_types.h"

int mucs_optscanner_lookup_arg( mucs_optscanner_t * optscan, void *userdata );
int mucs_optscanner_next_word( mucs_optscanner_t * optscan );

#endif
