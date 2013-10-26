/* license {{{ */
/* 

wmctrl
A command line tool to interact with an EWMH/NetWM compatible X Window Manager.

Author, current maintainer: Tomas Styblo <tripie@cpan.org>

Copyright (C) 2003

This program is free software which I release under the GNU General Public
License. You may redistribute and/or modify this program under the terms
of that license as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

To get a copy of the GNU General Puplic License,  write to the
Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <glib.h>

#include "mas_runonce_window.h"

#define _NET_WM_STATE_REMOVE        0 /* remove/unset property */
#define _NET_WM_STATE_ADD           1 /* add/set property */
#define _NET_WM_STATE_TOGGLE        2 /* toggle property  */


#define MAX_PROPERTY_VALUE_LEN 4096
#define SELECT_WINDOW_MAGIC ":SELECT:"
#define ACTIVE_WINDOW_MAGIC ":ACTIVE:"

#define p_verbose(...) if (options.verbose) { \
    fprintf(stderr, __VA_ARGS__); \
}


static struct
{
  int verbose;
  int force_utf8;
  int show_class;
  int show_pid;
  int show_geometry;
  int match_by_id;
  int match_by_cls;
  int full_window_title_match;
  int wa_desktop_titles_invalid_utf8;
  char *param_window;
  char *param;
} options;

static gboolean envir_utf8;



int
client_msg( Display * disp, Window win, char *msg, unsigned long data0, unsigned long data1, unsigned long data2, unsigned long data3,
            unsigned long data4 )
{
  XEvent event;
  long mask = SubstructureRedirectMask | SubstructureNotifyMask;

  event.xclient.type = ClientMessage;
  event.xclient.serial = 0;
  event.xclient.send_event = True;
  event.xclient.message_type = XInternAtom( disp, msg, False );
  event.xclient.window = win;
  event.xclient.format = 32;
  event.xclient.data.l[0] = data0;
  event.xclient.data.l[1] = data1;
  event.xclient.data.l[2] = data2;
  event.xclient.data.l[3] = data3;
  event.xclient.data.l[4] = data4;

  if ( XSendEvent( disp, DefaultRootWindow( disp ), False, mask, &event ) )
  {
    return EXIT_SUCCESS;
  }
  else
  {
    fprintf( stderr, "Cannot send %s event.\n", msg );
    return EXIT_FAILURE;
  }
}

void
window_set_title( Display * disp, Window win, char *title, char mode )
{
  gchar *title_utf8;
  gchar *title_local;

  if ( envir_utf8 )
  {
    title_utf8 = g_strdup( title );
    title_local = NULL;
  }
  else
  {
    if ( !( title_utf8 = g_locale_to_utf8( title, -1, NULL, NULL, NULL ) ) )
    {
      title_utf8 = g_strdup( title );
    }
    title_local = g_strdup( title );
  }

  if ( mode == 'T' || mode == 'N' )
  {
    /* set name */
    if ( title_local )
    {
      XChangeProperty( disp, win, XA_WM_NAME, XA_STRING, 8, PropModeReplace, ( unsigned char * ) title_local, strlen( title_local ) );
    }
    else
    {
      XDeleteProperty( disp, win, XA_WM_NAME );
    }
    XChangeProperty( disp, win, XInternAtom( disp, "_NET_WM_NAME", False ),
                     XInternAtom( disp, "UTF8_STRING", False ), 8, PropModeReplace, ( unsigned char * ) title_utf8, strlen( title_utf8 ) );
  }

  if ( mode == 'T' || mode == 'I' )
  {
    /* set icon name */
    if ( title_local )
    {
      XChangeProperty( disp, win, XA_WM_ICON_NAME, XA_STRING, 8, PropModeReplace, ( unsigned char * ) title_local, strlen( title_local ) );
    }
    else
    {
      XDeleteProperty( disp, win, XA_WM_ICON_NAME );
    }
    XChangeProperty( disp, win, XInternAtom( disp, "_NET_WM_ICON_NAME", False ),
                     XInternAtom( disp, "UTF8_STRING", False ), 8, PropModeReplace, ( unsigned char * ) title_utf8, strlen( title_utf8 ) );
  }

  g_free( title_utf8 );
  g_free( title_local );

}

