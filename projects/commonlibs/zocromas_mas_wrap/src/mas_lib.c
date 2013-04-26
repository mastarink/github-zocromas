#include "mas_std_def.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#  include <stdio.h>
#  include <errno.h>

#include <time.h>

#include "mas_memory.h"
#include "mas_lib.h"

/*
this:
  mas_lib.c
related:
  mas_lib0.c
  mas_memory.c

*/
__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}


int
mas_connect( int sockfd, const struct sockaddr *addr, socklen_t addrlen )
{
  return connect( sockfd, addr, addrlen );
}

int
mas_accept( int sockfd, struct sockaddr *addr, socklen_t * addrlen )
{
  return accept( sockfd, addr, addrlen );
}

int
mas_bind( int sockfd, const struct sockaddr *addr, socklen_t addrlen )
{
  return bind( sockfd, addr, addrlen );
}

int
mas_setsockopt( int sockfd, int level, int optname, const void *optval, socklen_t optlen )
{
  return setsockopt( sockfd, level, optname, optval, optlen );
}

int
mas_getsockopt( int sockfd, int level, int optname, void *optval, socklen_t * optlen )
{
  return getsockopt( sockfd, level, optname, optval, optlen );
}

const char *
mas_inet_ntop( int af, const void *src, char *dst, socklen_t size )
{
  return inet_ntop( af, src, dst, size );
}

char *
mas_inet_ntoa( struct in_addr in )
{
  return inet_ntoa( in );
}

int
mas_inet_pton( int af, const char *src, void *dst )
{
  return inet_pton( af, src, dst );
}

int
mas_listen( int sockfd, int backlog )
{
  return listen( sockfd, backlog );
}

int
mas_mkfifo( const char *pathname, mode_t mode )
{
  return mkfifo( pathname, mode );
}

FILE *
mas_fopen( const char *path, const char *mode )
{
  return fopen( path, mode );
}

int
mas_fclose( FILE * fp )
{
  return fclose( fp );
}

ssize_t
mas_read( int fd, void *buf, size_t count )
{
  return read( fd, buf, count );
}

ssize_t
mas_write( int fd, const void *buf, size_t count )
{
  return write( fd, buf, count );
}

void
mas_nanosleep( double s )
{
  long nano = 0;
  struct timespec d;
  float si;

  /* s = s / 1E6; */
  si = ( float ) ( long ) ( s );
  nano = ( long ) ( ( s - si ) * 1E9 );
  d.tv_sec = si;
  d.tv_nsec = nano;
  nanosleep( &d, NULL );
}

pid_t
mas_gettid( void )
{
  /* Glibc does not provide a wrapper for this system call; call it using syscall(2). */
  /* ctrl.main_tid = gettid(  ); */
  return syscall( SYS_gettid );
}
