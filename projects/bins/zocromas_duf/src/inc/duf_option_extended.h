#ifndef MAS_DUF_OPTION_EXTENDED_H
#  define MAS_DUF_OPTION_EXTENDED_H

#  include "duf_option_types.h"



duf_option_t *duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable );
void duf_options_delete_longopts_table( duf_option_t * longopts );

const duf_longval_extended_t *duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pno );

/* const char *duf_longindex_extended_name( int longindex, int *pr ); */
/* duf_option_code_t duf_longindex_extended_codeval( int longindex, int *pr ); */

int duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables );

const char *duf_extended_code2string( duf_option_code_t code );
const char *duf_extended_class2string( duf_option_class_t oclass );
const char *duf_extended_vtype2string( duf_option_vtype_t ovtype );

void duf_extended_table_print( const duf_longval_extended_table_t * xtable, const char *name );
void duf_extended_table_multi_print( const duf_longval_extended_table_t ** xtables, const char *name );
void duf_extended_table_multi_std_print( const char *name );


#endif
