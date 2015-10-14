#ifndef MAS_DUF_OPTION_EXTENDED_H
#  define MAS_DUF_OPTION_EXTENDED_H

#  include "duf_option_types.h"



duf_option_t *duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable );
void duf_options_delete_longopts_table( duf_option_t * longopts );

const duf_longval_extended_t *duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pno );

/* const char *duf_longindex_extended_name( int longindex, int *pr ); */
/* duf_option_code_t duf_longindex_extended_codeval( int longindex, int *pr ); */

int duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables );

#endif
