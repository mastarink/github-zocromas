#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_channel_socket.h"
#include "mas_channel_object.h"
#include "mas_channel_listen.h"
#include "mas_channel_open.h"

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
    pchannel->stream_io = fdopen( fd_io, "r+" );
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
  if ( mas_channel_test( pchannel ) && !pchannel->opened )
  {
    if ( r >= 0 )
    {
      /* MAS_LOG( "to listem chn." ); */
      r = mas_channel_listen( pchannel );
      /* MAS_LOG( "(%d)listen chn.", r ); */
    }
    if ( r >= 0 )
      switch ( pchannel->type )
      {
      case CHN_SOCKET:
        /* MAS_LOG( "to open socket" ); */
        r = _mas_channel_open_tcp( pchannel );
        /* MAS_LOG( "(%d) tcp chn. socket:%d", r, pchannel->fd_socket ); */
        /* MAS_LOG( "(%d)tcp chn. socket:%d", r, pchannel->fd_socket ); */
      }
#ifdef EMSG
    if ( r < 0 )
    {
      P_ERR;
    }
#endif
  }
  else
  {
#ifdef EMSG
    EMSG( "pchannel not defined or opened (%d:%d)", pchannel ? 1 : 0, pchannel ? pchannel->opened : 0 );
#endif
    r = -1;
  }

#ifdef EMSG
  if ( r < 0 )
  {
    EMSG( "?" );
  }
#endif
  /* MAS_LOG( "opened chn. %d", r ); */
  return r;
}

int
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

int
mas_channel_close_tcp( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) && pchannel->opened )
    _mas_channel_close_tcp( pchannel );
  return r;
}

int
mas_channel_close_tcp2( mas_channel_t * pchannel )
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
    case CHN_SOCKET:
      r = mas_channel_close_tcp( pchannel );
      break;
    }
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
    case CHN_SOCKET:
      r = mas_channel_close_tcp2( pchannel );
      break;
    }
  }
  return r;
}
