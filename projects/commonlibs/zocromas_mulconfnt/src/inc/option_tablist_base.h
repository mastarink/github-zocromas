#ifndef MUCS_OPTION_TABLIST_BASE_H
# define MUCS_OPTION_TABLIST_BASE_H

# include "mulconfnt_types.h"

void mucs_config_option_tabnode_init( mucs_option_table_list_t * tablist );
mucs_option_table_list_t *mucs_config_option_tabnode_create( void );

mucs_option_table_list_t *mucs_config_option_tabnode_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_t * options,
                                                          size_t count );

void mucs_config_option_tabnode_delete( mucs_option_table_list_t * tablist );
void mucs_config_option_tabnode_reset( mucs_option_table_list_t * tablist );

void mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist );
void mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist );

#endif
