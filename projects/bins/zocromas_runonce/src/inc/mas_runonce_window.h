#ifndef MAS_RUNONCE_WINDOW_H
#  define MAS_RUNONCE_WINDOW_H

#  include <X11/Xlib.h>

void mas_runonce_display_create( void );
void mas_runonce_display_delete( void );

Window *get_client_list( unsigned long *size );
int client_msg( Window win, char *msg, unsigned long data0, unsigned long data1, unsigned long data2, unsigned long data3,
                unsigned long data4 );
int action_window_str( char mode );
char *get_window_title( Window win );
char *get_window_class( Window win );
char *get_window_role( Window win );
char *get_window_icon_name( Window win );
char *get_property( Window win, Atom xa_prop_type, char *prop_name, unsigned long *size );

pid_t get_window_pid( Window win );
int get_window_desktop_num( Window win );
int mas_runonce_close_window( unsigned long win, runonce_flags_t flags );

void runonce_wids_set( void );

#endif
