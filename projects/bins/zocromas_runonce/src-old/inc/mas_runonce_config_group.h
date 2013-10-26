#ifndef MAS_RUNONCE_GROUP_H
#  define MAS_RUNONCE_GROUP_H

config_group_t *runonce_config_group_create( const char *name, size_t namelen );
void runonce_config_group_delete( config_group_t * cfgg );
int runonce_config_group_load( const char *dir, config_group_t * cfg_group );

#endif
