#ifndef MUCS_OPTION_TABLIST_BASE_H
# define MUCS_OPTION_TABLIST_BASE_H

void mucs_config_option_tablist_init( mucs_option_table_list_t * tablist );
mucs_option_table_list_t *mucs_config_option_tablist_create( void );
void mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist );
void mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist );

#endif
