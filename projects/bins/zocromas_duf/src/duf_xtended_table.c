#include <string.h>

#include "duf_maintenance.h"



/* man getopt_long */


/* ###################################################################### */
#include "duf_xtended_table.h"
/* ###################################################################### */

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
extern const duf_longval_extended_table_t optable_io;
extern const duf_longval_extended_table_t optable_experimental;
extern const duf_longval_extended_table_t optable_fs;
extern const duf_longval_extended_table_t optable_interactive;
extern const duf_longval_extended_table_t optable_str;
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
extern const duf_longval_extended_table_t optable_debug;
extern const duf_longval_extended_table_t optable_redo_and_obsolete;

static const duf_longval_extended_table_t *_lo_extended_table_multi[] = {
  &optable_main,
  &optable_experimental,
  &optable_fs,
  &optable_io,
  &optable_interactive,
  &optable_str,
  &optable_sccb,
  &optable_info,
  &optable_sflag,
  &optable_flag,
  &optable_uplus,
#ifdef MAS_TRACING_OPTIONS
  &optable_trace,
#endif
  &optable_filter,
  &optable_debug,
  &optable_help,
  &optable_format,
  &optable_redo_and_obsolete,

  NULL
};

static const duf_longval_extended_table_t **lo_extended_table_multi = NULL;

static void
_duf_extended_table_multi_init( void )
{
  unsigned n = 0;
  const duf_longval_extended_table_t **src = NULL;
  const duf_longval_extended_table_t **dst = NULL;

  src = _lo_extended_table_multi;
  while ( src && *src )
  {
    n++;
    src++;
  }
  n++;
  lo_extended_table_multi = mas_malloc( sizeof( duf_longval_extended_table_t ** ) * n );
  memset( lo_extended_table_multi, 0, n );
  dst = lo_extended_table_multi;
  for ( const duf_longval_extended_table_t ** src = _lo_extended_table_multi; src && *src; src++, dst++ )
  {
    *dst = *src;
  }
  *dst++ = NULL;
#if 0
  for ( const duf_longval_extended_table_t ** dst = lo_extended_table_multi; dst && *dst; dst++ )
  {
    T( "t:%p '%s'", dst, ( *dst )->name );
  }
#endif
}

static void
duf_extended_table_multi_init( void )
{
  static unsigned inited = 0;

  if ( !inited )
  {
    _duf_extended_table_multi_init(  );
    inited = 1;
  }
}

static void extended_table_multi_init( void ) __attribute__ ( ( constructor( 101 ) ) );
static void
extended_table_multi_init( void )
{
  /* duf_extended_table_multi_init(  ); */
}

const duf_longval_extended_table_t **
duf_extended_table_multi( void )
{
  duf_extended_table_multi_init(  );
  return lo_extended_table_multi;
}


/* _lo_extended_table_multi; */
static void unbuild( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
unbuild( void )
{
  mas_free( lo_extended_table_multi );
}

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
