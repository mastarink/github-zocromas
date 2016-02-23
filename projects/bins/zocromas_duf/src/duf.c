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

  duf_option_O_list_sccbs
  duf_option_O_evaluate_sccb

  duf_option_O_list_sccb
  duf_option_O_db_open

  duf_option_O_cd
  ... ...
*/

/*

   tput reset ; run  --drop-tables /home/mastar/big/misc/media/photo/20130507/ -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress

   run  /mnt/new_media/media/photo/20130507/  -dfR --ls

*/

#include <time.h>
/* #include <signal.h> */
/* #include <unistd.h> */
#include <dlfcn.h>

#include "duf_maintenance.h"

#include "duf_status_ref.h"
#include "duf_status.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_config_credel.h"

#include "duf_options.h"
#include "duf_option_names.h"
#include "duf_option_defs.h"

#include <mastar/error/mas_error_reporting.h>

#include "duf_option_stage.h"
#include "duf_option_source.h"
#include "duf_options_all_stages.h"

#include "duf_levinfo_ref.h"

#include "duf_maindb.h"
#include "duf_pdi_global.h"

/* צאַצקע */
#include "duf_experiment.h"

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

#if 1
int
duf_verbose( void )
{
  return duf_config ? duf_config->opt.flow.verbose : 0;
}

int
duf_dry_run( void )
{
  return duf_config ? duf_config->opt.flow.v.flag.dry_run : 0;
}

#endif

