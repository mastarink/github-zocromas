#ifndef MAS_DUF_OPTION_TYPED_H
#  define MAS_DUF_OPTION_TYPED_H



int duf_clarify_xcmd_typed( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source );
int duf_clarify_xcmd_typed_call( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                 const duf_longval_extended_table_t * xtable, int noo, duf_option_source_t source );
int duf_clarify_xcmd_typed_gen( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source );

#endif
