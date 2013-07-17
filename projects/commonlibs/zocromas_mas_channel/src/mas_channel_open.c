#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include <netinet/tcp.h>

#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_channel_socket.h"
#include "mas_channel_object.h"
#include "mas_channel_listen.h"
#include "mas_channel_buffer.h"

#include "mas_channel_open.h"


#define MAS_CHANNEL_OPEN_TUNE

/*
this:
  mas_channel_open.c
related:
  mas_channel_object.c
  mas_channel.c
  mas_channel_socket.c
  mas_channel_listen.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h

*/
static int
_mas_channel_open_rfile( mas_channel_t * pchannel )
{
  int fd_io = -1;

  pchannel->error = 0;

  if ( pchannel->is_server )
  {
    fd_io = mas_open( pchannel->host, O_RDONLY );
  }
  else
  {
    fd_io = mas_open( pchannel->host, O_RDONLY );
  }
  if ( fd_io > 0 )
  {
    pchannel->fd_io = fd_io;
#ifdef MAS_CHANNEL_STREAM_WRITE
    pchannel->stream_io = fdopen( fd_io, "r+" );
#endif
    pchannel->opened = 1;
  }
  else
  {
    pchannel->error = 1;
#ifdef EMSG
    P_ERR;
    EMSG( "connect r:%d fd_socket:%d", fd_io, pchannel->fd_socket );
#endif
  }
  return pchannel->opened ? 1 : ( pchannel->error ? -1 : 0 );
}

int
mas_channel_cork( mas_channel_t * pchannel, int val )
{
  int r = -1;
  int q = val;

  if ( pchannel )
  {
#ifdef TCP_CORK
    IEVAL( r, mas_setsockopt( pchannel->fd_io, IPPROTO_TCP, TCP_CORK, &q, sizeof( q ) ) );
#endif
  }
  return r;
}

#ifdef MAS_CHANNEL_OPEN_TUNE
static int
_mas_channel_open_tune( mas_channel_t * pchannel )
{
  int r = 0;
  int yes = 1;

  /* r = mas_setsockopt( pchannel->fd_io, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ); */
  IEVAL( r, mas_setsockopt( pchannel->fd_io, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) );
  /* tMSG( "(%d) SO_REUSEADDR", r ); */


  if ( pchannel->serv.addr.sin_family == AF_INET )
  {
#  ifdef TCP_DEFER_ACCEPT
    IEVAL( r, mas_setsockopt( pchannel->fd_io, IPPROTO_TCP, TCP_DEFER_ACCEPT, &yes, sizeof( yes ) ) );
#  endif
#  ifdef TCP_QUICKACK
    IEVAL( r, mas_setsockopt( pchannel->fd_io, IPPROTO_TCP, TCP_QUICKACK, &yes, sizeof( yes ) ) );
#  endif

/* the TCP_CORK and TCP_NODELAY are mutually exclusive */
#  ifdef TCP_CORK
#    if 0
    /* with 'foolish use' : makes keep-alive very slow ..... */
    IEVAL( r, mas_setsockopt( pchannel->fd_io, IPPROTO_TCP, TCP_CORK, &yes, sizeof( yes ) ) );
#    endif
#  endif
#  ifdef TCP_NODELAY
    IEVAL( r, mas_setsockopt( pchannel->fd_io, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof( yes ) ) );
#  endif
  }
/* #ifdef EMSG                              */
/*   if ( r < 0 )                           */
/*   {                                      */
/*     EMSG( "(%d) can't tune socket", r ); */
/*     P_ERR;                               */
/*   }                                      */
/* #endif                                   */
  return r;
}
#endif

static int
_mas_channel_open_tcp( mas_channel_t * pchannel )
{
  int fd_io = -1;

  pchannel->error = 0;

  if ( pchannel->is_server )
  {
    fd_io = mas_channel_accept( pchannel );
    fcntl( fd_io, F_SETFD, FD_CLOEXEC );
  }
  else
  {
    fd_io = mas_channel_connect( pchannel );
  }
  if ( fd_io > 0 )
  {
    pchannel->fd_io = fd_io;
#ifdef MAS_CHANNEL_STREAM_WRITE
    pchannel->stream_io = fdopen( fd_io, "r+" );
#endif

#ifdef MAS_CHANNEL_OPEN_TUNE
    _mas_channel_open_tune( pchannel );
#endif

    pchannel->opened = 1;
  }
  else
  {
    pchannel->error = 1;
#ifdef EMSG
    P_ERR;
    EMSG( "connect r:%d fd_socket:%d", fd_io, pchannel->fd_socket );
#endif
  }
  return pchannel->opened ? 1 : ( pchannel->error ? -1 : 0 );
}


