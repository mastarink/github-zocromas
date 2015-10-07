#ifndef MAS_DUF_ERROR_REGFIND_H
#  define MAS_DUF_ERROR_REGFIND_H

#  include <stdarg.h>

#  include "duf_error_types.h"


duf_error_index_t duf_register_error_c( duf_error_code_t code, const char *funcid, int linid, const char *fmt, ... );
duf_error_index_t duf_vregister_error_c( duf_error_code_t code, const char *funcid, int linid, const char *fmt, va_list args );

duf_error_event_t *duf_find_error_event_i( duf_error_index_t ri );
duf_error_event_t *duf_find_error_event_p( size_t rp );
size_t duf_error_list_size( void );
void duf_delete_error_rev( duf_error_event_t * rev );
void duf_delete_error_i( duf_error_index_t ri );


#endif
