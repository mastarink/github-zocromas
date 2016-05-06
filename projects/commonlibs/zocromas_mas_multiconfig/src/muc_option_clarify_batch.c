/* #undef MAS_TRACING */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */

#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_clarify_new.h"
#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_clarify_batch.h"
/* ###################################################################### */

static
muc_SR( OPTIONS, boption_frag_xclarify_at_stdx, muc_config_cli_t * cli, const char **ppeo, char value_separator, muc_option_stage_t istage,
        muc_option_source_t source, char delim, muc_option_adata_t * paod )
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
    muc_CR( soption_xclarify_s_new_at_stdx_default, cli, bstr, value_separator, istage, source, paod );
  }
  mas_free( bstr );
  *ppeo = estr;
/* MAST_TRACE( explain, 0, "*ppeo \"%s\"", *ppeo ); */
  muc_ER( OPTIONS, boption_frag_xclarify_at_stdx, muc_config_cli_t * cli, const char **ppeo, char value_separator, muc_option_stage_t istage,
          muc_option_source_t source, char delim, muc_option_adata_t * paod );
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
muc_SR( OPTIONS, boption_xclarify_at_stdx, muc_config_cli_t * cli, char value_separator, muc_option_stage_t istage, muc_option_source_t source,
        const char *cmdstr, char delim, muc_option_adata_t * paod )
{
  if ( !delim )
    delim = muc_cli_options_delimiter( cli, source );
  /* fprintf(stderr, "A delim(%c) %s : %s\n", delim ? delim : '?', muc_optsource_name( cli, source ), muc_optstage_name( cli, istage ) ); */
  if ( cmdstr && *cmdstr == ':' )
  {
    cmdstr++;
    delim = *cmdstr++;
  }
  /* fprintf(stderr, "B delim(%c) %s : %s\n", delim ? delim : '?', muc_optsource_name( cli, source ), muc_optstage_name( cli, istage ) ); */
  if ( !delim )
    delim = ':';
  /* fprintf(stderr, "C delim(%c) %s : %s\n", delim ? delim : '?', muc_optsource_name( cli, source ), muc_optstage_name( cli, istage ) ); */
  while ( QNOERR && cmdstr && *cmdstr )
  {
    muc_CR( boption_frag_xclarify_at_stdx, cli, &cmdstr, value_separator, istage, source, delim, paod );
  }
  muc_ER( OPTIONS, boption_xclarify_at_stdx, muc_config_cli_t * cli, char value_separator, muc_option_stage_t istage, muc_option_source_t source,
          const char *cmdstr, char delim, muc_option_adata_t * paod );
}
