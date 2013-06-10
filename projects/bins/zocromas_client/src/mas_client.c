#define MAS_NOPASS_OPTS
#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <unistd.h>
#include <stdlib.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel_open.h>

#ifdef MAS_CLIENT_LOG
#  include <mastar/log/mas_logger.h>
#endif

#include "mas_client_session.h"
#include "mas_client_readline.h"


#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

#include "mas_client.h"


/*
this:
  mas_client.c
related:
  mas_client_session.c
  mas_client_main.c
  mas_client_readline.c
  mas_client_sig.c
  
  mas_init_client.c
  mas_transaction_xcromas.c
  mas_transaction.c

  mas_open.c
  mas_log.c

*/

static int
mas_client_transaction( mas_channel_t * pchannel )
{
  int r = 0;
  unsigned cnt = 0;

  ctrl.in_pipe++;
  do
  {
    cnt++;
    /* WMSG( "(%u:%u) client internal loop %s", ctrl.in_client, ctrl.in_pipe, pchannel->host ); */
    IEVAL( r, mas_exchange_with_readline( pchannel ) );
    WMSG( "(%d)C/T DONE i/c:%d", r, ctrl.in_client );
#ifdef MAS_CLIENT_LOG
    mas_logger_flush(  );
#endif
    /* WMSG( "(%u:%u) / client internal loop %s", ctrl.in_client, ctrl.in_pipe, pchannel->host ); */
  }
  while ( r >= 0 && ctrl.in_client && ctrl.in_pipe );
  WMSG( "CLIENT TR END" );
  ctrl.status = MAS_STATUS_CLOSE;
  tMSG( "after exchange cnt:%d; %d / %d", cnt, ctrl.in_pipe, ctrl.in_client );
  return 0;
}

int
mas_client_zerocommands( mas_channel_t * pchannel )
{
  int r = 0;

  if ( ctrl.in_client )
  {
    MAS_PASS_OPTS_DECL_PREF;
    char *cmd;

    cmd = mas_strdup( "check uuid " );
    cmd = mas_strcat_x( cmd, MAS_PASS_OPTS_PREF uuid );
    /* r = mas_client_exchange( pchannel, cmd, "%s\n" ); */
    IEVAL( r, mas_client_exchange( pchannel, cmd, "%s\n" ) );
    mas_free( cmd );
  }
  if ( ctrl.in_client )
  {
    char *cmd;
    char *args;

    cmd = mas_strdup( "check args " );
    args = mas_argv_string( ctrl.launchervv.c, ctrl.launchervv.v, ctrl.argv_nonoptind );
    cmd = mas_strcat_x( cmd, args );
    mas_free( args );
    /* WMSG( MAS_SEPARATION_LINE ); */
    IEVAL( r, mas_client_exchange( pchannel, cmd, "%s\n" ) );
    mas_free( cmd );
  }
  return r;
}

int
mas_client_autocommands( mas_channel_t * pchannel )
{
  MAS_PASS_OPTS_DECL_PREF;
  int r = 0;

  WMSG( "(%d) OPTS COMMANDS[%u]:", r, MAS_PASS_OPTS_PREF commandsv.c );
  for ( int ic = 0; !( r < 0 ) && ctrl.in_client && ic < MAS_PASS_OPTS_PREF commandsv.c; ic++ )
  {
    WMSG( "(opts) command to execute : '%s'", MAS_PASS_OPTS_PREF commandsv.v[ic] );
    IEVAL( r, mas_client_exchange( pchannel, MAS_PASS_OPTS_PREF commandsv.v[ic], "%s\n" ) );
  }
  WMSG( "(%d) CTRL COMMANDS[%u]:", r, ctrl.commandsv.c );
  for ( int ic = 0; !( r < 0 ) && ctrl.in_client && ic < ctrl.commandsv.c; ic++ )
  {
    WMSG( "(ctrl) command to execute : '%s'", ctrl.commandsv.v[ic] );
    IEVAL( r, mas_client_exchange( pchannel, ctrl.commandsv.v[ic], "%s\n" ) );
  }
  return r;
}

