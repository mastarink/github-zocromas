/* File 20140902.123820 */
#include <string.h>
#include <time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config.h"
#include "duf_utils.h"


#include "duf_cli_options.h"
#include "duf_options.h"
#include "duf_option_names.h"

#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_action.h"
#include "duf_maindb.h"

#include "duf_pdi.h"

/* #include "duf_path2db.h"        (* test only *) */

/* צאַצקע */
#include "duf_trace_defs.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

/*
Ideas: count for each dir pair number of matching files => path_pairs

INSERT OR IGNORE INTO path_pairs (samecnt, pathid1, pathid2) SELECT count(*), fna.Pathid, fnb.Pathid
	FROM filenames AS fna
	JOIN filedatas AS fda ON (fna.dataid=fda.rowid)
  	JOIN md5 AS mda ON (fda.md5id=mda.rowid)
  	JOIN filedatas AS fdb ON (fdb.md5id=mda.rowid)
  	JOIN filenames AS fnb ON (fdb.rowid=fnb.dataid)
  WHERE fna.Pathid != fnb.Pathid
  GROUP BY mda.rowid;
*/

unsigned long
duf_config_act_flags_combo_t_bits( duf_config_act_flags_t f )
{
  duf_config_act_flags_combo_t c;

  c.flag = f;
  return c.bit;
}

static int
duf_main( int argc, char **argv )
{
  DEBUG_STARTR( r );

  PF0( "%lu %lu %lu %lu %lu %lu %lu %lu", sizeof( duf_limits_t ), sizeof( duf_config_act_flags_t ),
       sizeof( duf_config_cli_flags_t ), sizeof( duf_ufilter_flags_t ), sizeof( duf_config_cli_disable_flags_t ), sizeof( unsigned ),
       sizeof( unsigned long ), sizeof( unsigned long long ) );
  /* DUF_TRACE( any, 0, "r=%d", r ); */
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 7;
    }
  }
  DOR( r, duf_config_create(  ) );
  {
    extern int dbgfunc_enabled __attribute__ ( ( weak ) );

    if ( &dbgfunc_enabled )
       /**/ dbgfunc_enabled = 1;
  }
  /* duf_config->cli.trace.explain = 1; */
  /* DUF_TRACE( any, 0, "r=%d", r ); */
  if ( r >= 0 )
  {
    DUF_TRACE( any, 1, "any test" );
    /* duf_config->cli.interstage_init = duf_interstage_init; (* ????? *) */

    assert( duf_config->targc == 0 );
    DEBUG_E_NO( DUF_ERROR_OPTION );
    DOR( r, duf_all_options( argc, argv ) );
    DUF_E_YES( DUF_ERROR_OPTION );
    /* assert( duf_config->targc > 0 ); */



    if ( r >= 0 )
    {
      DUF_TRACE( explain, 0, "to run main_db( argc, argv )" );
      DOR( r, main_db( argc, argv ) );
    }

    DUF_PUTS( 0, "--------------------------------------------------" );
    DUF_PRINTF( 0, " main_db ended" );
    DUF_TEST_R( r );            /* don't remove! */
    DUF_PUTS( 0, "--------------------------------------------------" );

    if ( r >= 0 && DUF_IF_TRACE( options ) )
      DOR( r, duf_show_options( argv[0] ) );
    /* {                                 */
    /*   char c;                         */
    /*   char *s = NULL;                 */
    /*                                   */
    /*   c = *s;                         */
    /*   fprintf( stderr, "%c\n", c );   */
    /*   DUF_TRACE( any, 0, "r=%d", r ); */
    /* }                                 */

    /* DUF_TEST_RN( r ); */

    duf_config_delete(  );
  }
  else
  {
    DUF_SHOW_ERROR( "(%d) %s", r, argv[0] );
  }
#ifdef MAS_TRACEMEM

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
  print_memlist( FL, stdout );


#endif
/* make exit status */
  DUF_CLEAR_ERROR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_NO_ACTIONS );
  r = r < 0 ? 31 : 0;
  DEBUG_ENDRN( r );
}

int
main( int argc, char **argv )
{
  /* setenv( "TZ", "Europe/Kiev", 0 ); */
  tzset(  );
  return duf_main( argc, argv );
}
