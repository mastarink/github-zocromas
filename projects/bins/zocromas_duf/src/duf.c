/*
 * TODO :
 *   - libarchive
 *   - option as for du(1)
 *         -x, --one-file-system :      skip directories on different file systems
 *   - 
 * */

/*! \mainpage
  main
    duf_main
      duf_main_with_config
	duf_all_options
	duf_main_db
	  duf_main_db_open
	  duf_main_db_close

  duf_optimpl_O_list_sccbs
  duf_optimpl_O_evaluate_sccb

  duf_optimpl_O_list_sccb
  duf_optimpl_O_db_open

  duf_optimpl_O_cd
  ... ...
*/

/*

   run  /mnt/new_media/media/photo/20130507/  -dfR --ls

*/

#include <assert.h>                                                  /* assert */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <link.h>
#include <time.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_options_all_stages.h>
#include <mastar/multiconfig/muc_option_names.h>

#include "duf_status.h"

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_config_credel.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */
#include "duf_pdi_global.h"                                          /* duf_pdi_global ✗ */

/* צאַצקע */
/* #include "duf_experiment.h" */

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

const mas_memfun_block_t *
mas_memory_allocation( void )
{
/* fprintf( stderr, "allocation: %p\n", MAS_MEMORY_ALLOCATION_STD ); */
/* return MAS_MEMORY_ALLOCATION_STD; */
  return MAS_MEMORY_ALLOCATION_MASTAR;
}

static int
lcb( struct dl_phdr_info *info MAS_UNUSED, size_t size MAS_UNUSED, void *data MAS_UNUSED )
{
/* fprintf( stderr, "-=- %s\n", info->dlpi_name ); */
  QT( "@@@@@-=- %s", info->dlpi_name );
  return 0;
}

/*
TODO Ideas: count for each dir pair number of matching files => path_pairs

*/
static void constructor_main_z( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 101 ) ) );
static void
constructor_main_z( int argc MAS_UNUSED, char **argv MAS_UNUSED, char **envp MAS_UNUSED )
{
  fprintf( stderr, "%s.%d : %d\n", __FILE__, __LINE__, argc );
}

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc MAS_UNUSED, char **argv MAS_UNUSED, char **envp MAS_UNUSED )
{
  fprintf( stderr, "%s.%d : %d\n", __FILE__, __LINE__, argc );
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

#endif
  duf_config_allocate(  );
  duf_config_create( argc, argv, 1 /* mandatory_config */  );
  dl_iterate_phdr( lcb, NULL );
}

