#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xatom.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_runonce_config_types.h"
#include "mas_runonce_pid.h"

#include "mas_runonce_window.h"

#define MAX_PROPERTY_VALUE_LEN 4096

Display *disp = NULL;

void
mas_runonce_display_create( void )
{
  if ( !( disp = XOpenDisplay( NULL ) ) )
  {
    printf( "Cannot open display.\n" );
  }
}

void
mas_runonce_display_delete( void )
{
  if ( disp )
  {
    XCloseDisplay( disp );
    disp = NULL;
  }
}

Display *
runonce_display( void )
{
  if ( !disp )
    mas_runonce_display_create(  );
  return disp;
}

int
mas_runonce_close_window( unsigned long win, runonce_flags_t flags )
{
  if ( flags.dry )
  {
    printf( " > DRY close 0x%lx\n", win );
  }
  else
    client_msg( ( Window ) ( void * ) win, "_NET_CLOSE_WINDOW", 0, 0, 0, 0, 0 );
  return 0;
}

char *
get_property( Window win, Atom xa_prop_type, char *prop_name, unsigned long *size )
{
  Atom xa_prop_name = 0;
  Atom xa_ret_type = 0;
  int ret_format = 0;
  unsigned long ret_nitems = 0;
  unsigned long ret_bytes_after = 0;
  unsigned long tmp_size = 0;
  unsigned char *ret_prop = NULL;
  char *ret = NULL;

  xa_prop_name = XInternAtom( runonce_display(  ), prop_name, False );

  /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
   *
   * long_length = Specifies the length in 32-bit multiples of the
   *               data to be retrieved.
   *
   * NOTE:  see 
   * http://mail.gnome.org/archives/wm-spec-list/2003-March/msg00067.html
   * In particular:
   *
   *  When the X window system was ported to 64-bit architectures, a
   * rather peculiar design decision was made. 32-bit quantities such
   * as Window IDs, atoms, etc, were kept as longs in the client side
   * APIs, even when long was changed to 64 bits.
   *
   */
  if ( XGetWindowProperty( runonce_display(  ), win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
                           xa_prop_type, &xa_ret_type, &ret_format, &ret_nitems, &ret_bytes_after, &ret_prop ) != Success )
  {
    printf( "Cannot get %s property.\n", prop_name );
    return NULL;
  }
  else
  {
    if ( ret_prop )
    {
      if ( ret_prop && xa_ret_type != xa_prop_type )
      {
        printf( "Invalid type of %s property.\n", prop_name );
      }
      else
      {
        /* null terminate the result to make string handling easier */
        tmp_size = ( ret_format / 8 ) * ret_nitems;
        /* Correct 64 Architecture implementation of 32 bit data */
        if ( ret_format == 32 )
          tmp_size *= sizeof( long ) / 4;
        ret = mas_malloc( tmp_size + 1 );
        memcpy( ret, ret_prop, tmp_size );
        ret[tmp_size] = '\0';

        if ( size )
          *size = tmp_size;
      }
      if ( ret_prop )
        XFree( ret_prop );
    }
  }
  return ret;
}

int
client_msg( Window win, char *msg, unsigned long data0, unsigned long data1, unsigned long data2, unsigned long data3, unsigned long data4 )
{
  XEvent event;
  long mask = SubstructureRedirectMask | SubstructureNotifyMask;

  event.xclient.type = ClientMessage;
  event.xclient.serial = 0;
  event.xclient.send_event = True;
  event.xclient.message_type = XInternAtom( runonce_display(  ), msg, False );
  event.xclient.window = win;
  event.xclient.format = 32;
  event.xclient.data.l[0] = data0;
  event.xclient.data.l[1] = data1;
  event.xclient.data.l[2] = data2;
  event.xclient.data.l[3] = data3;
  event.xclient.data.l[4] = data4;

  if ( XSendEvent( runonce_display(  ), DefaultRootWindow( runonce_display(  ) ), False, mask, &event ) )
  {
    /* printf( "%lx Sent %s event.\n", win, msg ); */
    return EXIT_SUCCESS;
  }
  else
  {
    printf( "Cannot send %s event.\n", msg );
    return EXIT_FAILURE;
  }
}

Window *
get_client_list( unsigned long *size )
{
  Window *client_list;

  if ( ( client_list =
         ( Window * ) get_property( DefaultRootWindow( runonce_display(  ) ), XA_WINDOW, "_NET_CLIENT_LIST", size ) ) == NULL )
  {
    if ( ( client_list =
           ( Window * ) get_property( DefaultRootWindow( runonce_display(  ) ), XA_CARDINAL, "_WIN_CLIENT_LIST", size ) ) == NULL )
    {
      printf( "Cannot get client list properties. \n" "(_NET_CLIENT_LIST or _WIN_CLIENT_LIST)" "\n" );
      return NULL;
    }
  }

  return client_list;
}

char *
get_window_class( Window win )
{
  char *wm_class;
  unsigned long size;

  wm_class = get_property( win, XA_STRING, "WM_CLASS", &size );
  if ( wm_class )
  {
    char *p_0 = strchr( wm_class, '\0' );

    if ( wm_class + size - 1 > p_0 )
      *( p_0 ) = '.';
  }
  return ( wm_class );
}

char *
get_window_title( Window win )
{
  char *title = NULL;

  title = get_property( win, XInternAtom( runonce_display(  ), "UTF8_STRING", False ), "_NET_WM_NAME", NULL );
  if ( !title || !*title )
    title = get_property( win, XA_STRING, "WM_NAME", NULL );
  return title;
}

char *
get_window_role( Window win )
{
  char *role = NULL;

  role = get_property( win, XA_STRING, "WM_WINDOW_ROLE", NULL );
  return role;
}

char *
get_window_icon_name( Window win )
{
  char *role = NULL;

  role = get_property( win, XA_STRING, "WM_ICON_NAME", NULL );
  return role;
}

pid_t
get_window_pid( Window win )
{
  char *spid = NULL;
  unsigned long lpid=0;

  spid = get_property( win, XA_CARDINAL, "_NET_WM_PID", NULL );
  if ( spid )
    lpid = *( ( unsigned long * ) spid );
  mas_free( spid );
  return (pid_t)lpid;
}

int
get_window_desktop_num( Window win )
{
  unsigned long dtid = 0;
  char *sdtid = NULL;

  sdtid = get_property( win, XA_CARDINAL, "_NET_WM_DESKTOP", NULL );
  if ( sdtid )
    dtid = *( ( unsigned long * ) sdtid );
  mas_free( sdtid );
  return ( int ) dtid;
}

void
runonce_wids_set( void )
{
  unsigned long client_list_size = 0;
  Window *client_list = NULL;
  prec_t *pidinfo = runonce_pids_precs(  );

  client_list = get_client_list( &client_list_size );
  for ( unsigned i = 0; i < client_list_size / sizeof( Window ); i++ )
  {
    pid_t wpid = 0;

    wpid = get_window_pid( client_list[i] );
    if ( wpid && pidinfo )
      pidinfo[wpid].wid = ( unsigned long ) client_list[i];
    else
      pidinfo[wpid].wid = 0;
  }
  mas_free( client_list );
  /* .....
   * client_msg( activate, "_NET_CLOSE_WINDOW", 0, 0, 0, 0, 0 );
   * .....
   * */
}