int
window_to_desktop( Display * disp, Window win, int desktop )
{                               /*{{{ */
  unsigned long *cur_desktop = NULL;
  Window root = DefaultRootWindow( disp );

  if ( desktop == -1 )
  {
    if ( !( cur_desktop = ( unsigned long * ) get_property( disp, root, XA_CARDINAL, "_NET_CURRENT_DESKTOP", NULL ) ) )
    {
      if ( !( cur_desktop = ( unsigned long * ) get_property( disp, root, XA_CARDINAL, "_WIN_WORKSPACE", NULL ) ) )
      {
        fputs( "Cannot get current desktop properties. " "(_NET_CURRENT_DESKTOP or _WIN_WORKSPACE property)" "\n", stderr );
        return EXIT_FAILURE;
      }
    }
    desktop = *cur_desktop;
  }
  g_free( cur_desktop );

  return client_msg( disp, win, "_NET_WM_DESKTOP", ( unsigned long ) desktop, 0, 0, 0, 0 );
}

int
activate_window( Display * disp, Window win, gboolean switch_desktop )
{
  unsigned long *desktop;

  /* desktop ID */
  if ( ( desktop = ( unsigned long * ) get_property( disp, win, XA_CARDINAL, "_NET_WM_DESKTOP", NULL ) ) == NULL )
  {
    if ( ( desktop = ( unsigned long * ) get_property( disp, win, XA_CARDINAL, "_WIN_WORKSPACE", NULL ) ) == NULL )
    {
      p_verbose( "Cannot find desktop ID of the window.\n" );
    }
  }

  if ( switch_desktop && desktop )
  {
    if ( client_msg( disp, DefaultRootWindow( disp ), "_NET_CURRENT_DESKTOP", *desktop, 0, 0, 0, 0 ) != EXIT_SUCCESS )
    {
      p_verbose( "Cannot switch desktop.\n" );
    }
    g_free( desktop );
  }

  client_msg( disp, win, "_NET_ACTIVE_WINDOW", 0, 0, 0, 0, 0 );
  XMapRaised( disp, win );

  return EXIT_SUCCESS;
}

int
close_window( Display * disp, Window win )
{                               /*{{{ */
  return client_msg( disp, win, "_NET_CLOSE_WINDOW", 0, 0, 0, 0, 0 );
}

int
window_state( Display * disp, Window win, char *arg )
{                               /*{{{ */
  unsigned long action;
  Atom prop1 = 0;
  Atom prop2 = 0;
  char *p1, *p2;
  const char *argerr = "The -b option expects a list of comma separated parameters: \"(remove|add|toggle),<PROP1>[,<PROP2>]\"\n";

  if ( !arg || strlen( arg ) == 0 )
  {
    fputs( argerr, stderr );
    return EXIT_FAILURE;
  }

  if ( ( p1 = strchr( arg, ',' ) ) )
  {
    gchar *tmp_prop1, *tmp1;

    *p1 = '\0';

    /* action */
    if ( strcmp( arg, "remove" ) == 0 )
    {
      action = _NET_WM_STATE_REMOVE;
    }
    else if ( strcmp( arg, "add" ) == 0 )
    {
      action = _NET_WM_STATE_ADD;
    }
    else if ( strcmp( arg, "toggle" ) == 0 )
    {
      action = _NET_WM_STATE_TOGGLE;
    }
    else
    {
      fputs( "Invalid action. Use either remove, add or toggle.\n", stderr );
      return EXIT_FAILURE;
    }
    p1++;

    /* the second property */
    if ( ( p2 = strchr( p1, ',' ) ) )
    {
      gchar *tmp_prop2, *tmp2;

      *p2 = '\0';
      p2++;
      if ( strlen( p2 ) == 0 )
      {
        fputs( "Invalid zero length property.\n", stderr );
        return EXIT_FAILURE;
      }
      tmp_prop2 = g_strdup_printf( "_NET_WM_STATE_%s", tmp2 = g_ascii_strup( p2, -1 ) );
      p_verbose( "State 2: %s\n", tmp_prop2 );
      prop2 = XInternAtom( disp, tmp_prop2, False );
      g_free( tmp2 );
      g_free( tmp_prop2 );
    }

    /* the first property */
    if ( strlen( p1 ) == 0 )
    {
      fputs( "Invalid zero length property.\n", stderr );
      return EXIT_FAILURE;
    }
    tmp_prop1 = g_strdup_printf( "_NET_WM_STATE_%s", tmp1 = g_ascii_strup( p1, -1 ) );
    p_verbose( "State 1: %s\n", tmp_prop1 );
    prop1 = XInternAtom( disp, tmp_prop1, False );
    g_free( tmp1 );
    g_free( tmp_prop1 );


    return client_msg( disp, win, "_NET_WM_STATE", action, ( unsigned long ) prop1, ( unsigned long ) prop2, 0, 0 );
  }
  else
  {
    fputs( argerr, stderr );
    return EXIT_FAILURE;
  }
}