static void destructor_main( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_main( void )
{
}

static int
cb_do_interactive( void )
{
  return DUF_ACTG_FLAG( interactive );
}

static const char *
cb_prompt_interactive( void )
{
  static char rl_prompt[256 * 10] = "";

  snprintf( rl_prompt, sizeof( rl_prompt ), "A-F:%d;A-D:%d; %s:%s> ", DUF_ACTG_FLAG( allow_files ), DUF_ACTG_FLAG( allow_dirs ), "db",
            duf_levinfo_path( duf_pdi_global(  ) ) );
  return rl_prompt;
}

static
SR( TOP, main_with_config, int argc, char **argv )
{
  CR( treat_option_stage_ne, duf_get_config_cli(  ), DUF_OPTION_STAGE_DEBUG, duf_pdi_reinit_anypath_global, cb_do_interactive, cb_prompt_interactive ); /* here to be before following DUF_TRACE's */
  CR( treat_option_stage_ne, duf_get_config_cli(  ), DUF_OPTION_STAGE_BOOT, duf_pdi_reinit_anypath_global, cb_do_interactive, cb_prompt_interactive );

  DUF_TRACE( any, 1, "any test" );
  DUF_TRACE( explain, 0, "to run main_db( argc, argv )" );

  CR( treat_all_optstages, duf_get_config_cli(  ), duf_pdi_create_global, duf_pdi_reinit_anypath_global, cb_do_interactive, cb_prompt_interactive );
  fputs( "\n", stderr );
  DUF_TRACE( temp, 0, "∈0∋ Zero " );
  TT( "∈1∋ One " );
  TT( "∈2∋ Two " );
  TT( "∈1∋ Three " );
  TT( "∈3 Four " );
  TT( "∈1∋ Five " );
  TT( "∈4∋ Six " );
  TT( "∈1∋ Seven " );
  TT( "∈6 Eight " );
  TT( "∈1∋ Nine " );
  TT( "∈7∋ Ten " );
  TT( "∈8∋ -= !! Split config : duf-related and opt/cfg+i/o related !! =- " );
  T( "@act: %d; i: %d; no: %d;", DUF_ACTG_FLAG( testflag ), DUF_ACTG_FLAG( testiflag ), DUF_ACTG_FLAG( testnoflag ) );
  T( "@recetc: %d; i: %d; no: %d;", DUF_UG_FLAG( testflag ), DUF_UG_FLAG( testiflag ), DUF_UG_FLAG( testnoflag ) );
  T( "@disable: %d; i: %d; no: %d;", DUF_CONFIGG( opt.disable.flag.testflag ), DUF_CONFIGG( opt.disable.flag.testiflag ),
     DUF_CONFIGG( opt.disable.flag.testnoflag ) );
  T( "@test-num: %lu", DUF_CONFIGG( testnum ) );

//          opt   disable  testnoflag
#define SFLAG_SET(_styp, _prf, _loc, _fld )  ((duf_ ## _styp ## _ ## _prf ## _ ## _loc ## _flags_combo_t) {.flag._fld = 1 }).sbit
  {
    unsigned long long t1 DUF_UNUSED = ( unsigned long long ) SFLAG_SET( config, opt, disable, testnoflag );
    unsigned long long t2 DUF_UNUSED = ( unsigned long long ) ( ( duf_option_anyflag_t ) {.disable.testnoflag = 1 } ).sbit;
    T( "@>>>> %llx : %llx <<<<", t1, t2 );
  }

  CR( main_db, argc, argv );

/* XXX XXX XXX XXX XXX XXX */

#if 0
  DUF_PUTS( 0, "------------------------------------(*)" );
  DUF_PRINTF( 0, "------- main_db ended --------" );
  DUF_TEST_R( r );                                                   /* don't remove! */
  DUF_PUTS( 0, "---------------------------------------------(o)" );
#endif

#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage /* __attribute__ ( ( weak ) ) */ ;

    if ( &mas_mem_disable_print_usage && DUF_CONFIGG( opt.disable.flag.memusage ) )
    {
      mas_mem_disable_print_usage = 1;
    }
    if ( &mas_mem_disable_print_usage && mas_mem_disable_print_usage )
    {
      DUF_TRACE( explain, 1, "@no %s option", DUF_OPT_NAME( duf_get_config_cli(  ), MEMUSAGE ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "@     option %s", DUF_OPT_NAME( duf_get_config_cli(  ), MEMUSAGE ) );
    }
  }
#endif

  ER( TOP, main_with_config, int argc, char **argv );
}

static int
duf_main( int argc, char **argv )
{
  DUF_STARTR( r );
/* fprintf(stderr, "◀"  ); */
  duf_config_create( argc, argv );

  assert( duf_config );
/* raise( SIGABRT ); */
/* *( ( int * ) NULL ) = 0; */
/* mas_strdup( "abrakadabra" ); */
  DUF_E_MAX( 1, DUF_ERROR_MAX_SEQ_REACHED );

  DUF_TRACE( explain, 1, "@main with config" );
#if 0
  DOR_LOWERE( r, duf_main_with_config( argc, argv ) /* XXX XXX XXX XXX */ , DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_OPTION_NEW_NOT_FOUND );
#else
  DORF( r, duf_main_with_config, argc, argv );
#endif
#if 0
  if ( DUF_IS_ERROR( r ) )
  {
    _DUF_SHOW_ERROR( "@@@@@@@@(i:%d;c:%d:%s) %s (%s:%d)", r, mas_error_code_i( r ), mas_error_name_i( r ), mas_error_message_i( r ),
                     mas_error_func_i( r ), mas_error_line_i( r ) );
    _DUF_SHOW_ERROR( "@@@@@@@@@         at %s", argv[0] );
  }
#endif

  TR( r );
#if 0
  {
    FILE *f = NULL;

    f = fopen( "options.tmp", "w" );
    if ( f )
    {
      duf_option_stage_t stage = DUF_OPTION_STAGE_NONE;
      duf_option_source_t source = {.sourcecode = DUF_OPTION_SOURCE_NONE };

      for ( size_t iod = 0; iod < global_status.aod.count; iod++ )
      {
        duf_option_data_t *pod;

        pod = &global_status.aod.pods[iod];
      /* T( "%lu. %s.pod %s => %s", iod, duf_optstage_name( pod->stage ), duf_optsource_name( pod->source ), pod->name ); */
        if ( source.sourcecode != pod->source.sourcecode )
          fprintf( f, "* SOURCE %s\n", duf_optsource_name( source = pod->source ) );
        if ( stage != pod->stage )
          fprintf( f, "* STAGE %s\n", duf_optstage_name( stage = pod->stage ) );
        if ( pod->doindex >= 0 )
        {
          fprintf( f, "\t%c(%2ld) %lu. --%s", ( pod->clarified[stage] ? '+' : ' ' ), pod->doindex, iod,
                   pod->xfound.xarray[pod->doindex].xtended->o.name );
          if ( pod->optarg )
            fprintf( f, "='%s'", pod->optarg );
        }
        fprintf( f, "\t\t[%c(%2ld) %lu. --%s", ( pod->clarified[stage] ? '+' : ' ' ), pod->doindex, iod, pod->name );
        if ( pod->optarg )
          fprintf( f, "='%s'", pod->optarg );
        fprintf( f, "]\n" );
      }
      fclose( f );
    }
  }
#endif
  global_status_reset(  );
  duf_config_delete(  );
  assert( !duf_config );

/* make exit status */
  DUF_CLEAR_ERROR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_NO_ACTIONS );
  r = !DUF_NOERROR( r ) ? 31 : 0;
/* fprintf(stderr, "▶"  ); */
/* T( "@@@@%d %d %d -- %ld", DUF_SQL_ERROR, DUF_ERROR_ERROR_MAX, DUF_SQL_ERROR < DUF_ERROR_ERROR_MAX, mas_error_list_size(  ) ); */
/* sleep( 3 ); */
  DUF_ENDR( r );
}

/*
 * 1. set time zone
 * 2. call duf_main
 * */
int
main( int argc, char **argv )
{
/* setenv( "TZ", "Europe/Kiev", 0 ); */
  tzset(  );
  return duf_main( argc, argv );
}
