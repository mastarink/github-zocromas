#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/modules/mas_modules_commands_eval.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


int
main( int argc, char *argv[] )
{
  extern mas_options_t gopts;
  mas_evaluated_t *infos = NULL;

  /* gopts.dir.mods = "/mnt/new_misc/develop/autotools/zoc/admin/install/lib/zocmds/"; */
  gopts.dir.mods = "/mnt/new_misc/develop/autotools/zoc/admin/install/default/lib/zocromod/zocmds/";
  infos = mas_evaluate_command( &gopts, "get version" );
  if ( MAS_VALID_ANSWER( infos ) )
  {
    /* fprintf( stdout, "[%p]\n", infos ); */
    HMSG( "ANSWER: %s", ( const char * ) infos->data );
    /* fputs( infos->data, stdout ); */
    /* mas_free( infos ); */
    mas_evaluated_delete( infos );
  }
  else
  {
    HMSG( "NO ANSWER" );
    infos = NULL;
  }
}