gboolean
wm_supports( Display * disp, const gchar * prop )
{                               /*{{{ */
  Atom xa_prop = XInternAtom( disp, prop, False );
  Atom *list;
  unsigned long size;
  int i;

  if ( !( list = ( Atom * ) get_property( disp, DefaultRootWindow( disp ), XA_ATOM, "_NET_SUPPORTED", &size ) ) )
  {
    p_verbose( "Cannot get _NET_SUPPORTED property.\n" );
    return FALSE;
  }

  for ( i = 0; i < size / sizeof( Atom ); i++ )
  {
    if ( list[i] == xa_prop )
    {
      g_free( list );
      return TRUE;
    }
  }

  g_free( list );
  return FALSE;
}

int
window_move_resize( Display * disp, Window win, char *arg )
{                               /*{{{ */
  signed long grav, x, y, w, h;
  unsigned long grflags;
  const char *argerr = "The -e option expects a list of comma separated integers: \"gravity,X,Y,width,height\"\n";

  if ( !arg || strlen( arg ) == 0 )
  {
    fputs( argerr, stderr );
    return EXIT_FAILURE;
  }

  if ( sscanf( arg, "%ld,%ld,%ld,%ld,%ld", &grav, &x, &y, &w, &h ) != 5 )
  {
    fputs( argerr, stderr );
    return EXIT_FAILURE;
  }

  if ( grav < 0 )
  {
    fputs( "Value of gravity mustn't be negative. Use zero to use the default gravity of the window.\n", stderr );
    return EXIT_FAILURE;
  }

  grflags = grav;
  if ( x != -1 )
    grflags |= ( 1 << 8 );
  if ( y != -1 )
    grflags |= ( 1 << 9 );
  if ( w != -1 )
    grflags |= ( 1 << 10 );
  if ( h != -1 )
    grflags |= ( 1 << 11 );

  p_verbose( "grflags: %lu\n", grflags );

  if ( wm_supports( disp, "_NET_MOVERESIZE_WINDOW" ) )
  {
    return client_msg( disp, win, "_NET_MOVERESIZE_WINDOW",
                       grflags, ( unsigned long ) x, ( unsigned long ) y, ( unsigned long ) w, ( unsigned long ) h );
  }
  else
  {
    p_verbose( "WM doesn't support _NET_MOVERESIZE_WINDOW. Gravity will be ignored.\n" );
    if ( ( w < 1 || h < 1 ) && ( x >= 0 && y >= 0 ) )
    {
      XMoveWindow( disp, win, x, y );
    }
    else if ( ( x < 0 || y < 0 ) && ( w >= 1 && h >= -1 ) )
    {
      XResizeWindow( disp, win, w, h );
    }
    else if ( x >= 0 && y >= 0 && w >= 1 && h >= 1 )
    {
      XMoveResizeWindow( disp, win, x, y, w, h );
    }
    return EXIT_SUCCESS;
  }
}

int
action_window( Display * disp, Window win, char mode )
{                               /*{{{ */
  p_verbose( "Using window: 0x%.8lx\n", win );
  switch ( mode )
  {
  case 'a':
    return activate_window( disp, win, TRUE );

  case 'c':
    return close_window( disp, win );

  case 'e':
    /* resize/move the window around the desktop => -r -e */
    return window_move_resize( disp, win, options.param );

  case 'b':
    /* change state of a window => -r -b */
    return window_state( disp, win, options.param );

  case 't':
    /* move the window to the specified desktop => -r -t */
    return window_to_desktop( disp, win, atoi( options.param ) );

  case 'R':
    /* move the window to the current desktop and activate it => -r */
    if ( window_to_desktop( disp, win, -1 ) == EXIT_SUCCESS )
    {
      usleep( 100000 );         /* 100 ms - make sure the WM has enough
                                   time to move the window, before we activate it */
      return activate_window( disp, win, FALSE );
    }
    else
    {
      return EXIT_FAILURE;
    }

  case 'N':
  case 'I':
  case 'T':
    window_set_title( disp, win, options.param, mode );
    return EXIT_SUCCESS;

  default:
    fprintf( stderr, "Unknown action: '%c'\n", mode );
    return EXIT_FAILURE;
  }
}

