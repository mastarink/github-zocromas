#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_config_ref.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_cli_options.h"
#include "duf_options.h"

#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_action.h"
#include "duf_maindb.h"

/* צאַצקע */
#include "duf_trace_defs.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

static int
duf_test_help( int argc, char **argv, duf_option_code_t codeval )
{
  int r = -1;

  duf_option_class_t oclass = DUF_OPTION_CLASS_BAD;


  oclass = duf_help_option2class( codeval );
  /* DUF_PRINTF( 0, "%d / %c => OC:%d (?%d)", opt, opt > ' ' && opt <= 'z' ? opt : '?', oclass, DUF_OPTION_CLASS_ALL ); */
  if ( oclass == DUF_OPTION_CLASS_ALL )
  {
    for ( duf_option_class_t oc = DUF_OPTION_CLASS_MIN + 1; oc < DUF_OPTION_CLASS_MAX; oc++ )
      duf_option_smart_help( oc );
    r = 0;
  }
  else if (  /* oclass != DUF_OPTION_CLASS_NO_HELP && */ oclass != DUF_OPTION_CLASS_BAD )
  {
    duf_option_smart_help( oclass );
    r = 0;
  }
  else
    switch ( codeval )
    {
    case DUF_OPTION_VERSION:
      duf_option_version( argc, argv );
      r = 0;
      break;
    case DUF_OPTION_HELP:
      duf_option_help( argc, argv );
      r = 0;
      break;
    case DUF_OPTION_EXAMPLES:
      duf_option_examples( argc, argv );
      r = 0;
      break;
    default:
      break;
    }
  return r;
}

unsigned long
duf_config_act_flags_combo_t_bits( duf_config_act_flags_t f )
{
  duf_config_act_flags_combo_t c;

  c.flag = f;
  return c.bit;
}

unsigned long long static int
duf_main( int argc, char **argv )
{
  int r = 0;

  DUF_PRINTF( 0, "************************* %lu %lu %lu %lu %lu %lu %lu", sizeof( duf_config_act_flags_t ), sizeof( duf_config_cli_flags_t ),
              sizeof( duf_ufilter_flags_t ), sizeof( duf_config_cli_disable_flags_t ), sizeof( unsigned ), sizeof( unsigned long ),
              sizeof( unsigned long long ) );

  /* DUF_TRACE( any, 0, "r=%d", r ); */
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 7;
    }
  }
  r = duf_config_create(  );
  /* duf_config->cli.trace.explain = 1; */
  /* DUF_TRACE( any, 0, "r=%d", r ); */
  if ( r >= 0 )
  {
    int er = 0, fr = 0, or = 0;

    DUF_TRACE( any, 1, "any test" );

    if ( r >= 0 )
      er = r = duf_env_options( argc, argv );
    DUF_TRACE( explain, 0, "got env options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );

    if ( r >= 0 )
      fr = r = duf_infile_options( argc, argv );
    DUF_TRACE( explain, 0, "got infile options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );

    /* duf_config->cli.dbg.verbose = 4; */

    if ( r >= 0 )
      or = r = duf_cli_options( argc, argv );
    DUF_TRACE( explain, 0, "got cli options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );

    if ( r >= 0 && duf_config->cli.trace.options )
      r = duf_show_options( argv[0] );
    /* {                                 */
    /*   char c;                         */
    /*   char *s = NULL;                 */
    /*                                   */
    /*   c = *s;                         */
    /*   fprintf( stderr, "%c\n", c );   */
    /*   DUF_TRACE( any, 0, "r=%d", r ); */
    /* }                                 */
    {
      extern int dbgfunc_enabled __attribute__ ( ( weak ) );

      if ( &dbgfunc_enabled )
         /**/ dbgfunc_enabled = 1;
    }

    DUF_TEST_RN( r );
    DUF_TRACE( explain, 2, "or: %d; fr: %d; er: %d; r: %d", or, fr, er, r );
    if ( duf_test_help( argc, argv, or ) < 0 && duf_test_help( argc, argv, fr ) < 0 && duf_test_help( argc, argv, er ) < 0 )
    {
      if ( r == 0 && duf_config && duf_config->db.dir )
      {
        DUF_TRACE( explain, 0, "to run main_db( argc, argv )" );
        r = main_db( argc, argv );
        DUF_TEST_R( r );
      }
      else if ( r > 0 )
      {
        DUF_TRACE( explain, 1, "or: %d; fr: %d; er: %d; r: %d", or, fr, er, r );
        /* r=0; */
      }
      DUF_PUTS( 0, "--------------------------------------------------" );
      DUF_PRINTF( 0, " main_db ended                                                       [%s] (#%d)", duf_error_name( r ), r );
      DUF_PUTS( 0, "--------------------------------------------------" );
    }
    else
    {
      r = 0;
    }
    DUF_TEST_R( r );
    duf_config_delete(  );
  }
  else
  {
    DUF_ERROR( "(%d) %s", r, argv[0] );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );

  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage && mas_mem_disable_print_usage )
    {
      DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( MEMUSAGE ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( MEMUSAGE ) );
    }
  }


#endif
  DUF_ERRORR( r, "-" );
  return r < 0 && r != DUF_ERROR_MAX_REACHED ? 31 : 0;
}

int
main( int argc, char **argv )
{
  return duf_main( argc, argv );
}