int
mas_client( const char *host_port )
{
  mas_channel_t *pchannel;
  int r = 0, rop = -1;
  unsigned cnt = 0;

  /* if ( prctl( PR_SET_NAME, ( unsigned long ) "zoclient" ) < 0 ) */
  /* {                                                             */
  /*   P_ERR;                                                      */
  /* }                                                             */
  IEVAL( r, prctl( PR_SET_NAME, ( unsigned long ) "zoclient" ) );
  WMSG( "CLIENT" );
  pchannel = mas_channel_create(  );
  while ( r >= 0 && ctrl.in_client && !ctrl.fatal )
  {
    MAS_PASS_OPTS_DECL_PREF;
    size_t hostlen = 0;
    unsigned hport = 0;

    /* mMSG( "host_port:%s", host_port ); */
    WMSG( "(i/c:%u; i/p:%d) client external loop %s", ctrl.in_client, ctrl.in_pipe, host_port );
    WMSG( "to init '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
    WMSG( "R.CLIENT #%u host_port:%s (def.port: %u)", cnt, host_port, MAS_PASS_OPTS_PREF default_port );
    hostlen = mas_parse_host_port( host_port, &hport, MAS_PASS_OPTS_PREF default_port );
    /* r = mas_channel_init( pchannel, 0  , CHN_SOCKET, host_port, hostlen, hport ); */
    IEVAL( r, mas_channel_init( pchannel, 0, CHN_SOCKET, host_port, hostlen, hport ) );
    WMSG( "to open '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
    WMSG( "(%d) CHANNEL INIT", r );

    WMSG( "to open '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
    IEVAL( r, mas_channel_open( pchannel ) );
    rop = r;
    WMSG( "(%d) CHANNEL OPEN", r );

    WMSG( "(%d : %d) AFTER OPEN", r, errno );
    ctrl.status = MAS_STATUS_OPEN;
    if ( 0 )
    {
      IEVAL( r, mas_client_zerocommands( pchannel ) );
    }
    WMSG( "(%d) CONNECTED TO %s [%d]", r, pchannel->host, ctrl.argv_nonoptind );

    IEVAL( r, mas_client_autocommands( pchannel ) );
    WMSG( "(%d)AUTO DONE i/c:%d", r, ctrl.in_client );
    if ( ctrl.in_client )
    {
#if 1
      if ( MAS_PASS_OPTS_PREF disconnect_prompt )
        r = 0;
      IEVAL( r, mas_client_transaction( pchannel ) );
#else
      if ( MAS_PASS_OPTS_PREF disconnect_prompt || r > 0 )
      {
        IEVAL( r, mas_client_transaction( pchannel ) );
      }
      else if ( MAS_PASS_OPTS_PREF wait_server )
      {
        /* if ( ctrl.try_cnt % 100 == 0 ) */
        {
          WMSG( "waiting for server (%d:%d:%d)...\x1b[K\r", ctrl.term_cnt, ctrl.int_cnt, ctrl.try_cnt );
          usleep( 300 );
        }
        ctrl.try_cnt++;
      }
      else
        ctrl.in_client = 0;
#endif
    }
    if ( rop >= 0 )
    {
      IEVAL( r, mas_channel_close( pchannel ) );
      rop = -1;
    }
    WMSG( "(i/c:%u;fatal:%u) %s / external loop", ctrl.in_client, ctrl.fatal, pchannel->host );
    cnt++;
  }
  mas_channel_delete( pchannel, 0, 0 );
  pchannel = NULL;
  ctrl.status = MAS_STATUS_STOP;
  WMSG( "exiting client (%d)", ctrl.try_cnt );
  return r;
}


/* int                                                                               */
/* mas_channel_deaf( mas_channel_t * pchannel )                                      */
/* {                                                                                 */
/*   EMSG( "@@@@channel_deaf@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" );       */
/*   return 0;                                                                       */
/* }                                                                                 */
/*                                                                                   */
/* int                                                                               */
/* mas_channel_listen( mas_channel_t * pchannel )                                    */
/* {                                                                                 */
/*   EMSG( "@@@@mas_channel_listen@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" ); */
/*   return 0;                                                                       */
/* }                                                                                 */
