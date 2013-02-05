#include "mas_basic_def.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel_open.h>

#include "mas_common.h"

#include "log/inc/mas_logger.h"

#include "mas_client_session.h"
#include "mas_client_readline.h"

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

  mas_open.c
  mas_log.c

*/

static int
mas_client_transaction( mas_channel_t * pchannel )
{
  unsigned cnt = 0;

  ctrl.in_pipe++;
  opts.f.bit.msg_c = 0;
  while ( ctrl.in_client && ctrl.in_pipe )
  {
    /* HMSG( "(%u:%u) client internal loop %s", ctrl.in_client, ctrl.in_pipe, pchannel->host ); */
    mas_exchange_with_readline( pchannel );
    /* fprintf( stderr, "after exchange cnt:%d; %d / %d\n", cnt, ctrl.in_pipe, ctrl.in_client ); */
    cnt++;
    tMSG( "i/c:%d; i/p:%d", ctrl.in_client, ctrl.in_pipe );
    /* {                                                */
    /*   int fd;                                        */
    /*                                                  */
    /*   if ( ( fd = mas_channel_fd( pchannel ) ) < 0 ) */
    /*   {                                              */
    /*     EMSG( "fd:%d", fd );                         */
    /*   }                                              */
    /* }                                                */
    mas_logger_flush(  );
    /* HMSG( "(%u:%u) / client internal loop %s", ctrl.in_client, ctrl.in_pipe, pchannel->host ); */
  }
  ctrl.status = MAS_STATUS_CLOSE;
  tMSG( "after exchange cnt:%d; %d / %d", cnt, ctrl.in_pipe, ctrl.in_client );
  return 0;
}

int
mas_client( const char *host_port )
{
  mas_channel_t *pchannel;
  int r = 0, rop = 0;



  if ( prctl( PR_SET_NAME, ( unsigned long ) "zoc_romas" ) < 0 )
  {
    P_ERR;
  }
  pchannel = mas_channel_create(  );
  while ( ctrl.in_client && !ctrl.fatal )
  {
    size_t hostlen = 0;
    unsigned hport = 0;

    /* mMSG( "host_port:%s", host_port ); */
    HMSG( "(i/c:%u; i/p:%d) client external loop %s", ctrl.in_client, ctrl.in_pipe, host_port );
    HMSG( "to init '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
    hostlen = mas_parse_host_port( host_port, &hport, opts.default_port );
    r = mas_channel_init( pchannel, 0 /* is_server */ , CHN_SOCKET, host_port, hostlen, hport );
    HMSG( "to open '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
    if ( r >= 0 )
    {
      int maxit = 0;

      HMSG( "to open '%s' ; fd_socket:%d", host_port, pchannel->fd_socket );
      do
      {
        rop = r = mas_channel_open( pchannel );
        if ( r < 0 )
        {
          HMSG( "restarted %s (delay %10.5f sec)\n", opts.argv[0], opts.restart_sleep );
          mas_nanosleep( opts.restart_sleep );
        }
        maxit++;
      }
      while ( r < 0 && ctrl.restart_cnt && maxit < 5 );
    }
    if ( r < 0 )
    {
      P_ERR;
    }
    HMSG( "(%d : %d) after open", r, errno );
    ctrl.status = MAS_STATUS_OPEN;
    if ( r > 0 )
    {
      char *cmd;

      cmd = mas_strdup( "check uuid " );
      cmd = mas_strcat_x( cmd, opts.uuid );
      r = mas_client_exchange( pchannel, cmd, "%s\n" );
      mas_free( cmd );
    }
    if ( r > 0 )
    {
      char *cmd;

      {
        char *args;

        cmd = mas_strdup( "check args " );
        args = mas_argv_string( ctrl.launcherc, ctrl.launcherv, ctrl.argv_nonoptind );
        cmd = mas_strcat_x( cmd, args );
        mas_free( args );
      }
      /* HMSG( MAS_SEPARATION_LINE ); */
      r = mas_client_exchange( pchannel, cmd, "%s\n" );
      mas_free( cmd );
    }
    HMSG( "connected to %s [%d]", pchannel->host, ctrl.argv_nonoptind );

    if ( opts.commands_num )
    {
      HMSG( "OPTS COMMANDS:" );
      for ( int ic = 0; ic < opts.commands_num; ic++ )
      {
        HMSG( "(opts) command to execute : '%s'", opts.commands[ic] );
        r = mas_client_exchange( pchannel, opts.commands[ic], "%s\n" );
      }
    }
    if ( ctrl.commands_num )
    {
      HMSG( "CTRL COMMANDS:" );
      for ( int ic = 0; ic < ctrl.commands_num; ic++ )
      {
        HMSG( "(ctrl) command to execute : '%s'", ctrl.commands[ic] );
        r = mas_client_exchange( pchannel, ctrl.commands[ic], "%s\n" );
      }
    }

    if ( opts.disconnect_prompt || r > 0 )
    {
      mas_client_transaction( pchannel );
    }
    else if ( opts.wait_server )
    {
      /* perror("Error "); */
      /* if ( ctrl.try_cnt % 100 == 0 ) */
      {
        HMSG( "waiting for server (%d:%d:%d)...\x1b[K\r", ctrl.term_cnt, ctrl.int_cnt, ctrl.try_cnt );
        usleep( 300 );
        /* sleep( 1 ); */
      }
      ctrl.try_cnt++;
    }
    else
    {
      ctrl.in_client = 0;
    }
    /* {                                                */
    /*   int fd;                                        */
    /*                                                  */
    /*   if ( ( fd = mas_channel_fd( pchannel ) ) < 0 ) */
    /*   {                                              */
    /*     EMSG( "fd:%d", fd );                         */
    /*   }                                              */
    /* }                                                */
    if ( rop >= 0 )
    {
      r = mas_channel_close( pchannel );
      if ( r >= 0 )
      {
        HMSG( "closed (%d:%d)", pchannel ? 1 : 0, pchannel ? pchannel->opened : 0 );
      }
      else
      {
        P_ERR;
        EEMSG( "(%d) can't close : (i/c:%u;fatal:%u)", r, ctrl.in_client, ctrl.fatal );
      }
    }
    HMSG( "(i/c:%u;fatal:%u) %s / external loop", ctrl.in_client, ctrl.fatal, pchannel->host );
  }
  /* mas_free( pchannel ); */
  mas_channel_delete( pchannel, 0, 0 );
  pchannel = NULL;
  ctrl.status = MAS_STATUS_STOP;
  HMSG( "exiting client (%d)\x1b[K\r", ctrl.try_cnt );
  return 0;
}


int
mas_channel_deaf( mas_channel_t * pchannel )
{
  return 0;
}

int
mas_channel_listen( mas_channel_t * pchannel )
{
  return 0;
}
