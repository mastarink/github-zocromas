#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/thtools/mas_ocontrol_tools.h>

#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

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

static size_t
info_transaction( mas_rcontrol_t * prcontrol, unsigned itr, char *cp, size_t bufsz )
{
  size_t len = 0;
  char *sip = NULL;
  char *cp0;

  cp0 = cp;

  if ( prcontrol )
  {
    mas_channel_t *pchannel = NULL;

    /* cMSG( "srv L %p", prcontrol ); */
    pchannel = prcontrol->h.pchannel;
    /* if ( pchannel )                            */
    /*   sip = mas_channel_ip_string( pchannel ); */
    if ( pchannel )
    {
      if ( pchannel->serv.path.sun_family == AF_UNIX )
        sip = mas_strdup( pchannel->cli.path.sun_path );
      else
        sip = mas_ip_string( &pchannel->cli.addr.sin_addr );
    }

    len = snprintf( cp, bufsz, "\t\t\t%u(s%lu). %s [<%s>] \ttid:%5u/%4x; [%lx] %s %s %6s #%u\n", itr,
                    prcontrol->h.serial, sip ? sip : "?", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?",
                    prcontrol->h.tid, prcontrol->h.tid, prcontrol->h.thread, prcontrol->uuid ? prcontrol->uuid : "-",
                    pthread_equal( mas_pthread_self(  ), prcontrol->h.thread ) ? "*" : " ", mas_sstatus( prcontrol->h.status ),
                    prcontrol->xch_cnt );
    cp += len;
    bufsz -= len;
  }
  mas_free( sip );
  return cp - cp0;
}

static size_t
info_listener( mas_lcontrol_t * plcontrol, unsigned ith, char *cp, size_t bufsz )
{
  size_t len = 0;
  unsigned port = 0;
  mas_channel_t *plchannel = NULL;
  char *cp0;
  unsigned itr;

  itr = 0;

  cp0 = cp;
  plchannel = plcontrol ? plcontrol->h.pchannel : NULL;
  if ( plchannel->serv.path.sun_family != AF_UNIX )
    port = plcontrol->port;

  {
    len = snprintf( cp, bufsz, "\t\t%u(s%lu). %s:%u; \t\t\ttid:%5u/%4x; [%lx] {%lu - %lu = %lu}\n", ith,
                    plcontrol->h.serial, plcontrol->host, port, plcontrol->h.tid, plcontrol->h.tid, plcontrol->h.thread,
                    plcontrol->clients_came, plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone );
    cp += len;
    bufsz -= len;
  }
  {
    mas_rcontrol_t *prcontrol;

    /* cMSG( "srv S" ); */
    /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
    pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
    MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
    {
      len = info_transaction( prcontrol, itr, cp, bufsz );
      cp += len;
      bufsz -= len;
      itr++;
    }

    pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
    /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
    /* cMSG( "srv E" ); */
  }
  return cp - cp0;
}

static char *
info_cmd( STD_CMD_ARGS )
{
  cMSG( "args:%s", args );
  char *buf = NULL;
  size_t bufsz0 = 1024 * 30;

  /* if ( pheader )                */
  /* {                            */
  /*   pheader->no_len = 1;        */
  /*   pheader->direct_output = 1; */
  /* }                            */
  buf = mas_malloc( bufsz0 );
  {
    char *cp;
    size_t len;
    mas_lcontrol_t *plcontrol;
    ssize_t bufsz;
    char s1lts[64];
    char slts[64];
    char splts[64];
    unsigned ith;

    ith = 0;

    *buf = 0;
    cp = buf;
    bufsz = bufsz0;

    {
      s1lts[0] = '-';
      s1lts[1] = '\0';
      if ( ctrl.stamp.first_lts && ctrl.stamp.first_lts != ctrl.stamp.lts )
        mas_tstrftime( s1lts, sizeof( s1lts ), "first lts: %Y%m%d %T", ctrl.stamp.first_lts );
      slts[0] = '-';
      slts[1] = '\0';
      if ( ctrl.stamp.lts )
        mas_tstrftime( slts, sizeof( slts ), "lts: %Y%m%d %T", ctrl.stamp.lts );
      splts[0] = '-';
      splts[1] = '\0';
      if ( ctrl.stamp.prev_lts )
        mas_tstrftime( splts, sizeof( splts ), "prev.lts:%Y%m%d %T", ctrl.stamp.prev_lts );
    }
    {
      len = snprintf( cp, bufsz,
                      "(%u/%u) #%u Server info:\n\n\tbin: %s\n\tclients: {%lu - %lu = %lu}\n"
                      "\t%s; %s; %s\n\tmodsdir:%s\n"
                      "\t(%lu - %lu) logdir: %s;\n\tlogpath: %s;\n\tserver; pid:%u; \t\ttid:%5u/%4x; [%lx]\n", level,
                      this_command ? this_command->only_level : 0, ctrl.restart_cnt, ctrl.launcherv[0], ctrl.clients_came,
                      ctrl.clients_gone, ctrl.clients_came - ctrl.clients_gone, s1lts, slts, splts, opts.modsdir, ctrl.log_q_came,
                      ctrl.log_q_gone, opts.logdir, ctrl.logpath, ctrl.main_pid, ctrl.main_tid, ctrl.main_tid, ctrl.main_thread );
      cp += len;
      bufsz -= len;
    }
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
    MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
    {
      len = info_listener( plcontrol, ith, cp, bufsz );
      cp += len;
      bufsz -= len;
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
  return NULL;
}

static char *
restart_cmd( STD_CMD_ARGS )
{
  ctrl.restart = 1;
  ctrl.do_quit = 1;
  return NULL;
}

char *
quit_cmd( STD_CMD_ARGS )
{
  ctrl.quit = 1;
  ctrl.do_quit = 1;
  return NULL;
}




mas_cmd_t subcmdtable[] = {
  {
   0, NULL, list_commands_cmd, NULL}
  ,
  {
   1, "info", info_cmd, NULL}   /* server info */
  ,
  {
   2, "listener", NULL, "listener"} /* server listener ... */
  ,
  {
   3, "stop", stop_cmd, NULL}   /* server stop */
  ,
  {
   4, "cls", cls_cmd, NULL}     /* server cls */
  ,
  {
   5,.name = "restart",.function = restart_cmd,.libname = NULL} /* server restart */
  ,
  {
   6,.name = "exit",.function = exit_cmd,.libname = NULL} /* server exit */
  ,
  {
   7,.name = "quit",.function = quit_cmd,.libname = NULL} /* server quit */
  ,
  {
   999, NULL, NULL, NULL}
};
