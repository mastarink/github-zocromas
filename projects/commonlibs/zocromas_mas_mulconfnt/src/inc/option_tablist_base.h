#ifndef MULCONF_OPTION_TABLIST_BASE_H
# define MULCONF_OPTION_TABLIST_BASE_H

void mucs_config_option_tablist_init( config_option_table_list_t * tablist );
config_option_table_list_t *mucs_config_option_tablist_create( void );
void mucs_config_option_tablist_close( config_option_table_list_t * tablist );
void mucs_config_option_tablist_delete( config_option_table_list_t * tablist );

#endif
