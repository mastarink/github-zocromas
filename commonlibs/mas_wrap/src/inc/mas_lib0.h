#ifndef MAS_LIB0_H
#  define MAS_LIB0_H

#  include <unistd.h>

pid_t mas_fork( void );

#  define mas_open( ... ) open(__VA_ARGS__)
int mas_close( int fd );
pid_t mas_wait( int *status );
pid_t mas_waitpid( pid_t pid, int *status, int options );
int mas_pipe( int pipefd[2] );


#endif
