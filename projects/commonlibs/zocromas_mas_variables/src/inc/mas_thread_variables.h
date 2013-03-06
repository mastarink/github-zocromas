#ifndef MAS_THREAD_VARIABLES_H
#  define MAS_THREAD_VARIABLES_H

#  include <mastar/types/mas_thread_variables_types.h>


int mas_thread_variable_create_x( th_type_t thtype, const char *vclass, const char *name, mas_xvsnprintf_t func, const char *fmt,
                                  ... );
int mas_thread_variable_create_text( th_type_t thtype, const char *vclass, const char *name, const char *txt );
int mas_thread_variable_set_text( th_type_t thtype, const char *vclass, const char *name, const char *txt );

mas_variable_t *mas_thread_variables_find( const char *vclass, const char *name );

/* int mas_thread_variables_msg( const char *vclass ); */

int _mas_thread_variables_channel_write( const char *vclass, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel );
int mas_thread_variables_channel_write( const char *vclass, const char *fmt, mas_channel_t * pchannel );

int mas_thread_variables_delete( void );

#endif
