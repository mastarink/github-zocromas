#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>



#include <string.h>
#include <stdlib.h>

/* #include <gtk/gtk.h>          */
/* #include <libnotify/notify.h> */
#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>



#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_logger.h>
#include <mastar/log/mas_log.h>

#include <mastar/thvariables/mas_thread_variables.h>
#ifdef MAS_USE_LISTENER
#  include <mastar/listener/mas_listener_control.h>
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
  cMSG( "##              >>mC<<\n" );
}

__attribute__ ( ( destructor ) )
     static void d_notify( void )
{
  cMSG( "##              >>mD<<\n" );
  /* HMSG( "NOTIFY UNINIT" ); */
  /* notify_uninit(  ); */
  /* HMSG( "NOTIFY UNINIT done" ); */
}

static char *
msg_cmd( STD_CMD_ARGS )
{
  cMSG( ">>[%s]<<\n", args );
  return NULL;
}

static NotifyNotification *notif = NULL;

/* static void                                   */
/* n_closed( GtkWidget * widget, gpointer data ) */
/* {                                             */
/*   HMSG( "NOTIFY CLOSED" );                    */
/*   (* g_object_unref( G_OBJECT( notif ) ); *)  */
/*   notify_uninit(  );                          */
/*   (* gtk_main_quit(  ); *)                    */
/*   HMSG( "NOTIFY CLOSED done" );               */
/*   notif = NULL;                               */
/* }                                             */

static char *
msg_nt( STD_CMD_ARGS )
{
  GError *error = NULL;

  g_type_init(  );

  /* g_set_prgname( argv[0] );                                           */
  g_log_set_always_fatal( G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL );


  /* static int gi = 0;          */
  /*                             */
  /* if ( !gi )                  */
  /* {                           */
  /*   int argc;                 */
  /*   char **argv=NULL;         */
  /*   gtk_init( &argc, &argv ); */
  /*   gi = 1;                   */
  /* }                           */

  if ( !notify_is_initted(  ) )
    notify_init( "zocromas" );
  if ( !notif )
#ifdef NOTIFY_CHECK_VERSION
#  if NOTIFY_CHECK_VERSION (0, 7, 0)
    notif = notify_notification_new( args, NULL, NULL );
#  else
    notif = notify_notification_new( args, NULL, NULL, NULL );
#  endif
#else
    notif = notify_notification_new( args, NULL, NULL, NULL );
#endif


  /* notify_notification_set_category( notify, type );          */
  /* notify_notification_set_urgency( notify, urgency );        */
  /* notify_notification_set_timeout( notify, expire_timeout ); */
  /* notify_notification_set_app_name( notify, app_name );      */

  /* g_signal_connect( notif, "closed", ( GCallback ) n_closed, NULL ); */
  /* notify_notification_set_timeout( notif, NOTIFY_EXPIRES_NEVER ); */
  notify_notification_set_timeout( notif, 10000 );
  /* if ( accepts_actions )                                                                              */
  /* {                                                                                                   */
  /*   notify_notification_add_action( notif, "Wow", "Lab", cb, ( "Hooo" ), ff (* ff not working? *)  ); */
  /*   notify_notification_add_action( notif, "quit", "Quit", cb, ( "Hooo" ), NULL );                    */
  /* }                                                                                                   */
  /* notify_notification_update( notif, "*zocromas*", args, NULL ); */
  notify_notification_show( notif, &error );

  /*                                                                      */
  /* gtk_main(  ); */
  g_object_unref( G_OBJECT( notif ) );
  notify_uninit(  );
  notif = NULL;
  return NULL;
}

static char *
msg_test( STD_CMD_ARGS )
{
  NotifyNotification *notify;

  g_type_init(  );
  g_set_prgname( "Test" );
  g_log_set_always_fatal( G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL );
  if ( !notify_init( "notify-send" ) )
    exit( 1 );
#ifdef NOTIFY_CHECK_VERSION
#  if NOTIFY_CHECK_VERSION (0, 7, 0)
  notify = notify_notification_new( "Hello", "Wow", NULL );
  notify_notification_set_app_name( notify, "Boooo" );
#  else
  notify = notify_notification_new( "Hello", "Wow", NULL, NULL );
#  endif
#else
  notify = notify_notification_new( "Hello", "Wow", NULL, NULL );
#endif


  notify_notification_set_category( notify, "the category" );
  notify_notification_set_urgency( notify, NOTIFY_URGENCY_NORMAL );
  notify_notification_set_timeout( notify, NOTIFY_EXPIRES_DEFAULT );
  notify_notification_show( notify, NULL );
  g_object_unref( G_OBJECT( notify ) );
  notify_uninit(  );
  return 0;
}

int
module_before_close( void )
{
  /* notify_uninit(  ); */
  /* gtk_main_quit(  ); */
  HMSG( "MODULE BEFORE CLOSE" );
  return 0;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "msg", msg_cmd, NULL}
  ,
  {2, "test", msg_test, NULL}
  ,
  {3, "nt", msg_nt, NULL}
  ,
  {5, "*", msg_nt, NULL}
  ,
  {999, NULL, NULL, NULL}
};
