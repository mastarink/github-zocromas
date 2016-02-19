#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_defs.h"

#include "duf_xtended_table.h"

#include "duf_option_config.h"
/* ###################################################################### */
#include "duf_cli.h"
/* ###################################################################### */

void
duf_cli_init( duf_config_cli_t * cli, int argc, char **argv )
{
  /* cli->carg.argc = argc;                       */
  /* cli->carg.argv = argv;                       */
  /* cli->xtable_multi = lo_extended_table_multi; */
#if 0
  if ( !cli->shorts )
    cli->shorts = duf_cli_options_shorts( lo_extended_table_multi );
#else
  duf_cli_options_init( cli, argc, argv, duf_extended_vtable_multi(), DUF_CONFIGGS( conf.config_dir ), DUF_CONFIGGS( conf.cmds_dir ) );
  /* duf_cli_options_shorts_init( lo_extended_table_multi ); */
#endif
}

void
duf_cli_shut( duf_config_cli_t * cli )
{
  /* mas_del_argv( cli->targ.argc, cli->targ.argv, 0 ); */
  /* cli->targ.argc = 0;                                */
  /* cli->targ.argv = NULL;                             */
  duf_cli_options_shut( cli );
}

/* mas_argvc_t *                          */
/* duf_cli_targ( duf_config_cli_t * cli ) */
/* {                                      */
/*   return cli->targ;                    */
/* }                                      */
