#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

/* extern mas_options_t gopts; */

#include <mastar/modules/mas_modules_commands_eval.h>




int
main( int argc, char *argv[] )
{
  extern mas_options_t gopts;
  char *infos = NULL;

  gopts.dir.mods = "/mnt/new_misc/develop/autotools/zoc/admin/install/lib/zocmds/";
  infos = mas_evaluate_command( MAS_PASS_OPTS_GREF "get version" );
  if ( MAS_VALID_ANSWER( infos ) )
  {
    /* fprintf( stdout, "[%p]\n", infos ); */
    fputs( infos, stdout );
    mas_free( infos );
  }
  else
    infos = NULL;
}
