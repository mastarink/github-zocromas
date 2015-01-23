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

/*
 * do everything (almost :)
 * *************
 *  1. (DBG) disable_print_usage
 *  2. create config struct (global variable: duf_config)
 *  3. call duf_all_options
 *  4. call duf_main_db
 *  5. (optional) call duf_show_options (see also duf_config_show call at duf_main_db)
 *  6. delete config struct
 *  7. final error process and set exit code
 * */
static int
duf_main( int argc, char **argv )
{
  DEBUG_STARTR( r );
/* output some useful(?) for development & debugging info at the beginning */
  PF0( "%lu %lu %lu %lu %lu %lu %lu %lu", sizeof( duf_limits_t ), sizeof( duf_config_act_flags_t ),
       sizeof( duf_config_cli_flags_t ), sizeof( duf_ufilter_flags_t ), sizeof( duf_config_cli_disable_flags_t ), sizeof( unsigned ),
       sizeof( unsigned long ), sizeof( unsigned long long ) );
  /* DUF_TRACE( any, 0, "r=%d", r ); */

  {
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 7;
    }
  }
  duf_config4trace = duf_config = duf_config_create(  );
  assert( duf_config );
  assert( duf_config4trace );
  assert( duf_config->pu );
  assert( duf_config->longopts_table );
  {
    /* enale debug function ... Is is obolete/useless? */
    extern int dbgfunc_enabled __attribute__ ( ( weak ) );

    if ( &dbgfunc_enabled )
       /**/ dbgfunc_enabled = 1;
  }
  if ( r >= 0 )
  {
    DUF_TRACE( any, 1, "any test" );
    DOR_NOE( r, duf_all_options( argc, argv ), DUF_ERROR_OPTION_NOT_FOUND );
    DUF_TRACE( explain, 0, "to run duf_main_db( argc, argv )" );
    DOR( r, duf_main_db( argc, argv ) );

    DUF_PUTS( 0, "--------------------------------------------------" );
    DUF_PRINTF( 0, " duf_main_db ended" );
    DUF_TEST_R( r );            /* don't remove! */
    DUF_PUTS( 0, "--------------------------------------------------" );

    if ( DUF_IF_TRACE( options ) )
      DOR( r, duf_show_options( argv[0] ) );
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
  duf_config_delete( duf_config );
  duf_config4trace = duf_config = NULL;


#endif
/* make exit status */
  DUF_CLEAR_ERROR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_NO_ACTIONS );
  r = r < 0 ? 31 : 0;
  DEBUG_ENDRN( r );
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
