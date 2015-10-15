#ifndef MAS_DUF_OPTION_FIND_H
#  define MAS_DUF_OPTION_FIND_H


const duf_longval_extended_t *duf_find_codeval_extended_std( duf_option_code_t codeval, const duf_longval_extended_table_t ** pxtable, int *pr );
const duf_longval_extended_t *duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno,
                                                     int *pr );
int duf_check_stage( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
      __attribute__ ( ( warn_unused_result ) );

#endif
