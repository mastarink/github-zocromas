#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H

/*
 * TODO rename
 * */

/* codeval + std.xtables => extended; codeval2extended_by_multitable */
const duf_longval_extended_t *duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_table_t ** result_pxtable, int *pr );

/* name + extended_array => extended; name2extended_by_xarr */
const duf_longval_extended_t *duf_noption_xfind_no_at_xarr( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno,
                                                            int *pcnt, int *pr );

/* + extended_array => extended; string2extended */
const duf_longval_extended_t *duf_soption_xfind_at_xarr( const char *string, const duf_longval_extended_t * xtended, char vseparator, char **parg,
                                                         int *pno, int *pcnt, int *pr );

#endif
