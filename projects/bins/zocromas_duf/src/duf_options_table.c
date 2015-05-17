#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

#include "duf_config_types.h"
#include "duf_option_types.h"
#include "duf_option_defs.h"
#include "duf_cli_types.h"

#include "duf_option_extended.h"
#include "duf_option_help.h"
#include "duf_options.h"

/* man getopt_long */

extern duf_scan_callbacks_t duf_integrity_callbacks __attribute( ( weak ) ),
      duf_directories_callbacks __attribute( ( weak ) ),
      duf_filedata_callbacks __attribute( ( weak ) ),
      duf_filenames_callbacks __attribute( ( weak ) ),
      duf_collect_openat_crc32_callbacks __attribute( ( weak ) ),
      duf_collect_openat_sd5_callbacks __attribute( ( weak ) ),
      duf_collect_openat_md5_callbacks __attribute( ( weak ) ),
      duf_collect_mime_callbacks __attribute( ( weak ) ), duf_collect_exif_callbacks __attribute( ( weak ) ),
      /* duf_collect_mdpath_callbacks __attribute( ( weak ) ), */
      /* duf_print_md5_callbacks __attribute( ( weak ) ),      */
 
      duf_print_tree_callbacks __attribute( ( weak ) ),
      duf_print_dir_callbacks __attribute( ( weak ) ),
      duf_bubububububububububububububububububububububububububububububububububububu __attribute( ( weak ) );



static const duf_longval_extended_t _lo_extended1[] = {
/* #include "duf_optable.cdef"                   */
/* #include "duf_optable_str.cdef"               */
/* #include "duf_optable_sflag.cdef"             */
/* #include "duf_optable_uplus.cdef"             */
/* #include "duf_optable_help.cdef"              */
/* #include "duf_optable_trace.cdef"             */
/* #include "duf_optable_filter.cdef"            */
/* #include "duf_optable_debug.cdef"             */
/* #include "duf_optable_redo_and_obsolete.cdef" */
  {0}
};

/* static const duf_longval_extended_t _lo_extended2[] = { */
/* (* #include "duf_optable_flag.cdef" *)                  */
/*   {0}                                                   */
/* };                                                      */
extern const duf_longval_extended_table_t optable_main;
extern const duf_longval_extended_table_t optable_interactive;
extern const duf_longval_extended_table_t optable_str;
extern const duf_longval_extended_table_t optable_sccb;
extern const duf_longval_extended_table_t optable_info;
extern const duf_longval_extended_table_t optable_flag;
extern const duf_longval_extended_table_t optable_sflag;
extern const duf_longval_extended_table_t optable_uplus;
extern const duf_longval_extended_table_t optable_help;
extern const duf_longval_extended_table_t optable_trace;
extern const duf_longval_extended_table_t optable_filter;
extern const duf_longval_extended_table_t optable_debug;
extern const duf_longval_extended_table_t optable_redo_and_obsolete;

static const duf_longval_extended_table_t *_lo_extended_table_multi[] = {
  &optable_main,
  &optable_interactive,
  &optable_str,
  &optable_sccb,
  &optable_info,
  &optable_sflag,
  &optable_flag,
  &optable_uplus,
  &optable_trace,
  &optable_filter,
  &optable_debug,
  &optable_help,
  &optable_redo_and_obsolete,

  NULL
};



const duf_longval_extended_table_t **lo_extended_table_multi = _lo_extended_table_multi;

/* unsigned lo_extended_count = sizeof( _lo_extended ) / sizeof( _lo_extended[0] ); */

/* static const duf_option_t duf_longopts_table[] = {                                                                                             */
/* (*{.name = "add-to-group",                      .has_arg = no_argument,                         .val = DUF_OPTION_ADD_TO_GROUP}, *)            */
/*   {.name = "dry-run",.has_arg = no_argument,.val = DUF_OPTION_DRY_RUN_TRACE},                                                                  */
/*   {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},                                                       */
/* (*{.name = "group",                             .has_arg = required_argument,                           .val = DUF_OPTION_GROUP}, *)           */
/*   {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},                                                       */
/* (*{.name = "limit",                             .has_arg = required_argument,                           .val = DUF_OPTION_LIMIT}, *)           */
/*   {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},                                                        */
/*   {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},                                                         */
/*   {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},                                                           */
/*   {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},                                                         */
/*   {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},                                                               */
/*   {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},                                                                 */
/*   {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MDPATH},                                                                     */
/*   {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},                                                                      */
/*   {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},                                                        */
/*   {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},                                                         */
/*   {.name = "output-level",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT_LEVEL},                                                        */
/* (*{.name = "print-dirs",                        .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DIRS},   *)            */
/* (*{.name = "print-duplicates",                  .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DUPLICATES}, *)        */
/* (*{.name = "print-files",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_FILES}, *)             */
/* (*{.name = "print-paths",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_PATHS}, *)             */
/*   {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_FLAG_REMOVE_DATABASE},                                                   */
/* (*{.name = "remove-from-group",                 .has_arg = no_argument,                         .val = DUF_OPTION_REMOVE_FROM_GROUP}, *)       */
/* (*{.name = "same-exif",                         .has_arg = no_argument,                         .val = DUF_OPTION_SAME_EXIF},   *)             */
/* (*{.name = "same-files",                        .has_arg = no_argument,                         .val = DUF_OPTION_SAME_FILES}, *)              */
/*   {.name = "same",.has_arg = required_argument,.val = DUF_OPTION_SAME},                                                                        */
/* (*{.name = "same-md5",                          .has_arg = no_argument,                         .val = DUF_OPTION_SAME_MD5},     *)            */
/*   {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},                                                                    */
/*   {.name = "sampupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD},                                                                  */
/*   {.name = "test-option",.has_arg = required_argument,.val = DUF_OPTION_TEST},                                                                 */
/*   {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                     */
/*   {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_FLAG_TREE},                                                                         */
/*   {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                  */
/*   {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_FLAG_UNI_SCAN},                                                                 */
/* (*{.name = "update-exif",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_EXIF}, *)             */
/* (*{.name = "update-mdpath",                     .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH}, *)           */
/* (*{.name = "update-mdpath-selective",           .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, *) */
/* (*{.name = "update-path",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_PATH}, *)             */
/*   {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_FLAG_VACUUM},                                                                     */
/*   {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ZERO_DB},                                                                   */
/* (*{.name = "zero-duplicates",                   .has_arg = no_argument,                         .val = DUF_OPTION_ZERO_DUPLICATES}, *)         */
/*   {0},                                                                                                                                         */
/* };                                                                                                                                             */
/*                                                                                                                                                */
/* const duf_option_t *duf_longopts = &duf_longopts_table[0];                                                                                     */
/* unsigned duf_longopts_count = sizeof( duf_longopts_table ) / sizeof( duf_longopts_table[0] );                                                  */