static void destructor_main( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_main( void )
{
  duf_config_delete(  );
}

int __attribute__ ( ( weak ) ) mas_verbose( void )
{
  return duf_get_config_num_flow_verbose(  );
/* return duf_config ? duf_config->opt.flow.verbose : 0; */
}

int
mas_dry_run( void )
{
  return duf_get_config_flag_flow_dry_run(  );
/* return duf_config ? duf_config->opt.flow.v.flag.dry_run : 0; */
}

#if 0
static int
cb_do_interactive( void )
{
  return DUF_ACTG_FLAG( interactive );
}
#endif
/* 20160312.115753 */
static const char *
cb_prompt_interactive( void )
{
  static char rl_prompt[256 * 10] = "";

  snprintf( rl_prompt, sizeof( rl_prompt ), "A-F:%d;A-D:%d; %s:%s> ", duf_get_config_flag_act_allow_files(  ), duf_get_config_flag_act_allow_dirs(  ),
            "db", CRX( levinfo_path, CRX( pdi_global ) ) );
  return rl_prompt;
}

static
SR( TOP, main_with_config )
{
/* config-cli created at M_config_create at M_main */
  muc_CR( treat_option_stage_ne, CRX( get_config_cli ), MUC_OPTION_STAGE_DEBUG, F2ND( pdi_reinit_anypath_global ), duf_get_config_flag_act_interactive, cb_prompt_interactive ); /* here to be before following MAST_TRACE's */
  muc_CR( treat_option_stage_ne, CRX( get_config_cli ), MUC_OPTION_STAGE_BOOT, F2ND( pdi_reinit_anypath_global ), duf_get_config_flag_act_interactive,
          cb_prompt_interactive );

  MAST_TRACE( any, 1, "any test" );
  MAST_TRACE( explain, 0, "to run main_db" );

  muc_CR( treat_all_optstages, CRX( get_config_cli ), F2ND( pdi_create_global ), F2ND( pdi_reinit_anypath_global ),
          duf_get_config_flag_act_interactive, cb_prompt_interactive );

  fputs( "\n", stderr );
  MAST_TRACE( temp, 0, "∈0∋ Zero " );
  QTT( "∈1∋ One " );
  QTT( "∈2∋ Two " );
  QTT( "∈1∋ Three " );
  QTT( "∈3 Four " );
  QTT( "∈1∋ Five " );
  QTT( "∈4∋ Six " );
  QTT( "∈1∋ Seven " );
  QTT( "∈6 Eight " );
  QTT( "∈1∋ Nine " );
  QTT( "∈7∋ Ten " );
  QTT( "∈8∋ -= !! Split config : duf-related and opt/cfg+i/o related !! =- " );
  QT( "@act: %d; i: %d; no: %d;", duf_get_config_flag_act_testflag(  ), duf_get_config_flag_act_testiflag(  ),
      duf_get_config_flag_act_testnoflag(  ) );
  QT( "@recetc: %d; i: %d; no: %d;", duf_get_config_flag_puz_testflag(  ), duf_get_config_flag_puz_testiflag(  ),
      duf_get_config_flag_puz_testnoflag(  ) );
  QT( "@disable: %d; i: %d; no: %d;", duf_get_config_flag_disable_testflag(  ), duf_get_config_flag_disable_testiflag(  ),
      duf_get_config_flag_disable_testnoflag(  ) );
  QT( "@test-num: %lu", duf_get_config_num__testnum(  ) );

  CR( main_db );

#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage /* __attribute__ ( ( weak ) ) */ ;

    if ( &mas_mem_disable_print_usage && duf_get_config_flag_disable_memusage(  ) /* DUF_CONFIGG( opt.disable.flag.memusage ) */  )
    {
      mas_mem_disable_print_usage = 1;
    }
    if ( &mas_mem_disable_print_usage && mas_mem_disable_print_usage )
    {
      MAST_TRACE( explain, 1, "@no %s option",                       /* DUF_OPT_NAME( duf_get_config_cli(  ), MEMUSAGE ), */
                  duf_get_config_flagname_disable_memusage( -1 ) );
    }
    else
    {
      MAST_TRACE( explain, 0, "@     option %s",                     /* DUF_OPT_NAME( duf_get_config_cli(  ), MEMUSAGE ), */
                  duf_get_config_flagname_disable_memusage( -1 ) );
    }
  }
#endif

  ER( TOP, main_with_config );
}

static
SRP( TOP, int, rval, 0, main, int argc MAS_UNUSED, char **argv MAS_UNUSED, char **envp MAS_UNUSED )
{

#if 0
  int dl_iterate_phdr( int ( *callback ) ( struct dl_phdr_info * info, size_t size, void *data ), void *data );
#endif

/* assert( duf_config ); */
/* raise( SIGABRT ); */
/* *( ( int * ) NULL ) = 0; */
/* mas_strdup( "abrakadabra" ); */
  MASE_E_MAX( 1, DUF_ERROR_MAX_SEQ_REACHED );
  MASE_E_MAX( 7, DUF_ERROR_NO_FIELD );
/* assert( mas_emax_count_c( DUF_ERROR_NO_FIELD ) == 7 ); */
/* raise( SIGABRT );                                                  (* To continue from here in GDB: "signal 0". *) */
/* MASE_E_LOWER_N( -10, DUF_ERROR_NO_FIELD ); */
/* mas_mset_ereport_level_c( 0, -10, 0, DUF_ERROR_NO_FIELD, 0 ); */
/* mas_set_emax_count_c( 7, DUF_ERROR_NO_FIELD ); */
/* mas_mset_ereport_level_c( 0, 10, 0, DUF_ERROR_NO_FIELD, 0 ); */
/* mas_mset_ereport_level_c( 0, 10, 1, DUF_ERROR_NO_FIELD, 0 ); */

  MAST_TRACE( explain, 1, "@main with config" );
  CR( main_with_config );

#if 0
  {
    char *p = NULL;

    fprintf( stderr, "%c\n", *p );                                   /* make segfault */
  }
#endif
  QTR;

  global_status_reset(  );
  QT( "@ptracecfg: %p; class_levels: %p", duf_get_trace_config(  ), duf_get_trace_config(  )->class_levels );
  ERRCLEAR_X( ERRCODE( MAX_REACHED ), ERRCODE( NO_ACTIONS ) );
/* assert( !duf_config ); */

/* make exit status */
  rval = !QNOERR ? 31 : 0;
  ERP( TOP, int, rval, 0, main, int argc, char **argv, char **envp );
}

/*
 * 1. set time zone
 * 2. call duf_main
 * */
int
main( int argc, char **argv, char **envp )
{
  int errc = 0;

/* mas_strdup( "abrakadabra" ); */
/* setenv( "TZ", "Europe/Kiev", 0 ); */
  tzset(  );
  return duf_main( argc, argv, envp, &errc );
}
