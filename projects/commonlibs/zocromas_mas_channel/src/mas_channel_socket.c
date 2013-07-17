#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdlib.h>
#include <errno.h>

#include <netinet/tcp.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/tools/mas_tools.h>


#include "mas_channel.h"
#include "mas_channel_object.h"
#include "mas_channel_socket.h"

/*
this:
  mas_channel_socket.c
related:
  mas_channel_object.c
  mas_channel.c
  mas_channel_open.c
  mas_channel_listen.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h

*/

int
mas_channel_bind( mas_channel_t * pchannel )
{
  int r = -1;
  struct sockaddr *sa = NULL;
  socklen_t l;

  if ( mas_channel_test( pchannel ) )
  {
    if ( pchannel->serv.path.sun_family == AF_UNIX )
    {
      l = strlen( pchannel->serv.path.sun_path ) + sizeof( pchannel->serv.path.sun_family );
      sa = ( struct sockaddr * ) &pchannel->serv.path;
    }
    else
    {
      l = sizeof( pchannel->serv.addr );
      sa = ( struct sockaddr * ) &pchannel->serv.addr;
    }
    r = mas_bind( pchannel->fd_socket, sa, l );
#ifdef EMSG
    if ( r < 0 )
    {
      P_ERR;
    }
#endif
  }
  return r;
}

int
mas_channel_accept( mas_channel_t * pchannel )
{
  int fd_io = -1;
  struct sockaddr *sac = NULL;
  struct sockaddr *saa = NULL;
  socklen_t lc;
  socklen_t la;

  if ( mas_channel_test( pchannel ) )
  {
    if ( pchannel->serv.path.sun_family == AF_UNIX )
    {
      lc = sizeof( pchannel->cli.path );
      la = sizeof( pchannel->serv_instance.path );
      sac = ( struct sockaddr * ) &pchannel->cli.path;
      saa = ( struct sockaddr * ) &pchannel->serv_instance.path;
    }
    else
    {
      lc = sizeof( pchannel->cli.addr );
      la = sizeof( pchannel->serv_instance.addr );
      sac = ( struct sockaddr * ) &pchannel->cli.addr;
      saa = ( struct sockaddr * ) &pchannel->serv_instance.addr;
    }
    fd_io = mas_accept( pchannel->fd_socket, sac, &lc );

    /* move this to when need ip address of client etc */
    if ( 1 )
    {
      char *ip = NULL;

      getsockname( fd_io, saa, &la );
      ip = mas_ip_string( &pchannel->serv_instance.addr.sin_addr );
      HMSG( "ACCEPT AT: %s", ip );
      mas_free( ip );
    }
  }
  return fd_io;
}

int
mas_channel_connect( mas_channel_t * pchannel )
{
  int fd_io = -1;
  int r = -1;
  struct sockaddr *sa = NULL;
  socklen_t l;

  if ( mas_channel_test( pchannel ) )
  {
    if ( pchannel->serv.path.sun_family == AF_UNIX )
    {
      l = strlen( pchannel->serv.path.sun_path ) + sizeof( pchannel->serv.path.sun_family );
      sa = ( struct sockaddr * ) &pchannel->serv.path;
    }
    else
    {
      l = sizeof( pchannel->serv.addr );
      sa = ( struct sockaddr * ) &pchannel->serv.addr;
    }
    r = mas_connect( pchannel->fd_socket, sa, l );
    if ( r == 0 )
    {
      fd_io = pchannel->fd_socket;
      pchannel->fd_socket = 0;
    }
    else
      fd_io = r;
  }
  return fd_io;
}
