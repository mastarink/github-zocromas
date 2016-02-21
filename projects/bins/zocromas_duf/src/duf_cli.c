#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_defs.h"
#include "duf_config_util.h"

#include "duf_xtended_table.h"

#include "duf_option_config.h"
#include "duf_option_config_credel.h"
/* ###################################################################### */
#include "duf_cli.h"
/* ###################################################################### */

void
duf_cli_init( /* duf_config_cli_t * cli, */ int argc, char **argv )
{
#if 0 
  duf_cli_options_init_global( cli, argc, argv, duf_xtable_list(  ), DUF_CONFIGGS( conf.config_dir ), DUF_CONFIGGS( conf.cmds_dir ),
                               duf_string_options_at_string_xsdb_getvar );
#else
  duf_cli_options_init_global( duf_get_config_cli(), argc, argv, duf_xtable_list(  ), DUF_CONFIGGS( conf.config_dir ), DUF_CONFIGGS( conf.cmds_dir ),
                               duf_string_options_at_string_xsdb_getvar );
#endif
}

void
duf_cli_shut( void )
{
/* mas_del_argv( cli->targ.argc, cli->targ.argv, 0 ); */
/* cli->targ.argc = 0;                                */
/* cli->targ.argv = NULL;                             */
  duf_cli_options_shut_global(  );
}

/* mas_argvc_t *                          */
/* duf_cli_targ( duf_config_cli_t * cli ) */
/* {                                      */
/*   return cli->targ;                    */
/* }                                      */
