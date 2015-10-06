#ifndef MAS_DUF_ERRORS_H
#  define MAS_DUF_ERRORS_H

#  include <stdarg.h>

#  include "duf_error_types.h"


duf_error_code_t duf_vclear_error_c( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error_c( duf_error_code_t r, ... );

duf_error_index_t duf_clear_error_i( duf_error_index_t e, ... );

#  include "duf_error_base.h"
#  include "duf_error_reporting.h"
#  include "duf_error_regfind.h"

#endif
