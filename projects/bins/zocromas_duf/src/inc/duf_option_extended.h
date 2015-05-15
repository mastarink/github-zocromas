#ifndef MAS_DUF_OPTION_EXTENDED_H
#  define MAS_DUF_OPTION_EXTENDED_H

#  include "duf_option_types.h"
#  include "duf_options_enum.h"


extern const duf_longval_extended_t **lo_extended_multi;

const duf_longval_extended_t *duf_longindex2extended( int longindex, int *pr );
/* const char *duf_longindex_extended_name( int longindex, int *pr ); */
/* duf_option_code_t duf_longindex_extended_codeval( int longindex, int *pr ); */

int duf_longindex_extended_count( const duf_longval_extended_t ** xtables );

#endif
