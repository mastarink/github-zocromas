#ifndef MAS_ERROR_REGFIND_H
#  define MAS_ERROR_REGFIND_H

#  include <stdarg.h>
#  include <unistd.h>

#  include "mas_error_types.h"


mas_error_index_t mas_register_error_c( mas_error_code_t code, const char *funcid, int linid, const char *fmt, ... );
mas_error_index_t mas_vregister_error_c( mas_error_code_t code, const char *funcid, int linid, const char *fmt, va_list args );

mas_error_event_t *mas_find_error_event_i( mas_error_index_t ri );
mas_error_event_t *mas_find_error_event_p( size_t rp );
size_t mas_error_list_size( void );
void mas_delete_error_rev( mas_error_event_t * rev );
void mas_delete_error_i( mas_error_index_t ri );


#endif
