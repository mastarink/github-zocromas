#ifndef MAS_DUF_OPTION_DESCR_H
#  define MAS_DUF_OPTION_DESCR_H


char *duf_option_description_d( int longindex, const char *delimh, const char *delim );
char *duf_option_description_xd( const duf_longval_extended_t * extended, const char *delimh, const char *delim );

const char *duf_option_description_x_tmp( int index, const duf_longval_extended_t * extended );
const char *duf_find_longval_help( duf_option_code_t code, int *pr );



#endif
