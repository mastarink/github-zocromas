#ifndef MAS_DUF_ERRORS_H
#  define MAS_DUF_ERRORS_H

#  include "duf_error_types.h"

duf_error_code_t duf_vclear_error( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error( duf_error_code_t r, ... );

const char *duf_error_name( duf_error_code_t c );


#endif
