#ifndef DUF_OPTION_H
#  define DUF_OPTION_H


int duf_parse_option( duf_option_code_t opt, int longindex, const char *optarg, duf_option_stage_t stage );

int DUF_WRAPPED( duf_clarify_opt_full ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t stage,
                                                const duf_longval_extended_table_t * xtable, int no );

#endif
