#ifndef MAS_RUNONCE_ACTION_H
#  define MAS_RUNONCE_ACTION_H

int runonce_launch( config_group_t * grp, config_section_t * sect, runonce_flags_t flags );
int runonce_terminate( config_group_t * grp, config_section_t * sect, runonce_flags_t flags );
int runonce_exit( config_group_t * grp, config_section_t * sect, runonce_flags_t flags );

#endif
