#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <libnotify/notify.h>


#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */
/* #include "server/inc/mas_ticker.h" */

#include <mastar/log/mas_logger.h>
#include <mastar/log/mas_log.h>

#include <mastar/variables/mas_thread_variables.h>
#ifdef MAS_USE_LISTENER
#  include "listener/inc/mas_listener_control.h"
#endif

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

/*
this:
  mas_cmdmod_notify.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


__attribute__ ( ( constructor ) )
     static void c_notify( void )
{
  fprintf( ctrl.msgfile ? ctrl.msgfile : stderr, "##              >>C<<\n" );
  cMSG( "##              >>mC<<\n" );
  notify_init( "zocromas" );
}

__attribute__ ( ( destructor ) )
     static void d_notify( void )
{
  fprintf( ctrl.msgfile ? ctrl.msgfile : stderr, "##              >>D<<\n" );
  cMSG( "##              >>mD<<\n" );
  notify_uninit(  );
}

static char *
msg_cmd( STD_CMD_ARGS )
{
  fprintf( ctrl.msgfile ? ctrl.msgfile : stderr, ">>[%s]<<\n", args );
  return NULL;
}

/* static void                                       */
/* n_closed( GtkWidget * widget, gpointer data )     */
/* {                                                 */
/*   fprintf( stderr, ">>> %s\n", ( char * ) data ); */
/*   gtk_main_quit(  );                              */
/* }                                                 */

static char *
msg_nt( STD_CMD_ARGS )
{
  static NotifyNotification *notif = NULL;
  GError *error = NULL;

  if ( !notif )
    notif = notify_notification_new( args, NULL, NULL );
  /* notify_notification_set_timeout( notif, NOTIFY_EXPIRES_NEVER ); */
  notify_notification_set_timeout( notif, 10000 );
  /* if ( accepts_actions )                                                                              */
  /* {                                                                                                   */
  /*   notify_notification_add_action( notif, "Wow", "Lab", cb, ( "Hooo" ), ff (* ff not working? *)  ); */
  /*   notify_notification_add_action( notif, "quit", "Quit", cb, ( "Hooo" ), NULL );                    */
  /* }                                                                                                   */
  notify_notification_update( notif, "*zocromas*", args, NULL );
  notify_notification_show( notif, &error );

  /* g_signal_connect( notif, "closed", ( GCallback ) n_closed, clabel ); */
  /*                                                                      */
  /* gtk_main(  );                                                        */
  return NULL;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "msg", msg_cmd, NULL}
  ,
  {2, "nt", msg_nt, NULL}
  ,
  {999, NULL, NULL, NULL}
};
