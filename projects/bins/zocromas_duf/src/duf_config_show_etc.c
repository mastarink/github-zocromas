#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_output_util.h"

#include "duf_expandable.h"                                          /* duf_expandable_string_t; duf_string_expanded ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

/* ###################################################################### */
#include "duf_config_show_etc.h"
/* ###################################################################### */

int
duf_config_show( void )
{
  DUF_STARTR( r );

  if ( duf_config )
  {
    DUF_TRACE( temp, 4, "@@@db.dir: %s", DUF_CONFIGGSP( db.dir_x ) );
    DUF_TRACE( temp, 4, "@@@db.path: %s", DUF_CONFIGGS( db.path ) );
    if ( duf_config->pcli )
      for ( int ia = 0; ia < duf_config->pcli->targ.argc; ia++ )
      /* DUF_FPRINTF( 0, stderr, "@@@@targ.argv[%d]: %s", ia, duf_config->cli.targ.argv[ia] ); */
        DUF_TRACE( temp, 3, "@@@@targ.argv[%d]: %s", ia, duf_cli_options_get_targ( duf_get_config_cli(  ) )->argv[ia] );
  }

  DUF_ENDR( r );
}

int
duf_config_optionally_show( void )
{
  DUF_STARTR( r );

  if ( duf_verbose && duf_verbose(  ) > 1 )
  {
    DOR( r, duf_config_show(  ) );
  }
  else
  {
    DUF_TRACE( explain, 0, "not showing config: not verbose" );
  }

#if 0
/* Obsolete ??? */
  DUF_TRACE( temporary, 0, "@ maxitems.total %lld", DUF_CONFIGG( vars.puz )->maxitems.total );
  DUF_TRACE( temporary, 0, "@ maxitems.files %lld", DUF_CONFIGG( vars.puz )->maxitems.files );
  DUF_TRACE( temporary, 0, "@ maxitems.dirs %lld", DUF_CONFIGG( vars.puz )->maxitems.dirs );
  DUF_TRACE( temporary, 0, "@ dirfiles.min %u", DUF_CONFIGG( vars.puz )->dirfiles.min );
  DUF_TRACE( temporary, 0, "@ dirfiles.max %u", DUF_CONFIGG( vars.puz )->dirfiles.max );
#endif

#ifdef MAS_TRACING
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->vars.puz->globx.include_fs_files.argc, duf_config->vars.puz->globx.include_fs_files.argv, 0 );
    DUF_TRACE( config, 0, "@ include-fs %s", sif );
    mas_free( sif );
  }
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->vars.puz->globx.exclude_fs_files.argc, duf_config->vars.puz->globx.exclude_fs_files.argv, 0 );
    DUF_TRACE( config, 0, "@ exclude-fs %s", sif );
    mas_free( sif );
  }
#endif

  DUF_ENDR( r );
}
