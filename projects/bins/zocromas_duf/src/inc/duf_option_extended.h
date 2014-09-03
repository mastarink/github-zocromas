#ifndef MAS_DUF_OPTION_EXTENDED_H
#  define MAS_DUF_OPTION_EXTENDED_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"


extern const duf_longval_extended_t *lo_extended;
extern unsigned lo_extended_count;

/* const duf_longval_extended_t *_duf_find_longval_extended( duf_option_code_t code ); */
const duf_longval_extended_t *duf_longindex_extended( int longindex );
const char *duf_longindex_extended_name( int longindex );
duf_option_code_t duf_longindex_extended_codeval( int longindex );

#endif

