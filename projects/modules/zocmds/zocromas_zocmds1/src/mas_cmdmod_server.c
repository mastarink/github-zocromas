#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/thtools/mas_ocontrol_tools.h>

#include <mastar/channel/mas_channel.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


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
    mas_channel_t *prchannel = NULL;

    /* cMSG( "srv L %p", prcontrol ); */
    prchannel = prcontrol->h.pchannel;
    /* if ( prchannel )                            */
    /*   sip = mas_channel_ip_string( prchannel ); */
    if ( prchannel )
    {
      if ( prchannel->cli.path.sun_family == AF_UNIX )
        sip = mas_strdup( prchannel->cli.path.sun_path );
      else
        sip = mas_ip_string( &prchannel->cli.addr.sin_addr );
    }

    len = snprintf( cp, bufsz, "\t\t%u(s%lu). %s [<%s>] \ttid:%5u/%4x; [%lx] %s %s %s/%s.%u:%ld:%ld #%u\n", itr,
                    prcontrol->h.serial, sip ? sip : "?", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?",
                    prcontrol->h.tid, prcontrol->h.tid, prcontrol->h.thread, prcontrol->uuid ? prcontrol->uuid : "-",
                    pthread_equal( mas_pthread_self(  ), prcontrol->h.thread ) ? "*" : " ", mas_sstatus( prcontrol->h.status ),
                    mas_ssubstatus( prcontrol->h.substatus ), prcontrol->h.subpoint, prcontrol->h.subresult1, prcontrol->h.subresult2,
                    prcontrol->xch_cnt );
    cp += len;
    bufsz -= len;
  }
  if ( sip )
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
  if ( plcontrol )
  {
    {
      char *aip = NULL;

      plchannel = plcontrol->h.pchannel;
      if ( plchannel )
      {
        if ( plchannel->serv.path.sun_family != AF_UNIX )
          port = plcontrol->port;
        if ( plchannel->serv_instance.path.sun_family == AF_UNIX )
          aip = mas_strdup( plchannel->serv_instance.path.sun_path );
        else
          aip = mas_ip_string( &plchannel->serv_instance.addr.sin_addr );
      }
      {
        CTRL_PREPARE;
        len = snprintf( cp, bufsz, "\t%u(s%lu). %s:%u * %s; \t\t\ttid:%5u/%4x; [%lx] {%lu - %lu = %lu} {%lu - %lu = %lu}\n", ith,
                        plcontrol->h.serial, plcontrol->host, port, aip, plcontrol->h.tid, plcontrol->h.tid, plcontrol->h.thread,
                        plcontrol->clients_came, plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone,
                        ctrl.clients_created, ctrl.clients_removed, ctrl.clients_created - ctrl.clients_removed );
        cp += len;
        bufsz -= len;
      }
      if ( aip )
        mas_free( aip );
    }
    {
      mas_rcontrol_t *prcontrol;

      /* cMSG( "srv S" ); */
      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
      MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
      {
        if ( prcontrol->signature[0] != 'T' || prcontrol->signature[1] != 'R' )
        {
          EVAL_PREPARE;
          ERRRG( "Woooooooooooooooooo" );
          sleep( 100 );
        }
        if ( itr > 20 )
        {
          len = 0;
        }
        else
        {
          len = info_transaction( prcontrol, itr, cp, bufsz );
        }
        cp += len;
        bufsz -= len;
        itr++;
      }

      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
      /* cMSG( "srv E" ); */
    }
  }
  return cp - cp0;
}

