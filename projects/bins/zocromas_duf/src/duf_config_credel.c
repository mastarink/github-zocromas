/* #include <stdarg.h> */
#include <string.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_trace.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_cli.h"

#include "duf_config4trace_ref.h"
#include "duf_cfg_credel.h"
/* ###################################################################### */
#include "duf_config_credel.h"
/* ###################################################################### */

void
duf_config_create( int argc, char **argv )
{
  DUF_START(  );
  duf_config = duf_cfg_create(  );
  assert( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config;
  assert( duf_config4trace );
#endif

#if 0
  duf_config->cli.carg.argc = argc;
  duf_config->cli.carg.argv = argv;
  if ( !duf_config->cli.shorts )
    duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );
#else
  duf_cli_init( &duf_config->cli, argc, argv );
#endif

  DUF_END(  );
}

void
duf_config_delete( void )
{
  DUF_START(  );

  DUF_TRACE( config, 0, "deleting config %p:%p:%p:%p", duf_output_file(  ), duf_trace_file(  ), stderr, stdout );

  mas_error_report_all( 0, DUF_CONFIGG( opt.trace.output.out ), duf_verbose ? duf_verbose(  ) : 0 );

  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
  duf_config4trace =
#endif
          duf_config = NULL;
  DUF_END(  );
}
