#ifndef MAS_DUF_OPTION_CFIND_H
#  define MAS_DUF_OPTION_CFIND_H

/*
 * TODO rename
 * */
#  include "duf_options_enum.h" /* duf_option_code_t */

/* codeval + std.xtables => extended; codeval2extended_by_multitable */
const duf_longval_extended_t *duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_table_t ** result_pxtable, int *pr );
const char *duf_coption_find_name_at_std( duf_option_code_t codeval, int *pr );

const char *duf_lcoption_find_name_at_std( duf_option_code_t codeval, int longindex, int *pr );


#endif
