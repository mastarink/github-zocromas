#ifndef MAS_RUNONCE_WINDOW_H
#  define MAS_RUNONCE_WINDOW_H

gboolean wm_supports( Display * disp, const gchar * prop );
Window *get_client_list( Display * disp, unsigned long *size );
int client_msg( Display * disp, Window win, char *msg,
                unsigned long data0, unsigned long data1, unsigned long data2, unsigned long data3, unsigned long data4 );
int action_window( Display * disp, Window win, char mode );
int action_window_str( Display * disp, char mode );
int activate_window( Display * disp, Window win, gboolean switch_desktop );
int close_window( Display * disp, Window win );
int window_to_desktop( Display * disp, Window win, int desktop );
void window_set_title( Display * disp, Window win, char *str, char mode );
gchar *get_window_title( Display * disp, Window win );
gchar *get_window_class( Display * disp, Window win );
gchar *get_property( Display * disp, Window win, Atom xa_prop_type, gchar * prop_name, unsigned long *size );
int window_move_resize( Display * disp, Window win, char *arg );
int window_state( Display * disp, Window win, char *arg );
Window Select_Window( Display * dpy );
Window get_active_window( Display * dpy );



#endif
