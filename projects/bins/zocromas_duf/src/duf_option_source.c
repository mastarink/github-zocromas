#include "duf_maintenance_options.h"


/* ###################################################################### */
#include "duf_option_source.h"
/* ###################################################################### */

const char *
duf_optsource_name( duf_option_source_t source )
{
  static const char *tail[] = {
    [DUF_OPTION_SOURCE_ENV] = "env",
    [DUF_OPTION_SOURCE_STDIN] = "stdin",
    [DUF_OPTION_SOURCE_STREAM] = "stream",
    [DUF_OPTION_SOURCE_DUFFILE] = "file",
    [DUF_OPTION_SOURCE_CFG] = "cfg",
    [DUF_OPTION_SOURCE_CLI] = "cli",
    [DUF_OPTION_SOURCE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( source.sourcecode >= DUF_OPTION_SOURCE_MIN
           && source.sourcecode <= DUF_OPTION_SOURCE_MAX ) ? ( tail[source.sourcecode] ? tail[source.sourcecode] : "unset" ) : "unknown";
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
