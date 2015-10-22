/* ###################################################################### */
#include "mas_error_names.h"
/* ###################################################################### */

#define MASE_ERROR_NAME(_n) {.name=#_n, .code=_n}

const mas_errdesc_t _error_descriptions_table[] = {
#define ENUM_WRAP(_n)         MASE_ERROR_NAME(_n),
#define ENUM_WRAP_V(_n, _v)   MASE_ERROR_NAME(_n),
#include "mas_error_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
};

const mas_errdesc_t *mas_error_descriptions_table = _error_descriptions_table;
int mas_error_descriptions_table_size = sizeof( _error_descriptions_table ) / sizeof( _error_descriptions_table[0] );
