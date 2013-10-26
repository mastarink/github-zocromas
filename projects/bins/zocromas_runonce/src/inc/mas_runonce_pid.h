#ifndef MAS_RUNONCE_PID_H
#  define MAS_RUNONCE_PID_H

int runonce_pids_create( void );
void runonce_pids_delete( void );
void runonce_pids_reset( void );
int runonce_pidof( pid_t * pids, size_t num, const char *name, const char *subname, const char *path, int argc, char **argv,
                   runonce_flags_t flags );

int runonce_group_get_pids( config_group_t * grp, const char *sectpatt, runonce_flags_t flags );
int runonce_group_list_pids( config_group_t * grp, const char *sectpatt, runonce_flags_t flags );

int runonce_get_pids( const char *grppatt, const char *sectpatt, runonce_flags_t flags );
int runonce_list_pids( const char *grppatt, const char *sectpatt, runonce_flags_t flags );

prec_t *runonce_pids_precs( void );

#endif
