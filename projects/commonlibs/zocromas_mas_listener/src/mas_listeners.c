/* ميخائيل */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* pthread_cleanup_push and pthread_cleanup_pop are macros and must always */
/* be used in matching pairs at the same nesting level of braces.          */
#include <pthread.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


/* #include "mas_common.h" */
#include <mastar/log/mas_log.h>

#include "mas_listener_control_list.h"
#include "mas_listener.h"

#include "mas_listeners.h"


/*
this:
  mas_listeners.c
related:
  mas_listener.c
more:
  mas_control_types.h
  mas_lcontrol_object.c
  mas_lcontrol_object.h
  mas_listener_control.c
  mas_listener_control.h
  mas_listener_control_list.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.h
  mas_listener_wait_client.c
  mas_listener_wait_client.h
  mas_server_main.c
  mas_master.c

  mas_cmdmod_listener.c
*/



int
mas_listeners_start( const mas_options_t * popts )
{
  int r = 0;

  if ( popts->wolistener )
  {
    MAS_LOG( "stopped run w/o listeners" );
    sleep( popts->wolistener );
  }
  else
  {
    CTRL_PREPARE;
    MAS_LOG( "to start listeners" );
    lMSG( "to start listeners" );
    HMSG( "LISTENERS START" );
    for ( unsigned ith = 0; ith < popts->hostsv.c; ith++ )
    {
      if ( popts->hostsv.v[ith] )
      {
        lMSG( "host %d: '%s'", ith, popts->hostsv.v[ith] );
        HMSG( "LISTEN AT (#%d) %s (def.port:%u)", ith + 1, popts->hostsv.v[ith], popts->default_port );
        MAS_LOG( "to start listener #%d %s", ith + 1, popts->hostsv.v[ith] );
        r = mas_listener_start( popts, popts->hostsv.v[ith], popts->default_port );
      }
      else
      {
        EMSG( "no host for %d", ith );
        MAS_LOG( "no host for #%d", ith );
        r = -1;
      }
      if ( r < 0 )
        break;
    }
    ctrl.status = MAS_STATUS_OPEN;
  }
  return r;
}

/* returns stopped counter */
int
mas_listeners_stop( void )
{
  CTRL_PREPARE;
  int stopped = 0;

  if ( ctrl.lcontrols_list )
  {
    int cancelled;

    cancelled = mas_listeners_cancel(  );
    HMSG( "STOP : cancelled %d", cancelled );
  }
  stopped = mas_listeners_wait(  );
  return stopped;
}

/* returns cancelled counter */
int
mas_listeners_cancel( void )
{
  CTRL_PREPARE;
  int cancelled = 0;

  if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    mas_lcontrol_t *plcontrol = NULL;

    WMSG( "CANCEL l's" );
    MAS_LOG( "cancelling listeners" );
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
    MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
    {
      if ( mas_listener_cancel( plcontrol ) == 0 )
        cancelled++;
    }
    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
  }
  return cancelled;
}

/* returns stopped counter */
int
mas_listeners_wait( void )
{
  CTRL_PREPARE;
  int stopped = 0;
  mas_lcontrol_t *plcontrol = NULL;

  MAS_LOG( "to wait for listeners to stop ..." );
  lMSG( "to wait for listeners to stop ..." );
  while ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) && ( plcontrol = MAS_LIST_FIRST( ctrl.lcontrols_list ) ) )
  {
    stopped += ( mas_listener_wait( plcontrol ) == 0 );
    mas_lcontrols_clean_list( 0 );
  }
  ctrl.status = MAS_STATUS_CLOSE;
  /* ??????????? */
  mas_lcontrols_clean_list( 0 );

  ctrl.status = MAS_STATUS_END;
  if ( stopped )
  {
    thMSG( "joined l/th's" );
    thMSG( "finished waiters" );
    HMSG( "STOPPED listeners (%d)", stopped );
    MAS_LOG( "stopped listeners (%d)", stopped );
  }
  return stopped;
}

int
mas_listeners_main( const mas_options_t * popts )
{
  CTRL_PREPARE;

  int r;
  int qstopped = 0;

  MAS_LOG( "master loop for %d hosts", popts->hostsv.c );
  HMSG( "MASTER LOOP %d host; parent:%d", popts->hostsv.c, ctrl.is_parent );
  /* mas_listeners.c */
  r = mas_listeners_start( popts );

  HMSG( "WAITING..." );
  qstopped = mas_listeners_wait(  );
  HMSG( "STOPPED lsn's (%d)", qstopped );

  tMSG( "(%d) master loop for %d hosts", r, popts->hostsv.c );

  {
    /* ???????? All listeners closed, what shall I do ?
     * 1. exit ( what is to be done with 'ctrl.keep_listening = 0' )
     * 2. re-run default listener(s)
     * 3. re-run defined ( popts->  hostsv.v ) listeners ( what is to be done without 'ctrl.keep_listening = 0' )
     * */
    if ( MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
    {
      ctrl.keep_listening = 0;
      MAS_LOG( "listeners list is empty" );
    }
    else
    {
      MAS_LOG( "listeners list NOT is empty" );
    }
  }
  return r;
}
