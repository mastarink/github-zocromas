#ifndef MAS_DUF_OPTION_EXTENDED2STRING_H
#  define MAS_DUF_OPTION_EXTENDED2STRING_H

#  include "duf_option_types.h"





/* const char *duf_longindex_extended_name( int longindex, int *pr ); */
/* duf_option_code_t duf_longindex_extended_codeval( int longindex, int *pr ); */


const char *duf_extended_vtype2string( duf_option_vtype_t ovtype );
const char *duf_extended_call_type2string( duf_option_call_type_t call_type );

void duf_stdx_print( const char *name );


#endif