static mas_evaluated_t *
info_cmd( STD_CMD_ARGS )
{
  char *buf = NULL;
  size_t bufsz0 = 1024 * 1000;

  /* if ( pheader )                */
  /* {                            */
  /*   pheader->no_len = 1;        */
  /*   pheader->direct_output = 1; */
  /* }                            */
  MAS_LOG( "info cmd" );

  {
    CTRL_PREPARE;
    buf = mas_malloc( bufsz0 );
    {
      char *cp;
      size_t len;
      ssize_t bufsz;
      char s1lts[64] = "";
      char slts[64] = "";
      char splts[64] = "";
      unsigned ith;

      ith = 0;

      *buf = 0;
      cp = buf;
      bufsz = bufsz0;
      {
        if ( ctrl.stamp.first_lts && ctrl.stamp.first_lts != ctrl.stamp.lts )
          mas_tstrflocaltime( s1lts, sizeof( s1lts ), "%Y%m%d %T", ctrl.stamp.first_lts );
        if ( ctrl.stamp.lts )
          mas_tstrflocaltime( slts, sizeof( slts ), "%Y%m%d %T", ctrl.stamp.lts );
        if ( ctrl.stamp.prev_lts )
          mas_tstrflocaltime( splts, sizeof( splts ), "%Y%m%d %T", ctrl.stamp.prev_lts );
      }
      {
        extern unsigned long memory_balance;

        len = snprintf( cp, bufsz,
                        "\n[mem:%lu]"
                        "- (%u/%u)\tuuid:\t%s\n"
                        "\tmain pid:%u; master pid:%u; daemon pid:%u; server pid:%u; \t\t [%lx]\n"
                        "- #%u Server info:" "\t[%u:%s]\n"
                        "\tclients: {%lu - %lu = %lu}\n"
                        "\tfirst lts: %s; lts: %s; prev.lts: %s\n"
                        "- ticker : 0x%lx :: %d;%lu;\n"
                        "- watcher: 0x%lx :: stop:%1d;cnt:%lu;\n\n", memory_balance,
                        level, this_command ? this_command->only_level : 0, pqopts->uuid,
                        ctrl.threads.n.main.pid, ctrl.threads.n.master.pid, ctrl.threads.n.daemon.pid,
                        /* ctrl.threads.n.main.pid, ctrl.threads.n.main.thread, */
                        ctrl.pserver_thread ? ctrl.pserver_thread->pid : 0, ctrl.pserver_thread ? ctrl.pserver_thread->thread : 0,
                        ctrl.restart_cnt,
                        mas_gettid(  ), mas_thread_self_type_name(  ),
                        ctrl.clients_came, ctrl.clients_gone, ctrl.clients_came - ctrl.clients_gone, *s1lts ? s1lts : "-",
                        *slts ? slts : "-", *splts ? splts : "-", ctrl.threads.n.ticker.thread, ctrl.ticker_hide, ctrl.tick_cnt,
                        ctrl.threads.n.watcher.thread, ctrl.watcher_stop, ctrl.watch_cnt );
        cp += len;
        bufsz -= len;
      }
      {
        len = snprintf( cp, bufsz, "- config read:\n" );
        cp += len;
        bufsz -= len;
      }
      {
        for ( int ic = 0; ic < ctrl.loaded_optsv.c; ic++ )
        {
          len = snprintf( cp, bufsz, "\tconfig %d:\t%s\n", ic, ctrl.loaded_optsv.v[ic] );
          cp += len;
          bufsz -= len;
        }
      }
      {
        len = snprintf( cp, bufsz, "- paths:\n" );
        cp += len;
        bufsz -= len;
      }
      {
        extern int mas_tracemem_flag;

        len = snprintf( cp, bufsz,
                        "\texepath:\t%s\n"
                        "\tmsgfilename:\t%s\n"
                        "\tprotodir:\t%s\n"
                        "\tmodsdir:\t%s\n"
                        "\tpidsdir:\t%s\n\n"
                        "\thistorydir:\t%s\n\n"
                        "\tlogdir: \t%s;\n" "\tlogpath:\t%s\n"
                        "\tlog stat (%lu - %lu)\n\n" "\ttracemem:%d\n",
                        ctrl.exepath,
                        pqopts->msgfilename,
                        pqopts->dir.proto, pqopts->dir.mods, pqopts->dir.pids,
                        pqopts->dir.history, pqopts->dir.log, ctrl.logpath, ctrl.log_q_came, ctrl.log_q_gone, mas_tracemem_flag );
        cp += len;
        bufsz -= len;
      }
      {
        int z = 0;

        for ( int ipd = 0; ipd < ctrl.protos_num; ipd++ )
        {
          if ( ctrl.proto_descs[ipd].name )
          {
            if ( !z )
            {
              len = snprintf( cp, bufsz, "- loaded protocols:\n" );
              cp += len;
              bufsz -= len;
              z = 1;
            }
            len = snprintf( cp, bufsz, "\t%u. %s\n", ipd, ctrl.proto_descs[ipd].name );
            cp += len;
            bufsz -= len;
          }
        }
      }
      /* if ( ctrl.loaded_modules_cnt && ctrl.loaded_modules )                  */
      /* {                                                                      */
      /*   for ( int im = 0; im < ctrl.loaded_modules_cnt; im++ )               */
      /*   {                                                                    */
      /*     const char *name;                                                  */
      /*                                                                        */
      /*     name = ( char * ) dlsym( ctrl.loaded_modules[im], "module_name" ); */
      /*   }                                                                    */
      /* }                                                                      */
      {
        mas_lcontrol_t *plcontrol = NULL;
        int z = 0;

        /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
        pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
        len = snprintf( cp, bufsz, "[lcontrols_list %d]\n", ctrl.lcontrols_list ? 1 : 0 );
        cp += len;
        bufsz -= len;
        MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
        {
          if ( !z )
          {
            len = snprintf( cp, bufsz, "- servers:\n" );
            cp += len;
            bufsz -= len;
            z = 1;
          }
          len = info_listener( plcontrol, ith, cp, bufsz );
          cp += len;
          bufsz -= len;
          ith++;
        }
        pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
      }
      /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
    }
    if ( buf )
      buf = mas_realloc( buf, strlen( buf ) + 1 );
  }
  return mas_evaluated_wrap_pchar( buf );
}

