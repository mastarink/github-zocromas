#include <string.h>
#include <assert.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_utils_path.h"


#include "duf_options_cli.h"
#include "duf_options_interactive.h"
#include "duf_options_file.h"

#include "duf_option_names.h"
#include "duf_option_defs.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_eval_at_config.h"

#include "duf_levinfo_ref.h"



#include "duf_pdi.h"
#include "duf_sql_defs.h"
#include "duf_begfin.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */

/*
 * TODO join duf_action and duf_all_options
 *
 *
 * duf_action								| duf_all_options
 * with istage = DUF_OPTION_STAGE_FIRST					| with istage
 * 									|  
 * 									|
 * 									|  - duf_env_options
 * 									|  - duf_infile_options
 *   - duf_cli_options( istage )					|  - duf_cli_options
 *   - duf_stdin_options( istage )					|
 *   - duf_indirect_options( istage )					|  - duf_indirect_options
 *   - duf_interactive_options( DUF_OPTION_STAGE_INTERACTIVE )		|
 * */

#if 0
DUF_WRAPSTATIC int
duf_action( void )
{
  DEBUG_STARTR( r );


  DUF_E_SET( -96, DUF_ERROR_NO_ACTIONS );
  /* DUF_E_SET( 97, DUF_ERROR_TOO_DEEP, DUF_ERROR_NOT_IN_DB, (* DUF_ERROR_MAX_SEQ_REACHED, *) DUF_ERROR_MAX_REACHED ); */

  DUF_TRACE( path, 0, "@@path@pdi: %s", duf_levinfo_path( duf_config->pdi ) );
  {
    int ia = duf_config->targ_offset;
    duf_option_stage_t istage = DUF_OPTION_STAGE_FIRST;



    /* XXX XXX see duf_option_$_evaluate_sccb for "main events" XXX XXX ( through --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif ) */

    DUF_TRACE( pdi, 1, "@@@@c->pdi inited:%d; duf_levinfo_path( duf_config->pdi ):%s", duf_config->pdi->inited, duf_levinfo_path( duf_config->pdi ) );
    if ( duf_levinfo_path( duf_config->pdi ) )
    {
      DOR( r, duf_cli_options( istage ) );
#if 1
      /* TODO this will not work for path except first */
      DUF_TRACE( options, 0, "@stage_first stdin_options" );
      DOR( r, duf_stdin_options( istage ) );
#endif
      DOR( r, duf_indirect_options( istage ) );
    }
 

    DOR( r, duf_interactive_options( DUF_OPTION_STAGE_INTERACTIVE ) );
    while ( DUF_NOERROR( r ) && ia < duf_config->targ.argc )
    {
      istage = DUF_OPTION_STAGE_LOOP;
      DUF_TRACE( pdi, 1, "@@@c->pdi inited:%d; targ.argv[%d]:%s", duf_config->pdi->inited, ia, duf_config->targ.argv[ia] );
      DOR( r, duf_pdi_reinit_anypath( duf_config->pdi, duf_config->targ.argv[ia], 7 /* caninsert */ ,
                                      NULL /* node_selector2 */ , DUF_UG_FLAG( recursive ) ) );
      if ( duf_levinfo_path( duf_config->pdi ) )
      {
        DOR( r, duf_cli_options( istage ) );
#if 1
        /* TODO this will not work for path except first */
        DUF_TRACE( options, 0, "@stage_first stdin_options" );
        DOR( r, duf_stdin_options( istage ) );
#endif
        DOR( r, duf_indirect_options( istage ) );
      }
      ia++;
    }
    DUF_TRACE( path, 0, "@r:%d; %d ? %d; path@pdi: %s", r, ia, duf_config->targ.argc, duf_levinfo_path( duf_config->pdi ) );
  }
  /* assert( duf_config->pdi->levinfo ); */

  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  DUF_TRACE( explain, 0, "after actions" );

  DEBUG_ENDR( r );
}


#ifdef MAS_WRAP_FUNC
int
duf_action_wrap( void /* int argc, char **argv */  )
{
  DEBUG_STARTR( r );

  /* assert( !duf_levinfo_path( duf_config->pdi ) ); */
  DORF( r, duf_action /* , argc, argv */  ); /* duf_action : duf_action.c */
#  if 0
  DORF( r, duf_main_db_info );
#  endif
  if ( r < 0 && r != DUF_ERROR_MAX_REACHED )
  {
    DUF_TEST_RX_SHOW( r, "action FAIL" );
  }

  DEBUG_ENDR( r );
}
#endif
#endif
