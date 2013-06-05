#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/io/mas_io.h>

#include "mas_channel_object.h"
#include "mas_channel_listen.h"
#include "mas_channel_buffer.h"

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

  if ( mas_channel_test( pchannel ) )
  {
    r = pchannel->fd_io;
  }
  return r;
}

#ifdef MAS_CHANNEL_STREAM_WRITE
FILE *
mas_channel_stream( const mas_channel_t * pchannel )
{
  FILE *f = NULL;

  if ( pchannel )
    f = pchannel->stream_io;
  return f;
}
#endif

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
  int r = 0;
  va_list args;

  va_start( args, fmt );
  IEVAL( r, mas_channel_vwritef( pchannel, fmt, args ) );
  va_end( args );
  return r;
}

int
mas_channel_vwritef( const mas_channel_t * pchannel, const char *fmt, va_list args )
{
  int r = 0;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  IEVAL( r, mas_vwritef( wfd, fmt, args ) );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  IEVAL( r, mas_vfprintf( wstream, fmt, args ) );
#endif
  return r;
}

/*
 * z : to write z-byte if NZ
 * */
int
mas_channel_write_string( const mas_channel_t * pchannel, const char *cbuf, int z )
{
  int r = 0;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  IEVAL( r, mas_write_string( wfd, cbuf, z ) );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  IEVAL( r, mas_fwrite_string( wstream, cbuf, z ) );
#endif
  return r;
}

int
mas_channel_write( const mas_channel_t * pchannel, const char *cbuf, size_t sz )
{
  int r = 0;

#ifndef MAS_CHANNEL_STREAM_WRITE
  int wfd = mas_channel_fd( pchannel );

  IEVAL( r, mas_write( wfd, cbuf, sz ) );
#else
  FILE *wstream = mas_channel_stream( pchannel );

  /* int rf; */

  /* r = mas_fwrite( cbuf, 1, sz, wstream ); */
  IEVAL( r, mas_fwrite( cbuf, 1, sz, wstream ) );
  /* rf = fflush( wstream ); */
#endif
  return r;
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
mas_channel_read_some_new( mas_channel_t * pchannel )
{
  return mas_channel_read_some( pchannel );
}

int
mas_channel_read_some( mas_channel_t * pchannel )
{
  int r = 0;

  MAS_LOG( "to read ch (read all)" );
#ifndef MAS_CHANNEL_STREAM_READ
  int rfd = mas_channel_fd( pchannel );

  if ( !pchannel->buffer.maxread )
    pchannel->buffer.maxread = 1028;
  WMSG( "READ SOME maxread: %ld", ( unsigned long ) pchannel->buffer.maxread );
  /* WMSG( "(%d)SOME", r ); */
  IEVAL( r, mas_io_read_some( rfd, &pchannel->buffer.buffer, &pchannel->buffer.size, pchannel->buffer.maxread ) );
#else
  FILE *rstream = mas_channel_stream( pchannel );

  IEVAL( r, mas_io_fread_some( rstream, &pchannel->buffer.buffer, &pchannel->buffer.size, pchannel->buffer.maxread ) );
#endif
  if ( r == 0 )
    pchannel->buffer.endfile = 1;
  WMSG( "(%d)SOME %lu L%lu", r, ( unsigned long ) pchannel->buffer.size, ( unsigned long ) pchannel->buffer.length );
  if ( r > 0 )
    pchannel->buffer.length += r;
  return r;
}

int
mas_channel_read_remainder( mas_channel_t * pchannel )
{
  int r = 0;

  while ( !( r < 0 ) && !mas_channel_buffer_endfile( pchannel ) )
  {
    /* r = mas_channel_read_some( pchannel ); */
    IEVAL( r, mas_channel_read_some( pchannel ) );

    WMSG( "(%d)SOME/REM %lu L%lu", r, ( unsigned long ) pchannel->buffer.size, ( unsigned long ) pchannel->buffer.length );
  }
  return r;
}
