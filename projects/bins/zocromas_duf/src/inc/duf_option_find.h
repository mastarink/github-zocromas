#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H

/*
 * TODO rename
 * duf_find_codeval_extended_std : codeval + std.xtables => extended; duf_coption_xfind_x_at_std
 * duf_find_name_long_no         : name + extended_array => extended; duf_noption_xfind_no_x_at_array
 * */
const duf_longval_extended_t *duf_coption_xfind_x_at_std( duf_option_code_t codeval, const duf_longval_extended_table_t ** pxtable, int *pr );
const duf_longval_extended_t *duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pcnt,
                                                     int *pr );

#endif
