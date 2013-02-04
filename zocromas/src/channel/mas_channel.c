#include "mas_basic_def.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "io/inc/mas_io.h"

#include "mas_channel_listen.h"
/* #include "mas_channel_open.h" */

#include "mas_channel.h"

/*
this:
  mas_channel.c
related:
  mas_channel_listen.c
  mas_channel_open.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h
  
  mas_io.h
  mas_io.c
  mas_client.c
  
  mas_transaction_control.c
  mas_basic_def.h
*/


/**************
On Linux, TCP_DEFER_ACCEPT can be enabled to defer accept calls until there is (also) data ready to be read.
On FreeBSD, SO_ACCEPTFILTER can be enabled to defer accept calls until there is a complete HTTP request (or something else) to be read.

#ifdef SO_ACCEPTFILTER
    accept_filter_arg afa;
    bzero(&afa, sizeof(afa));
    strcpy(afa.af_name, "httpready");
    l.___setsockopt(SOL_SOCKET, SO_ACCEPTFILTER, &afa, sizeof(afa));
#elif TCP_DEFER_ACCEPT
    int v = true;
    ___setsockopt(acceptor_.native(), IPPROTO_TCP, TCP_DEFER_ACCEPT, &v, sizeof(v));
#endif
***********/

int
mas_channel_fd( const mas_channel_t * pchannel )
{
  int r = -1;

  if ( pchannel )
  {
    r = pchannel->fd_io;
  }
  return r;
}

FILE *
mas_channel_stream( const mas_channel_t * pchannel )
{
  FILE *f = NULL;

  if ( pchannel )
    f = pchannel->stream_io;
  return f;
}

mas_channel_t *
mas_channel_clone( mas_channel_t * pchannel_src )
{
  mas_channel_t *pchannel_dst = NULL;

  if ( pchannel_src )
  {
    pchannel_dst = mas_malloc( sizeof( mas_channel_t ) );
    *pchannel_dst = *pchannel_src;
    if ( pchannel_src->error_func )
      pchannel_dst->error_func = mas_strdup( pchannel_src->error_func );
    if ( pchannel_src->host )
      pchannel_dst->host = mas_strdup( pchannel_src->host );
  }
  return pchannel_dst;
}

int
mas_channel_writef( const mas_channel_t * pchannel, const char *fmt, ... )
{
  int w = 0;
  va_list args;

  va_start( args, fmt );
  w = mas_channel_vwritef( pchannel, fmt, args );
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  va_end( args );
  return w;
}

int
mas_channel_vwritef( const mas_channel_t * pchannel, const char *fmt, va_list args )
{
  int w = 0;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  w = mas_vwritef( wfd, fmt, args );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  w = mas_vfprintf( wstream, fmt, args );
#endif
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  return w;
}

/*
 * z : to write z-byte if NZ
 * */
int
mas_channel_write_string( const mas_channel_t * pchannel, char *cbuf, int z )
{
  int w;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  w = mas_write_string( wfd, cbuf, z );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  w = mas_fwrite_string( wstream, cbuf, z );
#endif
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  return w;
}

int
mas_channel_write( const mas_channel_t * pchannel, char *cbuf, size_t sz )
{
  int w;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  w = mas_write( wfd, cbuf, sz );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  /* int rf; */

  w = mas_fwrite( cbuf, 1, sz, wstream );
  /* rf = fflush( wstream ); */
#endif
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  return w;
}

char *
mas_channel_ip_string( mas_channel_t * pchannel )
{
  return mas_ip_string( &pchannel->serv.addr.sin_addr );
  /* char ip[128];                                                                    */
  /* const char *pip;                                                                 */
  /*                                                                                  */
  /* pip = mas_inet_ntop( AF_INET, &pchannel->serv.addr.sin_addr, ip, sizeof( ip ) ); */
  /* return pip ? mas_strdup( pip ) : NULL;                                           */
}

int
mas_channel_read_all( mas_channel_t * pchannel, char **pbuf, size_t * psz )
{
  int r;

  MAS_LOG( "to read ch (read all)" );
#ifndef MAS_CHANNEL_STREAM_READ
  int rfd = mas_channel_fd( pchannel );

  r = mas_read_all( rfd, pbuf, psz );
#else
  FILE *rstream = mas_channel_stream( pchannel );

  r = mas_fread_all( rstream, pbuf, psz );
#endif
  return r;
}
