/* #include <stdarg.h> */
#include <string.h>

#include <mastar/trace/mas_trace_credel.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_config4trace_ref.h"
/* #include "duf_config_trace_credel.h" */
#include "duf_cfg_credel.h"

#include "duf_option_config.h"
#include "duf_option_config_credel.h"
#include "duf_xtended_table.h"

#include "duf_expandable.h"

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
  duf_config->opt.ptracecfg = mas_config_trace_create( DUF_TRACE_LEVEL_MAX );
  duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_errors] += 2;
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 1;  */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_fs] += 1;      */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 11; */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 71; */
  duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_temp] += 2;

  duf_config4trace = duf_config;
  assert( duf_config4trace );
#endif

#if 0
  duf_config->cli.carg.argc = argc;
  duf_config->cli.carg.argv = argv;
  if ( !duf_config->cli.shorts )
    duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );
#else
# if 0
  duf_cli_init(  /* &duf_config->cli, */ argc, argv );
# else
  duf_get_config(  )->pcli = duf_cli_options_create( argc, argv, duf_xtable_list(  ), DUF_CONFIGGS( conf.config_dir ),
                                                     DUF_CONFIGGS( conf.cmds_dir ), duf_string_options_at_string_xsdb_getvar,
                                                     duf_config->opt.ptracecfg );
# endif

#endif
  duf_config->db.main.name_x.varfunc = duf_config->db.tempo.name_x.varfunc = duf_config->db.adm.name_x.varfunc = duf_config->db.dir_x.varfunc =
          duf_config->db.subdir_x.varfunc = duf_string_options_at_string_xsdb_getvar;
  DUF_END(  );
}

void
duf_config_delete( void )
{
  DUF_START(  );
  /* assert( duf_config->opt.ptracecfg->stream.out ); */
  /* assert( duf_get_trace_config(  )->stream.out );  */
/* T( "@%p %p", duf_config->opt.ptracecfg, duf_config->opt.ptracecfg->class_levels ); */
  DUF_TRACE( config, 0, "deleting config %p:%p:%p:%p", duf_output_file(  ), duf_trace_file(  ), stderr, stdout );
/* T( "@%p/%p %p", duf_config->opt.ptracecfg, duf_get_trace_config(  ), duf_config->opt.ptracecfg->class_levels ); */
  mas_error_report_all( duf_get_trace_config(  ), DUF_TRACE_LEVEL_errors, 0, /*FIXME: error output is not trace output */
                        duf_trace_file_c( duf_config->opt.ptracecfg ), duf_verbose ? duf_verbose(  ) : 0 );
  mas_config_trace_delete( duf_config->opt.ptracecfg );
  duf_config->opt.ptracecfg = NULL;
/* duf_cli_options_shut_global(  ); */
  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
  duf_config4trace =
#endif
          duf_config = NULL;

  DUF_END(  );
}
