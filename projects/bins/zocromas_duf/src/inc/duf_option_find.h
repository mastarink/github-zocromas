#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H

/*
 * TODO rename
 * */
#include "duf_options_enum.h" /* duf_option_code_t */ /* TODO duf_option_code_t => duf_option_code_general_t i.e. `unsigned` */

/* name + extended_array => extended; name2extended_by_xarr */
const duf_longval_extended_t *duf_noption_xfind_no_at_xarr( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno,
                                                            int *pcnt, int *pr );

/* + extended_array => extended; string2extended */
const duf_longval_extended_t *duf_soption_xfind_at_xarr( const char *string, const duf_longval_extended_t * xtended, char vseparator, char **parg,
                                                         int *pno, int *pcnt, int *pr );

#endif
