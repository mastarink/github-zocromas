#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace_credel.h>
#include <mastar/trace/mas_trace.h>
/* #include <mastar/error/mas_error_defs_ctrl.h> */
/* #include <mastar/error/mas_error_defs_make.h> */
/* #include <mastar/error/mas_error_defs.h>      */

#include <mastar/multiconfig/muc_option_config.h>                    /* duf_get_cli_options_trace_config ♠ */
#include <mastar/multiconfig/muc_option_config_credel.h>
#include <mastar/multiconfig/muc_option_types.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_handle.h"

/* man getopt_long */

/* ###################################################################### */
#include "duf_xtended_table.h"
/* ###################################################################### */

extern const muc_longval_extended_table_t optable_main;
extern const muc_longval_extended_table_t optable_io;
extern const muc_longval_extended_table_t optable_experimental;
extern const muc_longval_extended_table_t optable_fs;
extern const muc_longval_extended_table_t optable_interactive;
/* extern const muc_longval_extended_table_t optable_str; */
extern const muc_longval_extended_table_t optable_options;
/* extern const muc_longval_extended_table_t optable_db; */
extern const muc_longval_extended_table_t optable_sccb;

/* extern const muc_longval_extended_table_t optable_info; */
/* extern const muc_longval_extended_table_t optable_flag; */
/* extern const muc_longval_extended_table_t optable_flag_allow; */
extern const muc_longval_extended_table_t optable_sflag;
extern const muc_longval_extended_table_t optable_uplus;
extern const muc_longval_extended_table_t optable_help;

#ifdef MAS_TRACING_OPTIONS
extern const muc_longval_extended_table_t optable_trace;
#endif
extern const muc_longval_extended_table_t optable_format;

/* extern const muc_longval_extended_table_t optable_filter; */
/* extern const muc_longval_extended_table_t optable_test; */
extern const muc_longval_extended_table_t optable_debug;
extern const muc_longval_extended_table_t optable_redo_and_obsolete;

static const muc_longval_extended_table_t *const __lo_extended_table_multi[] = {
  &optable_main,
  &optable_experimental,
  &optable_fs,
  &optable_io,
  &optable_interactive,
  /* &optable_str, */
  &optable_options,
  /* &optable_db, */
  &optable_sccb,
/* &optable_info, */
  &optable_sflag,
  /* &optable_flag, */
  /* &optable_flag_allow, */
  &optable_uplus,
#ifdef MAS_TRACING_OPTIONS
  &optable_trace,
#endif
/* &optable_filter, */
  &optable_help,
  &optable_format,
  &optable_redo_and_obsolete,

  NULL
};

const muc_longval_extended_table_t *const *
duf_optable_xtable_list( void )
{
  return __lo_extended_table_multi;
}

#if 1
static const muc_longval_extended_table_t *const __lo_extended_table_multi2[] = {
  &optable_debug,
  /* &optable_test, */
  NULL
};

const muc_longval_extended_table_t *const *
duf_optable_xtable_list2( void )
{
  return __lo_extended_table_multi2;
}
#endif

muc_longval_extended_table_t **list_mod = NULL;
const muc_longval_extended_table_t *const *
duf_optable_xtable_list_mod( void )
{
  if ( !list_mod )
  {
    char **liblist;
    int num = 0, i = 0;

    liblist = duf_liblist( "ot_*.so", &num );
    list_mod = mas_malloc( sizeof( const muc_longval_extended_table_t * const * ) * ( num + 1 ) );
    memset( list_mod, 0, sizeof( const muc_longval_extended_table_t * const * ) * ( num + 1 ) );
    muc_longval_extended_table_t **lm = list_mod;

    for ( char **ll = liblist; ll && *ll; ll++ )
    {
      muc_longval_extended_table_t *mod;

      mod = duf_load_mod_handler_symbol_find( *ll, "optab" );
    /* QT( "@lib: %s : %p", *ll, mod ); */
      if ( mod )
        *lm++ = mod;
    /* QT( "@%d of %d : %s", i, num, *ll ); */
      i++;
    }
  /* TODO realloc list_mod ? */
    if ( liblist )
      duf_delete_liblist( liblist );
  }
  return ( const muc_longval_extended_table_t * const * ) list_mod;
}

static void xtable_destructor( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
xtable_destructor( void )
{
  mas_free( list_mod );
}
