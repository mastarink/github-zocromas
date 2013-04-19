#include "mas_std_def.h"

#include <unistd.h>
#  include <stdio.h>
#  include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "mas_lib0.h"

/*
this:
  mas_lib0.c
related:
  mas_lib.c
  mas_memory.c

*/
__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno );
}


/* int                                                      */
/* mas_open( const char *pathname, int flags, mode_t mode ) */
/* {                                                        */
/*   return open( pathname, flags, mode );                  */
/* }                                                        */

int
mas_open2( const char *pathname, int flags )
{
  return open( pathname, flags );
}

int
mas_open3( const char *pathname, int flags, mode_t mode )
{
  return open( pathname, flags, mode );
}

int
mas_close( int fd )
{
  return close( fd );
}

pid_t
mas_fork( void )
{
  return fork(  );
}

pid_t
mas_wait( int *status )
{
  return wait( status );
}

pid_t
mas_waitpid( pid_t pid, int *status, int options )
{
  return waitpid( pid, status, options );
}

int
mas_pipe( int pipefd[2] )
{
  return pipe( pipefd );
}
