#ifndef DUF_OPTION_H
#  define DUF_OPTION_H


int duf_parse_exec_option( duf_option_code_t opt, int longindex, const char *optarg, duf_option_stage_t stage, duf_option_source_t source )
      __attribute__ ( ( warn_unused_result ) );

int DUF_WRAPPED( duf_clarify_xcmd_full ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t stage,
                                           const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
      __attribute__ ( ( warn_unused_result ) );

#endif
