/* #include <stdarg.h> */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace_credel.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_option_config.h>                    /* duf_get_cli_options_trace_config ♠ */
#include <mastar/multiconfig/muc_option_config_credel.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_trace.h"                                        /* duf_trace_file_c; duf_trace_output_force_color_c; duf_trace_output_no_color_c ✗ */
#include "duf_config_output_weak.h"
#include "duf_config_ref.h"

#include "duf_cfg_credel.h"

#include "duf_xtended_table.h"

#include "duf_config_structs.h"
/* ###################################################################### */
#include "duf_config_credel.h"
/* ###################################################################### */

void
duf_config_allocate( void )
{
  duf_config = duf_cfg_create(  );
  assert( duf_config );
}

void
duf_config_create( int argc, char **argv, unsigned mandatory_config )
{
  if ( !duf_config )
    duf_config_allocate(  );
  assert( duf_config );
/* DUF_START(  ); */
#ifdef MAS_TRACING
  duf_config->opt.ptracecfg = mas_config_trace_create( DUF_TRACE_LEVEL_MAX );
  duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_errors] += 2;
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 1;  */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_fs] += 1;      */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 11; */
/* duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_options] = 71; */
  duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_temp] += 2;
  QT( "@=== %d:%d", DUF_TRACE_LEVEL_temp, duf_config->opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_temp] );
/* duf_config4trace = duf_config; */
/* assert( duf_config4trace ); */
#endif

#if 0
  duf_config->pcli = muc_cli_options_create( argc, argv, duf_xtable_list(  ), mandatory_config, duf_config->conf.config_dir,
                                             duf_config->conf.cmds_dir, duf_string_options_at_string_xsdb_getvar, duf_config->opt.ptracecfg );
#else
  duf_config->pcli = muc_cli_options_create( argc, argv, NULL /* duf_xtable_list(  ) */ , mandatory_config, duf_config->conf.config_dir,
                                             duf_config->conf.cmds_dir, duf_string_options_at_string_xsdb_getvar, duf_config->opt.ptracecfg );
  muc_cli_options_xtable_list_add( duf_config->pcli, duf_xtable_list(  ), 0 /* numtabs */  );
#endif
#if 0
  muc_cli_options_xtable_list_add( duf_config->pcli, duf_xtable_list2(  ), 0 );
#elif 0
  {
    extern const muc_longval_extended_table_t optable_test;
    const muc_longval_extended_table_t *t = &optable_test;

    muc_cli_options_xtable_list_add( duf_config->pcli, &t, 1 );
  }
#else
  {
    extern const muc_longval_extended_table_t optable_test;

    muc_cli_options_xtable_add_one( duf_config->pcli, &optable_test );
  }
#endif
/* muc_cli_options_postinit( duf_config->pcli ); */

  duf_config->db.main.name_x.varfunc = duf_config->db.tempo.name_x.varfunc = duf_config->db.adm.name_x.varfunc = duf_config->db.dir_x.varfunc =
          duf_config->db.subdir_x.varfunc = duf_string_options_at_string_xsdb_getvar;
/* DUF_END(  ); */
}

void
duf_config_delete( void )
{
/* DUF_START(  ); */
/* assert( duf_config->opt.ptracecfg->stream.out ); */
/* assert( duf_get_trace_config(  )->stream.out );  */
/* T( "@%p %p", duf_config->opt.ptracecfg, duf_config->opt.ptracecfg->class_levels ); */
  MAST_TRACE( config, 0, "deleting config %p:%p:%p:%p", mas_output_file(  ), duf_trace_file_c( duf_config->opt.ptracecfg ), stderr, stdout );
/* T( "@%p/%p %p", duf_config->opt.ptracecfg, duf_get_trace_config(  ), duf_config->opt.ptracecfg->class_levels ); */
  mas_error_report_all( duf_get_trace_config(  ), DUF_TRACE_LEVEL_errors, 0, /*FIXME: error output is not trace output */
                        duf_trace_file_c( duf_config->opt.ptracecfg ), mas_verbose ? mas_verbose(  ) : 0 );
  mas_config_trace_delete( duf_config->opt.ptracecfg );
  duf_config->opt.ptracecfg = NULL;
/* duf_cli_options_shut_global(  ); */
  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
/* duf_config4trace = */
#endif
  duf_config = NULL;

  assert( !duf_config );
/* DUF_END(  ); */
}
