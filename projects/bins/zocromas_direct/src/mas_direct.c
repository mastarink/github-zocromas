#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/modules/mas_modules_commands_eval.h>




int
main( int argc, char *argv[] )
{
  char *infos = NULL;

  opts.modsdir = "/mnt/new_misc/develop/autotools/zoc/admin/install/lib/zocmds/";
  infos = mas_evaluate_command( "server info" );
  if ( MAS_VALID_ANSWER( infos ) )
  {
    /* fprintf( stdout, "[%p]\n", infos ); */
    fputs( infos, stdout );
    mas_free( infos );
  }
  else
    infos = NULL;
}
