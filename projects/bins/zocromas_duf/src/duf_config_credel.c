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
  DEBUG_START(  );
  duf_config = duf_cfg_create(  );
  assert( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config;

#  if 0
  duf_config->cli.carg.argc = argc;
  duf_config->cli.carg.argv = argv;
  if ( !duf_config->cli.shorts )
    duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );
#  else
  duf_cli_init( &duf_config->cli, argc, argv );
#  endif

  assert( duf_config4trace );
#endif
  DEBUG_END(  );
}

void
duf_config_delete( void )
{
  DEBUG_START(  );

  DUF_TRACE( config, 0, "deleting config %p:%p:%p:%p", duf_output_file(  ), duf_trace_file(  ), stderr, stdout );
#if 0                           /* 20160115.115839 */
  mas_error_report_all( 0, MAST_TRACE_FILE, DUF_CONFIGG( opt.dbg.verbose ) );
#else
  mas_error_report_all( 0, DUF_CONFIGG( opt.trace.output.out ), DUF_CONFIGG( opt.dbg.verbose ) );
#endif

  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config = NULL;
#endif
  DEBUG_END(  );
}
