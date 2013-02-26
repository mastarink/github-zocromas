#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include <sys/socket.h>
#include <netdb.h>

#include <mastar/types/mas_common_defs.h>


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

#include "mas_channel_open.h"
#include "mas_channel_listen.h"
#include "mas_channel_object.h"

/*
this:
  mas_channel_object.c
related:
  mas_lcontrol_object.c

  mas_channel.c
  mas_channel_open.c
  mas_channel_listen.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h

  
  mas_listener.c
*/

mas_channel_t *
mas_channel_create( void )
{
  mas_channel_t *pchannel = NULL;

  tMSG( "channel create" );
  pchannel = mas_malloc( sizeof( mas_channel_t ) );
  tMSG( "channel create 2" );

  if ( mas_channel_test( pchannel ) )
    memset( pchannel, 0, sizeof( mas_channel_t ) );
  tMSG( "/channel create" );
  return pchannel;
}

int
mas_channel_reset( mas_channel_t * pchannel, int toclose, int todeaf )
{
  if ( mas_channel_test( pchannel ) )
  {
    if ( toclose )
      mas_channel_close( pchannel );
    if ( todeaf )
      mas_channel_deaf( pchannel );

    if ( pchannel->error_func )
      mas_free( pchannel->error_func );
    pchannel->error_func = NULL;
    {
      char *host;

      host = pchannel->host;
      memset( pchannel, 0, sizeof( mas_channel_t ) );
      pchannel->host = host;
    }
  }
  return 0;
}

int
mas_channel_delete( mas_channel_t * pchannel, int toclose, int todeaf )
{
  tMSG( "to delete channel" );
  if ( mas_channel_test( pchannel ) )
  {
    mas_channel_reset( pchannel, toclose, todeaf );

    if ( pchannel->host )
      mas_free( pchannel->host );
    pchannel->host = NULL;
    mas_free( pchannel );
  }
  return 0;
}

static int
mas_set_address( const char *host, unsigned port, mas_serv_addr_t * sa )
{
  int r = 0;
  struct addrinfo hints;

  if ( sa->addr.sin_family == AF_INET )
    memset( &sa->addr, 0, sizeof( sa->addr ) );
  else
    memset( &sa->path, 0, sizeof( sa->path ) );

  /* int getaddrinfo(const char *node, const char *service, */
  /*                    const struct addrinfo *hints,       */
  /*                    struct addrinfo **res);             */
  /* void freeaddrinfo(struct addrinfo *res);               */
  memset( &hints, 0, sizeof( struct addrinfo ) );
  if ( *host == '/' )
  {
    hints.ai_family = AF_UNIX;
    /* pchannel->serv.addr.sin_family = AF_INET; */
    sa->path.sun_family = AF_UNIX;
  }
  else
  {
    hints.ai_family = AF_INET;  /* Allow IPv4 or IPv6 */
    sa->addr.sin_family = AF_INET;
    /* pchannel->serv.path.sun_family = AF_UNIX; */
  }
  hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;        /* Any protocol */
  if ( sa->path.sun_family == AF_UNIX )
  {
    strncpy( sa->path.sun_path, host, sizeof( sa->path.sun_path ) );
  }
  else
  {
    struct addrinfo *result = NULL, *rp = NULL;

    /* r = getaddrinfo( host, NULL, &hints, &result ); */
    IEVAL( r, getaddrinfo( host, NULL, &hints, &result ) );
    if ( r >= 0 && result )
    {
      tMSG( "(%d) getaddrinfo %s (%p)", r, host, ( void * ) result );

      for ( rp = result; rp != NULL; rp = rp->ai_next )
      {
        char ip[128];
        const char *pip;
        struct sockaddr_in *aip = ( struct sockaddr_in * ) rp->ai_addr;

/* ai_addr : sockaddr */

        sa->addr.sin_addr = aip->sin_addr;
        sa->addr.sin_port = htons( port );
        pip = mas_inet_ntop( AF_INET, &sa->addr.sin_addr, ip, sizeof( ip ) );
        tMSG( "ip 0x%X : %s", htonl( *( ( unsigned int * ) &sa->addr.sin_addr ) ), pip );
        tMSG( "ip 0x%X : %s", htonl( sa->addr.sin_addr.s_addr ), pip );
      }
      if ( result )
        freeaddrinfo( result );
    }
#ifdef EMSG
    else
    {
      P_ERR;
      EMSG( "address error : '%s':%u", host, port );
    }
#endif
  }
/*   if ( sa->addr.sin_family == AF_INET )                       */
/*   {                                                           */
/*     r = mas_inet_pton( AF_INET, host, &sa->addr.sin_addr );   */
/* #ifdef EMSG                                                   */
/*     if ( r <= 0 )                                             */
/*     {                                                         */
/*       P_ERR;                                                  */
/*       EMSG( "(%d) inet_pton error occured @ '%s'", r, host ); */
/*     }                                                         */
/* #endif                                                        */
/*   }                                                           */
  return r;
}

