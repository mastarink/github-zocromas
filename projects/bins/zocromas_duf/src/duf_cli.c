#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_option_names.h"
#include "duf_options_table.h"

/* ###################################################################### */
#include "duf_cli.h"
/* ###################################################################### */

void
duf_cli_init( duf_config_cli_t * cli, int argc, char **argv )
{
  cli->carg.argc = argc;
  cli->carg.argv = argv;
  if ( !cli->shorts )
    cli->shorts = duf_cli_option_shorts( lo_extended_table_multi );
}

void
duf_cli_shut( duf_config_cli_t * cli )
{
  mas_del_argv( cli->targ.argc, cli->targ.argv, 0 );
  cli->targ.argc = 0;
  cli->targ.argv = NULL;

  mas_free( cli->shorts );
  cli->shorts = NULL;
}

/* mas_argvc_t *                          */
/* duf_cli_targ( duf_config_cli_t * cli ) */
/* {                                      */
/*   return cli->targ;                    */
/* }                                      */
