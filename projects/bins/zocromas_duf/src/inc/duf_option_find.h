#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_find_long( duf_option_code_t code, int *pr );
const duf_longval_extended_t *duf_find_extended( duf_option_code_t codeval, int *pr );

#endif