static int
__mas_channel_init( mas_channel_t * pchannel, int is_server, chn_type_t type, const char *host, size_t hostlen, int port )
{
  int r = 0;

  MAS_LOG( "init chn. w/%s:%d", host, port );
  mas_channel_reset( pchannel, 1, 1 );
  pchannel->is_server = is_server ? 1 : 0;
  pchannel->type = type ? type : CHN_SOCKET;
  if ( !pchannel->host )
  {
    pchannel->host = mas_strndup( host, hostlen );
  }
  /* r = mas_set_address( pchannel->host, port, &pchannel->serv ); */
  IEVAL( r, mas_set_address( pchannel->host, port, &pchannel->serv ) );
  if ( pchannel->serv.addr.sin_family == AF_INET )
    pchannel->port = port;

  if ( r >= 0 )
  {
    pchannel->type = type;
    if ( !pchannel->fd_socket )
    {
      /* tMSG( "(%d) to create [%d] l/socket (was:%d)", r, pchannel->type, pchannel->fd_socket ); */
      /* int keepvalue = 0; */

      /* r = pchannel->fd_socket = socket( pchannel->serv.path.sun_family, SOCK_STREAM, 0 ); */
      IEVAL( r, socket( pchannel->serv.path.sun_family, SOCK_STREAM, 0 ) );
      pchannel->fd_socket = r;
      /* r = mas_setsockopt( pchannel->fd_socket, SOL_SOCKET, SO_KEEPALIVE, ( void * ) &keepvalue, sizeof( keepvalue ) ); */
      /*  socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) */
      tMSG( "(%d) create l/socket", r );
    }
  }

/* #ifdef EMSG                                      */
/*   if ( r < 0 )                                   */
/*   {                                              */
/*     P_ERR;                                       */
/*     EMSG( "fd_socket:%d", pchannel->fd_socket ); */
/*   }                                              */
/* #endif                                           */
  return r;
}

static int
_mas_channel_init( mas_channel_t * pchannel, int is_server, chn_type_t type, const char *host, size_t hostlen, int port )
{
  int r = 0;

  MAS_LOG( "chn. initing from %s (def.p:%u)", host, port );
  tMSG( "chn. initing from %s (def.p:%u)", host, port );

  if ( mas_channel_test( pchannel ) && !pchannel->opened )
  {
    /* r = __mas_channel_init( pchannel, is_server, type, host, hostlen, port ); */
    IEVAL( r, __mas_channel_init( pchannel, is_server, type, host, hostlen, port ) );
  }
  return r;
}

int
mas_channel_init( mas_channel_t * pchannel, int is_server, chn_type_t type, const char *host, size_t hostlen, int port )
{
  int r = 0;

  if ( host )
  {
    /* r = _mas_channel_init( pchannel, is_server, type, host, hostlen, port ); */
    IEVAL( r, _mas_channel_init( pchannel, is_server, type, host, hostlen, port ) );
  }
/* #ifdef EMSG    */
/*   if ( r < 0 ) */
/*   {            */
/*     P_ERR;     */
/*   }            */
/* #endif         */
  return r;
}

int
mas_channel_test( const mas_channel_t * pchannel )
{
  int r;

  r = pchannel ? 1 : 0;
  if ( !r )
  {
    MAS_LOG( "pchannel not set" );
    IEVAL( r, -1 );
  }
  return r;
}
