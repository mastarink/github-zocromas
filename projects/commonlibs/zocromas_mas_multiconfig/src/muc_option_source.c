/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include <stddef.h>                                                  /* NULL */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

/* ###################################################################### */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */
/* ###################################################################### */

const char *
muc_optsourcecode_name( muc_config_cli_t * cli __attribute__ ( ( unused ) ), muc_option_source_code_t sourcecode )
{
  static const char *tail[] = {
    [MUC_OPTION_SOURCE_ENV] = "env",
    [MUC_OPTION_SOURCE_STDIN] = "stdin",
    [MUC_OPTION_SOURCE_STREAM] = "stream",
    [MUC_OPTION_SOURCE_DUFFILE] = "file",
    [MUC_OPTION_SOURCE_CFG] = "cfg",
    [MUC_OPTION_SOURCE_CFGSTG] = "cfg-stage",
    [MUC_OPTION_SOURCE_CFGLAST] = "cfg-last",
    [MUC_OPTION_SOURCE_CLI] = "cli",
    [MUC_OPTION_SOURCE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( sourcecode >= MUC_OPTION_SOURCE_MIN
           && sourcecode <= MUC_OPTION_SOURCE_MAX ) ? ( tail[sourcecode] ? tail[sourcecode] : "unknown" ) : "unset";
}

const char *
muc_optsource_name( muc_config_cli_t * cli, muc_option_source_t source )
{
  return muc_optsourcecode_name( cli, source.sourcecode );
}

const char *
muc_optsource_label( muc_config_cli_t * cli __attribute__ ( ( unused ) ), muc_option_source_t source )
{
  return source.label ? source.label : "-";
}

int
muc_optsource_labelled( muc_config_cli_t * cli __attribute__ ( ( unused ) ), muc_option_source_t source )
{
  return source.labelled;
}
