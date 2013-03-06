#ifndef MAS_LIB_H
#  define MAS_LIB_H

#  include <stdio.h>
#  include <arpa/inet.h>
#  include <sys/types.h>
#  include <sys/socket.h>

/* #  include "mas_lib0.h" */


int mas_accept( int sockfd, struct sockaddr *addr, socklen_t * addrlen );
int mas_connect( int sockfd, const struct sockaddr *addr, socklen_t addrlen );
int mas_bind( int sockfd, const struct sockaddr *addr, socklen_t addrlen );
int mas_getsockopt( int sockfd, int level, int optname, void *optval, socklen_t * optlen );
int mas_setsockopt( int sockfd, int level, int optname, const void *optval, socklen_t optlen );
const char *mas_inet_ntop( int af, const void *src, char *dst, socklen_t size );
char *mas_inet_ntoa( struct in_addr in );
int mas_inet_pton( int af, const char *src, void *dst );

int mas_listen( int sockfd, int backlog );
int mas_mkfifo( const char *pathname, mode_t mode );


FILE *mas_fopen( const char *path, const char *mode );
int mas_fclose( FILE * fp );

ssize_t mas_read( int fd, void *buf, size_t count );
ssize_t mas_write( int fd, const void *buf, size_t count );


void mas_nanosleep( double s );
pid_t mas_gettid( void );

#endif
