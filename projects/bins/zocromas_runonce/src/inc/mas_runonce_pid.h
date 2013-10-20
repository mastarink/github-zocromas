#ifndef MAS_RUNONCE_PID_H
#  define MAS_RUNONCE_PID_H

int runonce_pids_create( void );
void runonce_pids_delete( void );
void runonce_pids_reset( void );
int runonce_pidof( pid_t * pids, size_t num, const char *name, const char *subname, const char *path, int argc, char **argv );


#endif
