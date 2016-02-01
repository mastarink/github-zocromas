#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"

#include "duf_config.h"

#include "duf_option_find.h"
#include "duf_option_cfind.h"
#include "duf_option_lfind.h"

#include "duf_option_descr.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_option_clarify.h"
/* ###################################################################### */

DUF_WRAPSTATIC int
duf_xoption_clarify( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                     const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  if ( extended )
  {
/* 
 * duf_parse_option_long return 
 *            =0  for other option
 *   errorcode<0  for error
 * */
    DUF_TRACE( options, 55, "@@@fully clarify option: [--%s='%s'] stage:%s; `no`:%d; source:%s",
               extended ? extended->o.name : "?", optargg, duf_optstage_name( istage ), no, duf_optsource_name( source ) );
/* TODO : register  extended + optargg for further use */
    global_status_register_xcmd( extended, optargg, istage, no, source );
    DOR( r, duf_xoption_clarify_typed( extended, optargg, istage, xtable, no, source ) );
    DUF_TRACE( options, 55, "@clarified typed: --%s / [%s]; (%d:%s)", extended->o.name, duf_xoption_description_tmp( -1, extended ), r,
               mas_error_name_i( r ) );

#if 0
    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_PARSED ) && !no )
    {
      assert( 0 );
      DUF_TRACE( explain, 1, "@old opt for %s", extended ? extended->o.name : NULL );
      DOZR( r, duf_clarify_xcmd_old( extended, optargg, istage, xtable ) );
    }
#endif
    DUF_TRACE( options, 55, "@clarified cli option: %s (%d:%s)", duf_xoption_description_tmp( -1, extended ), r, mas_error_name_i( r ) );
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( 0 );
  }
  DUF_TRACE( options, +170, "@[%s]; arg:%s; istage:%d; `no`:%d", duf_xoption_description_tmp( -1, extended ), optargg, istage, no );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
DUF_WRAPPED( duf_xoption_clarify ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                     const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
{
  DEBUG_STARTR( r );

  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_PARSED );
  DOR( r, duf_xoption_clarify( extended, optargg, istage, xtable, no, source ) );
  DUF_TRACE( options, +150, "@@@@clarified xcmd full: xname:%s; arg:%s; no:%d (%d:%s)", duf_xoption_description_tmp( -1, extended ), optargg, no, r,
             mas_error_name_i( r ) );
  DEBUG_E_UPPER( DUF_ERROR_OPTION_NOT_PARSED );
  DEBUG_ENDR( r );
}
#endif

/* 20150924.144102 */
int
duf_argv_clarify( mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos )
{
  DEBUG_STARTR( r );
  mas_del_argv( ptarg->argc, ptarg->argv, 0 );
  ptarg->argc = 0;
  ptarg->argv = NULL;

  ptarg->argc = mas_add_argv_argv( ptarg->argc, &ptarg->argv, pcarg->argc, pcarg->argv, pos );
  DUF_TRACE( options, 150, "@@@@@argc:%d", ptarg->argc );

  /* targ.argv becomes valid here - may init pdi etc. */
  DEBUG_ENDR( r );
}
