#include <string.h>

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */
#include <mastar/multiconfig/muc_option_types.h>

/* man getopt_long */

/* ###################################################################### */
#include "duf_xtended_table.h"
/* ###################################################################### */

extern const muc_longval_extended_table_t optable_main;
extern const muc_longval_extended_table_t optable_io;
extern const muc_longval_extended_table_t optable_experimental;
extern const muc_longval_extended_table_t optable_fs;
extern const muc_longval_extended_table_t optable_interactive;
extern const muc_longval_extended_table_t optable_str;
extern const muc_longval_extended_table_t optable_options;
extern const muc_longval_extended_table_t optable_db;
extern const muc_longval_extended_table_t optable_sccb;
extern const muc_longval_extended_table_t optable_info;
extern const muc_longval_extended_table_t optable_flag;
extern const muc_longval_extended_table_t optable_flag_allow;
extern const muc_longval_extended_table_t optable_sflag;
extern const muc_longval_extended_table_t optable_uplus;
extern const muc_longval_extended_table_t optable_help;

#ifdef MAS_TRACING_OPTIONS
extern const muc_longval_extended_table_t optable_trace;
#endif
extern const muc_longval_extended_table_t optable_format;
extern const muc_longval_extended_table_t optable_filter;
extern const muc_longval_extended_table_t optable_test;
extern const muc_longval_extended_table_t optable_debug;
extern const muc_longval_extended_table_t optable_redo_and_obsolete;

static const muc_longval_extended_table_t *const __lo_extended_table_multi[] = {
  &optable_main,
  &optable_experimental,
  &optable_fs,
  &optable_io,
  &optable_interactive,
  &optable_str,
  &optable_options,
  &optable_db,
  &optable_sccb,
  &optable_info,
  &optable_sflag,
  &optable_flag,
  &optable_flag_allow,
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

const muc_longval_extended_table_t *const *
duf_xtable_list( void )
{
  return __lo_extended_table_multi;
}
