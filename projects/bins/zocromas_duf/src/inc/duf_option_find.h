#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H


const duf_longval_extended_t *duf_find_codeval_extended_std( duf_option_code_t codeval, const duf_longval_extended_table_t ** pxtable, int *pr );
const duf_longval_extended_t *duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pcnt,
                                                     int *pr );

#endif
