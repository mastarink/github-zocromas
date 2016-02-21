#undef MAS_TRACING
#include <string.h>

#include "duf_maintenance_options.h"

#include "duf_config_util.h"

#include "duf_option_config.h"
#include "duf_option_clarify_new.h"
#include "duf_option_source.h"

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
    DUF_TRACE( explain, 0, "option from \"%s\"", bstr );
    estr++;
  }
  else
  {
    bstr = mas_strdup( *ppeo );
    DUF_TRACE( explain, 0, "option (last) from \"%s\"", bstr );
  }
  if ( bstr )
  {
    DUF_TRACE( explain, 0, "bstr: \"%s\"", bstr );

/* look all std xvtables for cmd from string and exec if found */

    CR( soption_xclarify_s_new_at_stdx_default, cli, bstr, value_separator, istage, source, paod );
  }
  mas_free( bstr );
  *ppeo = estr;
/* DUF_TRACE( explain, 0, "*ppeo \"%s\"", *ppeo ); */
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
#if 0
  if ( !delim )
    delim = duf_option_delimiter(  );
#else
  if ( !delim )
    delim = duf_cli_options_delimiter( cli );
#endif
  if ( !delim )
    delim = ':';
  if ( cmdstr && *cmdstr == ':' )
  {
    cmdstr++;
    delim = *cmdstr++;
  }
  while ( cmdstr && *cmdstr )
  {
    CR( boption_frag_xclarify_at_stdx, cli, &cmdstr, value_separator, istage, source, delim, paod );
  }
  ER( OPTIONS, boption_xclarify_at_stdx, duf_config_cli_t * cli, char value_separator, duf_option_stage_t istage, duf_option_source_t source,
      const char *cmdstr, char delim, duf_option_adata_t * paod );
}
