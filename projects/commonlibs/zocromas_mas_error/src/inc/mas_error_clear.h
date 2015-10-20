#ifndef MAS_ERROR_CLEAR_H
#  define MAS_ERROR_CLEAR_H

#  include <stdarg.h>

#  include "mas_error_types.h"


mas_error_code_t mas_vclear_error_c( mas_error_code_t r, va_list args );
mas_error_code_t mas_clear_error_c( mas_error_code_t r, ... );

mas_error_index_t mas_clear_error_i( mas_error_index_t e, ... );

#endif
