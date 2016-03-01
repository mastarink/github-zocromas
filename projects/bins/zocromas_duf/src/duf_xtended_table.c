#include <string.h>

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

/* man getopt_long */

/* ###################################################################### */
#include "duf_xtended_table.h"
/* ###################################################################### */

extern const duf_longval_extended_table_t optable_main;
extern const duf_longval_extended_table_t optable_io;
extern const duf_longval_extended_table_t optable_experimental;
extern const duf_longval_extended_table_t optable_fs;
extern const duf_longval_extended_table_t optable_interactive;
extern const duf_longval_extended_table_t optable_str;
extern const duf_longval_extended_table_t optable_db;
extern const duf_longval_extended_table_t optable_sccb;
extern const duf_longval_extended_table_t optable_info;
extern const duf_longval_extended_table_t optable_flag;
extern const duf_longval_extended_table_t optable_sflag;
extern const duf_longval_extended_table_t optable_uplus;
extern const duf_longval_extended_table_t optable_help;

#ifdef MAS_TRACING_OPTIONS
extern const duf_longval_extended_table_t optable_trace;
#endif
extern const duf_longval_extended_table_t optable_format;
extern const duf_longval_extended_table_t optable_filter;
extern const duf_longval_extended_table_t optable_test;
extern const duf_longval_extended_table_t optable_debug;
extern const duf_longval_extended_table_t optable_redo_and_obsolete;

static const duf_longval_extended_table_t *const __lo_extended_table_multi[] = {
  &optable_main,
  &optable_experimental,
  &optable_fs,
  &optable_io,
  &optable_interactive,
  &optable_str,
  &optable_db,
  &optable_sccb,
  &optable_info,
  &optable_sflag,
  &optable_flag,
  &optable_uplus,
#ifdef MAS_TRACING_OPTIONS
  &optable_trace,
#endif
  &optable_filter,
  &optable_test,
  &optable_debug,
  &optable_help,
  &optable_format,
  &optable_redo_and_obsolete,

  NULL
};

const duf_longval_extended_table_t *const *
duf_xtable_list( void )
{
  return __lo_extended_table_multi;
}
