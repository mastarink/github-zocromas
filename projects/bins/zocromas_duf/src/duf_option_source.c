#undef MAS_TRACING
#include <stddef.h>                                                  /* NULL */

/* ###################################################################### */
#include "duf_option_source.h"
/* ###################################################################### */

const char *
duf_optsourcecode_name( duf_option_source_code_t sourcecode )
{
  static const char *tail[] = {
    [DUF_OPTION_SOURCE_ENV] = "env",
    [DUF_OPTION_SOURCE_STDIN] = "stdin",
    [DUF_OPTION_SOURCE_STREAM] = "stream",
    [DUF_OPTION_SOURCE_DUFFILE] = "file",
    [DUF_OPTION_SOURCE_CFG] = "cfg",
    [DUF_OPTION_SOURCE_CFGSTG] = "cfg-stage",
    [DUF_OPTION_SOURCE_CFGLAST] = "cfg-last",
    [DUF_OPTION_SOURCE_CLI] = "cli",
    [DUF_OPTION_SOURCE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( sourcecode >= DUF_OPTION_SOURCE_MIN
           && sourcecode <= DUF_OPTION_SOURCE_MAX ) ? ( tail[sourcecode] ? tail[sourcecode] : "unknown" ) : "unset";
}

const char *
duf_optsource_name( duf_option_source_t source )
{
  return duf_optsourcecode_name( source.sourcecode );
}

const char *
duf_optsource_label( duf_option_source_t source )
{
  return source.label ? source.label : "-";
}

int
duf_optsource_labelled( duf_option_source_t source )
{
  return source.labelled;
}
