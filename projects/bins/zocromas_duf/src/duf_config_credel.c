/* #include <stdarg.h> */
#include <assert.h>
#include <string.h>

#include <mastar/trace/mas_trace_credel.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_trace.h"                                        /* duf_trace_file_c; duf_trace_output_force_color_c; duf_trace_output_no_color_c ♠ */
#include "duf_config_output_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_config4trace_ref.h"
/* #include "duf_config_trace_credel.h" */
#include "duf_cfg_credel.h"

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_config_credel.h"
#include "duf_xtended_table.h"

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

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
  DUF_TRACE( config, 0, "deleting config %p:%p:%p:%p", duf_output_file(  ), duf_trace_file_c( duf_config->opt.ptracecfg ), stderr, stdout );
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
