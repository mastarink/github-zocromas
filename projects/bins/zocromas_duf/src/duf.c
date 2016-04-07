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

   tput reset ; run  --drop-tables /home/mastar/big/misc/media/photo/20130507/ -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress

   run  /mnt/new_media/media/photo/20130507/  -dfR --ls

*/

#include <assert.h>                                                  /* assert */
/* #include <time.h> */
/* #include <signal.h> */
/* #include <unistd.h> */
/* #include <dlfcn.h> */

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
/* #include <mastar/error/mas_error_reporting.h> */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include <mastar/multiconfig/muc_se.h> */
#include <mastar/multiconfig/muc_options_all_stages.h>
/* #include <mastar/multiconfig/muc_options.h> */
#include <mastar/multiconfig/muc_option_names.h>
/* #include <mastar/multiconfig/muc_option_defs.h> */
/* #include <mastar/multiconfig/muc_option_stage.h> */
/* #include <mastar/multiconfig/muc_option_source.h> */
/* #include <mastar/multiconfig/muc_options_all_stages.h> */

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

/*
TODO Ideas: count for each dir pair number of matching files => path_pairs

INSERT OR IGNORE INTO path_pairs (samecnt, pathid1, pathid2) SELECT COUNT(*), fna.Pathid, fnb.Pathid
	FROM filenames AS fna
	JOIN filedatas AS fda ON (fna.dataid=fda.rowid)
  	JOIN md5 AS mda ON (fda.md5id=mda.rowid)
  	JOIN filedatas AS fdb ON (fdb.md5id=mda.rowid)
  	JOIN filenames AS fnb ON (fdb.rowid=fnb.dataid)
  WHERE fna.Pathid != fnb.Pathid
  GROUP BY mda.rowid;
*/
static void constructor_main( void ) __attribute__ ( ( constructor( 101 ) ) );
static void
constructor_main( void )
{
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }
  {
  /* enale debug function ... Is is obolete/useless? */
    extern int dbgfunc_enabled __attribute__ ( ( weak ) );

    if ( &dbgfunc_enabled )
       /**/ dbgfunc_enabled = 1;
  }

#endif
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

static void destructor_main( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_main( void )
{
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
SR( TOP, main_with_config, int argc, char **argv )
{
/* config-cli created at M_config_create at M_main */
  muc_CR( treat_option_stage_ne, CRX( get_config_cli ), MUC_OPTION_STAGE_DEBUG, F2ND( pdi_reinit_anypath_global ), duf_get_config_flag_act_interactive, cb_prompt_interactive ); /* here to be before following MAST_TRACE's */
  muc_CR( treat_option_stage_ne, CRX( get_config_cli ), MUC_OPTION_STAGE_BOOT, F2ND( pdi_reinit_anypath_global ), duf_get_config_flag_act_interactive,
          cb_prompt_interactive );

  MAST_TRACE( any, 1, "any test" );
  MAST_TRACE( explain, 0, "to run main_db( argc, argv )" );

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

  CR( main_db, argc, argv );

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

  ER( TOP, main_with_config, int argc, char **argv );
}

static
SRP( TOP, int, rval, 0, main, int argc, char **argv )
{
  duf_config_allocate(  );
  duf_config_create( argc, argv, 1 /* mandatory_config */  );

/* assert( duf_config ); */
/* raise( SIGABRT ); */
/* *( ( int * ) NULL ) = 0; */
/* mas_strdup( "abrakadabra" ); */
  MASE_E_MAX( 1, DUF_ERROR_MAX_SEQ_REACHED );

  MAST_TRACE( explain, 1, "@main with config" );
  CR( main_with_config, argc, argv );

  QTR;

  global_status_reset(  );
  QT( "@ptracecfg: %p; class_levels: %p", duf_get_trace_config(  ), duf_get_trace_config(  )->class_levels );
  ERRCLEAR_X( ERRCODE( MAX_REACHED ), ERRCODE( NO_ACTIONS ) );
  duf_config_delete(  );
/* assert( !duf_config ); */

/* make exit status */
  rval = !QNOERR ? 31 : 0;
  ERP( TOP, int, rval, 0, main, int argc, char **argv );
}

/*
 * 1. set time zone
 * 2. call duf_main
 * */
int
main( int argc, char **argv )
{
  int errc = 0;

/* setenv( "TZ", "Europe/Kiev", 0 ); */
  tzset(  );
  return duf_main( argc, argv, &errc );
}