int
action_window_str( Display * disp, char mode )
{                               /*{{{ */
  Window activate = 0;
  Window *client_list;
  unsigned long client_list_size;
  int i;

  /* if ( strcmp( SELECT_WINDOW_MAGIC, options.param_window ) == 0 ) */
  /* {                                                               */
  /*   activate = Select_Window( disp );                             */
  /*   if ( activate )                                               */
  /*   {                                                             */
  /*     return action_window( disp, activate, mode );               */
  /*   }                                                             */
  /*   else                                                          */
  /*   {                                                             */
  /*     return EXIT_FAILURE;                                        */
  /*   }                                                             */
  /* }                                                               */
  /* if ( strcmp( ACTIVE_WINDOW_MAGIC, options.param_window ) == 0 ) */
  /* {                                                               */
  /*   activate = get_active_window( disp );                         */
  /*   if ( activate )                                               */
  /*   {                                                             */
  /*     return action_window( disp, activate, mode );               */
  /*   }                                                             */
  /*   else                                                          */
  /*   {                                                             */
  /*     return EXIT_FAILURE;                                        */
  /*   }                                                             */
  /* }                                                               */
  /* else                                                            */
  {
    if ( ( client_list = get_client_list( disp, &client_list_size ) ) == NULL )
    {
      return EXIT_FAILURE;
    }

    for ( i = 0; i < client_list_size / sizeof( Window ); i++ )
    {
      gchar *match_utf8;

      if ( options.show_class )
      {
        match_utf8 = get_window_class( disp, client_list[i] ); /* UTF8 */
      }
      else
      {
        match_utf8 = get_window_title( disp, client_list[i] ); /* UTF8 */
      }
      if ( match_utf8 )
      {
        gchar *match;
        gchar *match_cf;
        gchar *match_utf8_cf = NULL;

        if ( envir_utf8 )
        {
          match = g_strdup( options.param_window );
          match_cf = g_utf8_casefold( options.param_window, -1 );
        }
        else
        {
          if ( !( match = g_locale_to_utf8( options.param_window, -1, NULL, NULL, NULL ) ) )
          {
            match = g_strdup( options.param_window );
          }
          match_cf = g_utf8_casefold( match, -1 );
        }

        if ( !match || !match_cf )
        {
          continue;
        }

        match_utf8_cf = g_utf8_casefold( match_utf8, -1 );

        if ( ( options.full_window_title_match && strcmp( match_utf8, match ) == 0 ) ||
             ( !options.full_window_title_match && strstr( match_utf8_cf, match_cf ) ) )
        {
          activate = client_list[i];
          g_free( match );
          g_free( match_cf );
          g_free( match_utf8 );
          g_free( match_utf8_cf );
          break;
        }
        g_free( match );
        g_free( match_cf );
        g_free( match_utf8 );
        g_free( match_utf8_cf );
      }
    }
    g_free( client_list );

    if ( activate )
    {
      return action_window( disp, activate, mode );
    }
    else
    {
      return EXIT_FAILURE;
    }
  }
}

Window *
get_client_list( Display * disp, unsigned long *size )
{                               /*{{{ */
  Window *client_list;

  if ( ( client_list = ( Window * ) get_property( disp, DefaultRootWindow( disp ), XA_WINDOW, "_NET_CLIENT_LIST", size ) ) == NULL )
  {
    if ( ( client_list = ( Window * ) get_property( disp, DefaultRootWindow( disp ), XA_CARDINAL, "_WIN_CLIENT_LIST", size ) ) == NULL )
    {
      fputs( "Cannot get client list properties. \n" "(_NET_CLIENT_LIST or _WIN_CLIENT_LIST)" "\n", stderr );
      return NULL;
    }
  }

  return client_list;
}

gchar *
get_window_class( Display * disp, Window win )
{                               /*{{{ */
  gchar *class_utf8;
  gchar *wm_class;
  unsigned long size;

  wm_class = get_property( disp, win, XA_STRING, "WM_CLASS", &size );
  if ( wm_class )
  {
    gchar *p_0 = strchr( wm_class, '\0' );

    if ( wm_class + size - 1 > p_0 )
    {
      *( p_0 ) = '.';
    }
    class_utf8 = g_locale_to_utf8( wm_class, -1, NULL, NULL, NULL );
  }
  else
  {
    class_utf8 = NULL;
  }

  g_free( wm_class );

  return class_utf8;
}

