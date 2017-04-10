#ifndef MUCS_OPTION_TABLIST_H
# define MUCS_OPTION_TABLIST_H

# include "mulconfnt_types.h"


int mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, const char *arg, const char *eq,
                                                        mucs_optscanner_t * optscan );

#endif
