#ifndef MAS_DUF_OPTION_CFIND_H
#  define MAS_DUF_OPTION_CFIND_H

/*
 * TODO rename
 * */
#include "duf_options_enum.h" /* duf_option_code_t */ /* TODO duf_option_code_t => duf_option_code_general_t i.e. `unsigned` */

/* codeval + std.xtables => extended; codeval2extended_by_multitable */
const duf_longval_extended_t *duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_table_t ** result_pxtable, int *pr );


#endif