gchar *
get_window_title( Display * disp, Window win )
{                               /*{{{ */
  gchar *title_utf8;
  gchar *wm_name;
  gchar *net_wm_name;

  wm_name = get_property( disp, win, XA_STRING, "WM_NAME", NULL );
  net_wm_name = get_property( disp, win, XInternAtom( disp, "UTF8_STRING", False ), "_NET_WM_NAME", NULL );

  if ( net_wm_name )
  {
    title_utf8 = g_strdup( net_wm_name );
  }
  else
  {
    if ( wm_name )
    {
      title_utf8 = g_locale_to_utf8( wm_name, -1, NULL, NULL, NULL );
    }
    else
    {
      title_utf8 = NULL;
    }
  }

  g_free( wm_name );
  g_free( net_wm_name );

  return title_utf8;
}

gchar *
get_property( Display * disp, Window win, /*{{{ */
              Atom xa_prop_type, gchar * prop_name, unsigned long *size )
{
  Atom xa_prop_name;
  Atom xa_ret_type;
  int ret_format;
  unsigned long ret_nitems;
  unsigned long ret_bytes_after;
  unsigned long tmp_size;
  unsigned char *ret_prop;
  gchar *ret;

  xa_prop_name = XInternAtom( disp, prop_name, False );

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
  if ( XGetWindowProperty( disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
                           xa_prop_type, &xa_ret_type, &ret_format, &ret_nitems, &ret_bytes_after, &ret_prop ) != Success )
  {
    p_verbose( "Cannot get %s property.\n", prop_name );
    return NULL;
  }

  if ( xa_ret_type != xa_prop_type )
  {
    p_verbose( "Invalid type of %s property.\n", prop_name );
    XFree( ret_prop );
    return NULL;
  }

  /* null terminate the result to make string handling easier */
  tmp_size = ( ret_format / 8 ) * ret_nitems;
  /* Correct 64 Architecture implementation of 32 bit data */
  if ( ret_format == 32 )
    tmp_size *= sizeof( long ) / 4;
  ret = g_malloc( tmp_size + 1 );
  memcpy( ret, ret_prop, tmp_size );
  ret[tmp_size] = '\0';

  if ( size )
  {
    *size = tmp_size;
  }

  XFree( ret_prop );
  return ret;
}

Window
Select_Window( Display * dpy )
{                               /*{{{ */
  /*
   * Routine to let user select a window using the mouse
   * Taken from xfree86.
   */

  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = DefaultRootWindow( dpy );
  int buttons = 0;
  int dummyi;
  unsigned int dummy;

  /* Make the target cursor */
  cursor = XCreateFontCursor( dpy, XC_crosshair );

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer( dpy, root, False, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, root, cursor, CurrentTime );
  if ( status != GrabSuccess )
  {
    fputs( "ERROR: Cannot grab mouse.\n", stderr );
    return 0;
  }

  /* Let the user select a window... */
  while ( ( target_win == None ) || ( buttons != 0 ) )
  {
    /* allow one more event */
    XAllowEvents( dpy, SyncPointer, CurrentTime );
    XWindowEvent( dpy, root, ButtonPressMask | ButtonReleaseMask, &event );
    switch ( event.type )
    {
    case ButtonPress:
      if ( target_win == None )
      {
        target_win = event.xbutton.subwindow; /* window selected */
        if ( target_win == None )
          target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if ( buttons > 0 )        /* there may have been some down before we started */
        buttons--;
      break;
    }
  }

  XUngrabPointer( dpy, CurrentTime ); /* Done with pointer */

  if ( XGetGeometry( dpy, target_win, &root, &dummyi, &dummyi, &dummy, &dummy, &dummy, &dummy ) && target_win != root )
  {
    target_win = XmuClientWindow( dpy, target_win );
  }

  return ( target_win );
}

Window
get_active_window( Display * disp )
{                               /*{{{ */
  char *prop;
  unsigned long size;
  Window ret = ( Window ) 0;

  prop = get_property( disp, DefaultRootWindow( disp ), XA_WINDOW, "_NET_ACTIVE_WINDOW", &size );
  if ( prop )
  {
    ret = *( ( Window * ) prop );
    g_free( prop );
  }

  return ( ret );
}
