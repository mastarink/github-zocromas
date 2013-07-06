#ifndef MAS_THREAD_VARIABLES_H
#  define MAS_THREAD_VARIABLES_H

#  include <mastar/types/mas_thread_variables_types.h>


int mas_thread_variable_set_text( th_type_t thtype, const char *vclass, const char *name, const char *txt );

mas_varvec_element_t *mas_thread_variables_find( const char *vclass, const char *name );

int _mas_thread_variables_channel_write( const char *vclass, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel );

int mas_thread_variables_delete( void );

#endif
