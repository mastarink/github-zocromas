#ifndef MAS_DUF_ERROR_CLEAR_H
#  define MAS_DUF_ERROR_CLEAR_H

#  include <stdarg.h>

#  include "duf_error_types.h"


duf_error_code_t duf_vclear_error_c( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error_c( duf_error_code_t r, ... );

duf_error_index_t duf_clear_error_i( duf_error_index_t e, ... );

#endif
