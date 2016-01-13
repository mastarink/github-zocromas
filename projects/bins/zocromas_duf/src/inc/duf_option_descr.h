#ifndef MAS_DUF_OPTION_DESCR_H
#  define MAS_DUF_OPTION_DESCR_H

#include "duf_options_enum.h" /* duf_option_code_t :: TODO duf_option_code_t => duf_option_code_general_t i.e. `unsigned` */

char *duf_loption_description_d( int longindex, const char *delimh, const char *delim );
char *duf_xoption_description_d( const duf_longval_extended_t * extended, const char *delimh, const char *delim );

const char *duf_xoption_description_tmp( int tmp_index, const duf_longval_extended_t * extended );
const char *duf_coption_xfind_desc_at_stdx( duf_option_code_t codeval, int *pr );



#endif
