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
    [DUF_OPTION_SOURCE_FILE] = "file",
    [DUF_OPTION_SOURCE_CFG] = "cfg",
    [DUF_OPTION_SOURCE_CLI] = "cli",
    [DUF_OPTION_SOURCE_INTERACTIVE] = "interactive]",
    NULL
  };
  return ( source >= 0 && source < DUF_OPTION_SOURCE_MAX ) ? tail[source] : "unknown";
}