int
mas_channel_open( mas_channel_t * pchannel )
{
  int r = 0;

  /* MAS_LOG( "to open chn." ); */
  IEVAL( r, mas_channel_test( pchannel ) );
  /* IEVAL( r, pchannel->opened ? 0 : -1 ); */
  if ( !( r < 0 ) && !pchannel->opened )
  {
    if ( pchannel->is_server )
    {
      /* MAS_LOG( "to listem chn." ); */
      /* r = mas_channel_listen( pchannel ); */
      IEVAL( r, mas_channel_listen( pchannel ) );
      /* MAS_LOG( "(%d)listen chn.", r ); */
    }
    if ( r >= 0 )
      switch ( pchannel->type )
      {
      case CHN_NONE:
        break;
      case CHN_SOCKET:
        /* MAS_LOG( "to open socket" ); */
        IEVAL( r, _mas_channel_open_tcp( pchannel ) );
        /* MAS_LOG( "(%d) tcp chn. socket:%d", r, pchannel->fd_socket ); */
        /* MAS_LOG( "(%d)tcp chn. socket:%d", r, pchannel->fd_socket ); */
        break;
      case CHN_RFILE:
        IEVAL( r, _mas_channel_open_rfile( pchannel ) );
        break;
      case CHN_WFILE:
        IEVAL( r, -1 );
        break;
      }
  }
  /* MAS_LOG( "opened chn. %d", r ); */
  return r;
}

static int
_mas_channel_close_tcp( mas_channel_t * pchannel )
{
  int r = 0;

  if ( pchannel->fd_io > 0 )
  {
    /* shutdown( pchannel->fd_io, SHUT_RDWR ); */
    r = mas_close( pchannel->fd_io );
#ifdef EMSG
    if ( r < 0 )
    {
      P_ERR;
      EMSG( "r:%d", r );
      EMSG( "fd_io:%d", pchannel->fd_io );
    }
#endif
    pchannel->fd_io = 0;
  }
  /* pchannel->fd_socket = 0; */
  pchannel->opened = 0;
  return r;
}

static int
_mas_channel_close_file( mas_channel_t * pchannel )
{
  int r = 0;

  if ( pchannel->fd_io > 0 )
  {
    /* shutdown( pchannel->fd_io, SHUT_RDWR ); */
    r = mas_close( pchannel->fd_io );
#ifdef EMSG
    if ( r < 0 )
    {
      P_ERR;
      EMSG( "r:%d", r );
      EMSG( "fd_io:%d", pchannel->fd_io );
    }
#endif
    pchannel->fd_io = 0;
  }
  mas_channel_delete_buffer( pchannel );
  /* pchannel->fd_socket = 0; */
  pchannel->opened = 0;
  return r;
}


int
mas_channel_close_tcp( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
    _mas_channel_close_tcp( pchannel );
  return r;
}

int
mas_channel_close_file( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
    _mas_channel_close_file( pchannel );
  return r;
}


static int
mas_channel_close2_tcp( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
    pchannel->opened = 0;
  return r;
}

static int
mas_channel_close2_file( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
    pchannel->opened = 0;
  return r;
}

int
mas_channel_close( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
  {
    switch ( pchannel->type )
    {
    case CHN_NONE:
      break;
    case CHN_SOCKET:
      IEVAL( r, mas_channel_close_tcp( pchannel ) );
      break;
    case CHN_RFILE:
      IEVAL( r, mas_channel_close_file( pchannel ) );
      break;
    case CHN_WFILE:
      IEVAL( r, -1 );
      break;
    }
    mas_channel_delete_buffer( pchannel );
  }
  /* MAS_LOG( "closed chn. %d", r ); */
  return r;
}

int
mas_channel_close2( mas_channel_t * pchannel )
{
  int r = 0;


  if ( mas_channel_test( pchannel ) && pchannel->opened )
  {
    switch ( pchannel->type )
    {
    case CHN_NONE:
      break;
    case CHN_SOCKET:
      IEVAL( r, mas_channel_close2_tcp( pchannel ) );
      break;
    case CHN_RFILE:
      IEVAL( r, mas_channel_close2_file( pchannel ) );
      break;
    case CHN_WFILE:
      /* IEVAL(r , mas_channel_close2_file( pchannel )); */
      IEVAL( r, -1 );
      break;
    }
  }
  return r;
}

int
mas_channel_opened( mas_channel_t * pchannel )
{
  return ( mas_channel_test( pchannel ) && pchannel->opened );
}
