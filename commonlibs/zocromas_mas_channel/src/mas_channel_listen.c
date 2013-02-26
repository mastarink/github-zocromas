#include <stdlib.h>
#include <errno.h>
#include <mastar/types/mas_common_defs.h>


#include <netinet/tcp.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_channel.h"
#include "mas_channel_socket.h"
#include "mas_channel_object.h"

#include "mas_channel_listen.h"

/*
this:
  mas_channel_listen.c
related:
  mas_channel.c
  mas_channel_socket.c
  mas_channel_open.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h

*/


/*
unsigned int s, s2;
struct sockaddr_un local, remote;
int len;
s = socket(AF_UNIX, SOCK_STREAM, 0);

local.sun_family = AF_UNIX;  
strcpy(local.sun_path, "/home/beej/mysocket");
unlink(local.sun_path);
len = strlen(local.sun_path) + sizeof(local.sun_family);
mas_bind(s, (struct sockaddr *)&local, len);
mas_listen(s, 5);
*/


/* int                                                 */
/* mas_channel_print_addr( mas_channel_t * pchannel )  */
/* {                                                   */
/*   char *pip;                                        */
/*                                                     */
/*   pip = mas_channel_ip_string( pchannel );          */
/*   tMSG( "(s:%d) ip %s", pchannel->fd_socket, pip ); */
/*   mas_free( pip );                                  */
/*   return 0;                                         */
/* }                                                   */

static int
_mas_channel_tune_socket( mas_channel_t * pchannel )
{
  int r = 0;
  int yes = 1;

  /* r = mas_setsockopt( pchannel->fd_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ); */
  IEVAL( r, mas_setsockopt( pchannel->fd_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) );
  /* tMSG( "(%d) SO_REUSEADDR", r ); */


  if ( pchannel->serv.addr.sin_family == AF_INET )
  {
    /* r = mas_setsockopt( pchannel->fd_socket, IPPROTO_TCP, TCP_DEFER_ACCEPT, &yes, sizeof( yes ) ); */
    IEVAL( r, mas_setsockopt( pchannel->fd_socket, IPPROTO_TCP, TCP_DEFER_ACCEPT, &yes, sizeof( yes ) ) );
  }
  /* tMSG( "(%d) TCP_DEFER_ACCEPT", r ); */
  /* if ( r >= 0 && pchannel->serv.addr.sin_family == AF_INET )                               */
  /*   r = mas_setsockopt( pchannel->fd_socket, IPPROTO_TCP, TCP_CORK, &yes, sizeof( yes ) ); */
  if ( pchannel->serv.addr.sin_family == AF_INET )
  {
    /* r = mas_setsockopt( pchannel->fd_socket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof( yes ) ); */
    IEVAL( r, mas_setsockopt( pchannel->fd_socket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof( yes ) ) );
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

static int
mas_channel_listen_tcp( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) )
  {
    /* r = _mas_channel_tune_socket( pchannel ); */
    IEVAL( r, _mas_channel_tune_socket( pchannel ) );
    /* if ( r >= 0 )                       */
    /*   r = mas_channel_bind( pchannel ); */
    IEVAL( r, mas_channel_bind( pchannel ) );
    /* if ( r >= 0 )                               */
    /*   r = mas_listen( pchannel->fd_socket, 5 ); */
    IEVAL( r, mas_listen( pchannel->fd_socket, 5 ) );
/* #ifdef EMSG                           */
/*     if ( r < 0 )                      */
/*     {                                 */
/*       EMSG( "(%d) can't listen", r ); */
/*       P_ERR;                          */
/*     }                                 */
/* #endif                                */
    /* tMSG( "(%d) listening (sock:%d)? ", r, pchannel->fd_socket ); */
  }
  return r;
}

int
_mas_channel_listen( mas_channel_t * pchannel )
{
  int r = 0;

  switch ( pchannel->type )
  {
  case CHN_SOCKET:
    /* r = mas_channel_listen_tcp( pchannel ); */
    IEVAL( r, mas_channel_listen_tcp( pchannel ) );
    break;
  default:
    IEVAL( r, -1 );
    break;
  }
  if ( r >= 0 )
    pchannel->listening = 1;
/* #ifdef EMSG                         */
/*   else                              */
/*   {                                 */
/*     EMSG( "(%d) can't listen", r ); */
/*     P_ERR;                          */
/*   }                                 */
/* #endif                              */
  return r;
}

int
mas_channel_listen( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) )
  {
    if ( pchannel->is_server && !pchannel->listening )
    {
      /* r = _mas_channel_listen( pchannel ); */
      IEVAL( r, _mas_channel_listen( pchannel ) );
    }
  }
/* #ifdef EMSG         */
/*   else              */
/*   {                 */
/*     IEVAL( r, -1 ); */
/*   }                 */
/* #endif              */
  return r;
}

int
mas_channel_deaf( mas_channel_t * pchannel )
{
  int r = 0;

  if ( mas_channel_test( pchannel ) )
  {
    if ( pchannel->is_server && pchannel->listening )
    {
      switch ( pchannel->type )
      {
      case CHN_SOCKET:
        if ( pchannel->fd_socket > 0 )
        {
          /* r = mas_close( pchannel->fd_socket ); */
          /* if ( r < 0 )                          */
          /* {                                     */
          /*   EMSG( "deaf err" );                 */
          /*   P_ERR;                              */
          /* }                                     */
          IEVAL( r, mas_close( pchannel->fd_socket ) );
          if ( r == 0 )
          {
            pchannel->fd_socket = 0;
            if ( pchannel->serv.path.sun_family == AF_UNIX && pchannel->serv.path.sun_path[0] )
            {
              IEVAL( r, unlink( pchannel->serv.path.sun_path ) );
            }
          }
        }
        break;
      }
      if ( r >= 0 )
        pchannel->listening = 0;
    }
  }
  /* if ( r < 0 )            */
  /* {                       */
  /*   if ( errno )          */
  /*   {                     */
  /*     EMSG( "deaf err" ); */
  /*   }                     */
  /*   P_ERR;                */
  /* }                       */
  return r;
}