static mas_evaluated_t *
stop_cmd( STD_CMD_ARGS )
{
  CTRL_PREPARE;
  ctrl.do_exit = 1;
  return NULL;
}

static mas_evaluated_t *
cls_cmd( STD_CMD_ARGS )
{
/* echo -en "\e[H\e[2J" >&2 */
  fprintf( stderr, "\x1b[H\x1b[2JCLEARED\n" );
  return NULL;
}

static mas_evaluated_t *
restart_cmd( STD_CMD_ARGS )
{
  CTRL_PREPARE;
  ctrl.restart = 1;
  ctrl.do_exit = 1;
  return NULL;
}

static mas_evaluated_t *
exit_cmd( STD_CMD_ARGS )
{
  CTRL_PREPARE;
  ctrl.exit = 1;
  ctrl.do_exit = 1;
  return NULL;
}

static mas_evaluated_t *
sigquit_cmd( STD_CMD_ARGS )
{
  CTRL_PREPARE;
  int r = 0;

  /* HMSG( "KILL -QUIT %u", ctrl.threads.n.main.pid );         */
  /* if ( ctrl.threads.n.main.pid )                            */
  /*   r = kill( ctrl.threads.n.main.pid, SIGQUIT );           */
  /* HMSG( "(%d) KILL -QUIT %u", r, ctrl.threads.n.main.pid ); */

  HMSG( "KILL -QUIT %u", ctrl.pserver_thread->pid );
  if ( ctrl.pserver_thread && ctrl.pserver_thread->pid )
    r = kill( ctrl.pserver_thread->pid, SIGQUIT );
  HMSG( "(%d) KILL -QUIT %u", r, ctrl.pserver_thread->pid );
  return NULL;
}

static mas_evaluated_t *
segv_cmd( STD_CMD_ARGS )
{
  char *p = NULL;
  char ho;

  ho = *p;
  *p = ho;
  return NULL;
}



mas_cmd_t subcmdtable[] = {
  {
   0, NULL, mas_evaluate_list_cmd, NULL}
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
   7,.name = "exit",.function = exit_cmd,.libname = NULL} /* server exit */
  ,
  {
   8,.name = "sigquit",.function = sigquit_cmd,.libname = NULL} /* server exit */
  ,
  {
   9, "segv", segv_cmd, NULL}   /* server segv */
  ,
  {
   999, NULL, NULL, NULL}
};
