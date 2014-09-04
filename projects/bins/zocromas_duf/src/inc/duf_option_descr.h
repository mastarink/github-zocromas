#ifndef MAS_DUF_OPTION_DESCR_H
#  define MAS_DUF_OPTION_DESCR_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

/* char *duf_option_description( int longindex, int *pr );                                                                     */
char *duf_option_description_d( int longindex, const char *delimh, const char *delim, int *pr );
/* char *duf_option_description_xd( const duf_longval_extended_t * extended, const char *delimh, const char *delim, int *pr ); */
/* char *duf_option_description_x( const duf_longval_extended_t * extended, int *pr );                                         */
const char *duf_option_description_x_tmp( int index, const duf_longval_extended_t * extended, int *pr );

const char *duf_find_longval_help( duf_option_code_t code, int *pr );



#endif
