/*
 * TODO :
 *   - libarchive
 *   - option as for du(1)
 *         -x, --one-file-system :      skip directories on different file systems
 *   - 
 * */

/*
  main
    duf_main
      duf_main_with_config
	duf_all_options
	duf_main_db
	  duf_main_db_open
	  duf_action
	    duf_evaluate_all_at_config
	  duf_main_db_close


  duf_option_$_list_sccbs
  duf_option_$_evaluate_sccb

  duf_option_$_list_sccb
  duf_option_$_db_open

  duf_option_$_cd
  ... ...
*/


/*

   tput reset ; run  --drop-tables /home/mastar/big/misc/media/photo/20130507/ -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress

   run  /mnt/new_media/media/photo/20130507/  -dfR --ls

*/

#include <time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config.h"

#include "duf_pdi.h"

#include "duf_options.h"
#include "duf_option_names.h"
#include "duf_option_defs.h"


#include "duf_maindb.h"


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
 *  - (DBG) ..._disable_print_usage
 * */
__attribute__ ( ( constructor( 101 ) ) )
     static void constructor_main( void )
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

__attribute__ ( ( destructor( 101 ) ) )
     static void destructor_main( void )
{
}

static int
duf_main_with_config( int argc, char **argv )
{
  DEBUG_STARTR( r );

  DUF_TRACE( any, 1, "any test" );


  DOR_NOE( r, duf_all_options(  /* argc, argv, */ DUF_OPTION_STAGE_SETUP ), DUF_ERROR_OPTION_NOT_FOUND );

#if 0
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( DUF_CONFIGG( pdi ), NULL /* real_path */ , 0 /* caninsert */ , NULL /* sql_set */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#endif


  DUF_TRACE( explain, 0, "to run duf_main_db( argc, argv )" );

  /* XXX XXX XXX XXX XXX XXX */
  DOR( r, duf_main_db( argc, argv ) ); /* duf_action (  ) + duf_main_db_close  */
  /* XXX XXX XXX XXX XXX XXX */

#if 0
  DUF_PUTS( 0, "------------------------------------(*)" );
  DUF_PRINTF( 0, "------- duf_main_db ended --------" );
  DUF_TEST_R( r );              /* don't remove! */
  DUF_PUTS( 0, "---------------------------------------------(o)" );
#endif

  if ( DUF_IF_TRACE( options ) )
    DOR( r, duf_show_options( argv[0] ) );

#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage && DUF_CONFIGG( cli.disable.flag.memusage ) )
    {
      mas_mem_disable_print_usage = 1;
    }
    if ( &mas_mem_disable_print_usage && mas_mem_disable_print_usage )
    {
      DUF_TRACE( explain, 1, "@no %s option", DUF_OPT_NAME( MEMUSAGE ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "@     option %s", DUF_OPT_NAME( MEMUSAGE ) );
    }
  }
#endif

  DEBUG_ENDR( r );
}

static int
duf_main( int argc, char **argv )
{
  DEBUG_STARTR( r );
  duf_config_create( argc, argv );

  DUF_E_MAX( 1, DUF_ERROR_MAX_SEQ_REACHED );

  DUF_TRACE( explain, 1, "@main with config" );
  DOR_NOE( r, duf_main_with_config( argc, argv ) /* XXX XXX XXX XXX */ , DUF_ERROR_OPTION_NOT_FOUND );
  if ( !DUF_NOERROR( r ) )
  {
    DUF_SHOW_ERROR( "@@@@@@@(%d:%s) ", r, duf_error_name( r ) );
    DUF_SHOW_ERROR( "@@@@@@         at %s", argv[0] );
  }

  duf_config_delete(  );

/* make exit status */
  DUF_CLEAR_ERROR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_NO_ACTIONS );
  r = !DUF_NOERROR( r ) ? 31 : 0;

  DEBUG_ENDR( r );
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
