/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_tracen_defs_preset.h"
#include "duf_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_clarify_new.h"
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

/* ###################################################################### */
#include "duf_option_clarify_batch.h"
/* ###################################################################### */

static
SR( OPTIONS, boption_frag_xclarify_at_stdx, duf_config_cli_t * cli, const char **ppeo, char value_separator, duf_option_stage_t istage,
    duf_option_source_t source, char delim, duf_option_adata_t * paod )
{
  const char *estr;
  char *bstr;

  bstr = NULL;
  estr = strchr( *ppeo, delim );
  if ( estr )
  {
    bstr = mas_strndup( *ppeo, estr - *ppeo );
    MAST_TRACE( explain, 0, "option from \"%s\"", bstr );
    estr++;
  }
  else
  {
    bstr = mas_strdup( *ppeo );
    MAST_TRACE( explain, 0, "option (last) from \"%s\"", bstr );
  }
  if ( bstr )
  {
    MAST_TRACE( explain, 0, "bstr: \"%s\"", bstr );

/* look all std xvtables for cmd from string and exec if found */
    CR( soption_xclarify_s_new_at_stdx_default, cli, bstr, value_separator, istage, source, paod );
  }
  mas_free( bstr );
  *ppeo = estr;
/* MAST_TRACE( explain, 0, "*ppeo \"%s\"", *ppeo ); */
  ER( OPTIONS, boption_frag_xclarify_at_stdx, duf_config_cli_t * cli, const char **ppeo, char value_separator, duf_option_stage_t istage,
      duf_option_source_t source, char delim, duf_option_adata_t * paod );
}

/*
 * if DUF_CONFIGG(opt.option_delimiter) NOT set
 * "::trace-path=1:trace-options=1"  -- correct
 * ":;trace-path=1;trace-options=1"  -- correct
 * ":,trace-path=1,trace-options=1"  -- correct
 * "trace-path=1:trace-options=1"    -- correct
 *
 * */
/* look all std xvtables for cmd's separated with value_separator from string and exec if found */
SR( OPTIONS, boption_xclarify_at_stdx, duf_config_cli_t * cli, char value_separator, duf_option_stage_t istage, duf_option_source_t source,
    const char *cmdstr, char delim, duf_option_adata_t * paod )
{
  if ( !delim )
    delim = duf_cli_options_delimiter( cli );
  if ( !delim )
    delim = ':';
  if ( cmdstr && *cmdstr == ':' )
  {
    cmdstr++;
    delim = *cmdstr++;
  }
  while ( QNOERR && cmdstr && *cmdstr )
  {
    CR( boption_frag_xclarify_at_stdx, cli, &cmdstr, value_separator, istage, source, delim, paod );
  }
  ER( OPTIONS, boption_xclarify_at_stdx, duf_config_cli_t * cli, char value_separator, duf_option_stage_t istage, duf_option_source_t source,
      const char *cmdstr, char delim, duf_option_adata_t * paod );
}
