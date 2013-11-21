#ifndef MAS_RUNONCE_CONFIG_SECTION_H
#  define MAS_RUNONCE_CONFIG_SECTION_H

config_section_t *runonce_config_section_create( char *sectname, config_group_t * group );
void runonce_config_section_delete( config_section_t * section );
int runonce_config_section_item_create( config_section_t * section, const char *string, int nl );
int runonce_config_section_fill( config_group_t * group, config_section_t * section );


#endif
