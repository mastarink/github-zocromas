#ifndef MUCS_AOPTIONS_H
# define MUCS_AOPTIONS_H

const mucs_option_t *mucs_config_option_tabnode_aoptions( const mucs_option_table_list_t * tabnode );
void mucs_config_aoptions_reset( mucs_option_t * options, size_t count );
mucs_option_t *mucs_config_aoptions_clone( const mucs_option_t * options, size_t count );
void mucs_config_aoptions_delete( mucs_option_t * options, size_t count );

#endif
