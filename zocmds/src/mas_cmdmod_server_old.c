#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/thtools/mas_ocontrol_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
#include <mastar/types/mas_listener_control_types.h>

#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */

/* #ifdef MAS_USE_SERVER_TOOLS                */
/* #  include "server/inc/mas_server_tools.h" */
/* #endif                                     */

/* #include "transaction/inc/mas_transaction_control.h" */

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

/*
this:
  mas_cmdmod_server.c
related:
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
more:
  mas_watcher.c
  mas_ticker.c
  mas_server_sig.c

  mas_list_def.h
*/


static char *
info_cmd( STD_CMD_ARGS )
{
  /* cMSG( "args:%s", args ); */
  char *buf = NULL;
  size_t szbuf = 1024 * 30;

  /* if ( pheader )                */
  /* {                            */
  /*   pheader->no_len = 1;        */
  /*   pheader->direct_output = 1; */
  /* }                            */
  buf = mas_malloc( szbuf );
  {
    char *cp;
    unsigned ith;
    mas_lcontrol_t *plcontrol;

    *buf = 0;
    cp = buf;
    char s1lts[64];
    char slts[64];
    char splts[64];

    {
      mas_tstrftime( s1lts, sizeof( s1lts ), "%Y%m%d %T", ctrl.stamp.first_lts );
      mas_tstrftime( slts, sizeof( slts ), "%Y%m%d %T", ctrl.stamp.lts );
      mas_tstrftime( splts, sizeof( splts ), "%Y%m%d %T", ctrl.stamp.prev_lts );
    }
    snprintf( cp, szbuf - ( cp - buf ),
              "(%u/%u) #%u Server info:\n\tclients: {%lu - %lu = %lu}\n" "\tfirst lts: %s; lts: %s; prev.lts:%s\nmodsdir:%s\n"
              "\t(%lu - %lu) logdir: %s;\n\tlogpath: %s;\n\tserver; pid:%u; \t\ttid:%5u/%4x; [%lx]\n", level,
              this_command ? this_command->only_level : 0, ctrl.restart_cnt, ctrl.clients_came, ctrl.clients_gone,
              ctrl.clients_came - ctrl.clients_gone, s1lts, slts, splts, opts.modsdir, ctrl.log_q_came,
              ctrl.log_q_gone, opts.logdir, ctrl.logpath, ctrl.main_pid, ctrl.main_tid, ctrl.main_tid, ctrl.main_thread );
    ith = 0;
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
    MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
    {
      unsigned port = 0;
      mas_channel_t *plchannel = NULL;

      plchannel = plcontrol ? plcontrol->h.pchannel : NULL;
      if ( plchannel->serv.path.sun_family != AF_UNIX )
        port = plcontrol->port;
      cp += strlen( cp );
      snprintf( cp, szbuf - ( cp - buf ), "\t\t%u(s%lu). %s:%u; \t\t\ttid:%5u/%4x; [%lx] {%lu - %lu = %lu}\n", ith,
                plcontrol->h.serial, plcontrol->host, port, plcontrol->h.tid, plcontrol->h.tid, plcontrol->h.thread,
                plcontrol->clients_came, plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone );
      {
        unsigned itr;
        mas_rcontrol_t *prcontrol;

        /* cMSG( "srv S" ); */
        itr = 0;
        /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
        pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
        MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
        {
          mas_channel_t *pchannel = NULL;
          char *sip = NULL;

          /* cMSG( "srv L %p", prcontrol ); */

          cp += strlen( cp );
          pchannel = prcontrol ? prcontrol->h.pchannel : NULL;
          /* if ( pchannel )                            */
          /*   sip = mas_channel_ip_string( pchannel ); */
          if ( pchannel )
          {
            if ( pchannel->serv.path.sun_family == AF_UNIX )
              sip = mas_strdup( pchannel->cli.path.sun_path );
            else
              sip = mas_ip_string( &pchannel->cli.addr.sin_addr );
          }

          snprintf( cp, szbuf - ( cp - buf ), "\t\t\t%u(s%lu). [<%s>] %s \ttid:%5u/%4x; [%lx] %s %s %6s #%u\n", itr,
                    prcontrol->h.serial, sip ? sip : "?", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?",
                    prcontrol->h.tid, prcontrol->h.tid, prcontrol->h.thread, prcontrol->uuid ? prcontrol->uuid : "-",
                    pthread_equal( mas_pthread_self(  ), prcontrol->h.thread ) ? "*" : " ", mas_sstatus( prcontrol->h.status ),
                    prcontrol->xch_cnt );
          mas_free( sip );
          itr++;
        }
        pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
        /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
        /* cMSG( "srv E" ); */
      }
      ith++;
    }
    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
  }
  buf = mas_realloc( buf, strlen( buf ) + 1 );
  return buf;
}

static char *
stop_cmd( STD_CMD_ARGS )
{
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}

static char *
cls_cmd( STD_CMD_ARGS )
{
/* echo -en "\e[H\e[2J" >&2 */
  fprintf( stderr, "\x1b[H\x1b[2JCLEARED\n" );
  return NULL;
}

static char *
exit_cmd( STD_CMD_ARGS )
{
  ctrl.quit = 1;
  if ( prcontrol )
    prcontrol->qbin = MSG_BIN_QUIT;
  return NULL;
}

static char *
restart_cmd( STD_CMD_ARGS )
{
  ctrl.restart = 1;
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}

char *
quit_cmd( STD_CMD_ARGS )
{
  ctrl.quit = 1;
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}




mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "info", info_cmd, NULL}   /* server info */
  ,
  {2, "listener", NULL, "listener"} /* server listener ... */
  ,
  {3, "stop", stop_cmd, NULL}   /* server stop */
  ,
  {4, "cls", cls_cmd, NULL}     /* server cls */
  ,
  {5,.name = "restart",.function = restart_cmd,.libname = NULL} /* server restart */
  ,
  {6,.name = "exit",.function = exit_cmd,.libname = NULL} /* server exit */
  ,
  {7,.name = "quit",.function = quit_cmd,.libname = NULL} /* server quit */
  ,
  {999, NULL, NULL, NULL}
};
