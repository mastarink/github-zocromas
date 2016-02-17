#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_trace.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_expandable.h"

#include "duf_option_config.h"

/* ###################################################################### */
#include "duf_config_show_etc.h"
/* ###################################################################### */

int
duf_config_show( void )
{
  DUF_STARTR( r );

  if ( duf_config )
  {
    DUF_TRACE( temp, 4, "@@@db.dir: %s", DUF_CONFIGGSP( db.dir ) );
    DUF_TRACE( temp, 4, "@@@db.path: %s", DUF_CONFIGGS( db.path ) );
  }
  for ( int ia = 0; ia < duf_config->cli.targ.argc; ia++ )
    /* DUF_FPRINTF( 0, stderr, "@@@@targ.argv[%d]: %s", ia, duf_config->cli.targ.argv[ia] ); */
    DUF_TRACE( temp, 3, "@@@@targ.argv[%d]: %s", ia, duf_cli_options_get_targ(  )->argv[ia] );

  DUF_ENDR( r );
}

int
duf_config_optionally_show( void )
{
  DUF_STARTR( r );

  if ( duf_config->cli.verbose > 1 )
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



