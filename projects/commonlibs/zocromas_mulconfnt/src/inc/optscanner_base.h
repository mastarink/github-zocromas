#ifndef MUCS_OPTSCANNER_BASE_H
# define MUCS_OPTSCANNER_BASE_H

# include "mulconfnt_types.h"

void mucs_optscanner_init( mucs_optscanner_t * optscan, mucs_source_t * osrc, const mucs_option_table_list_t * tablist );
void mucs_optscanner_reset( mucs_optscanner_t * optscan );

#endif
