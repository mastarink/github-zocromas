#undef MAS_TRACING
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
#include "duf_maintenance_errors.h"

#include "duf_config_util.h"

#include "duf_option_descr.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_option_clarify.h"
/* ###################################################################### */

int
duf_xoption_clarify( const duf_longval_extended_t * extended, const char *optargg,
                     const duf_longval_extended_vtable_t * xtable, unsigned noo, duf_option_stage_t istage, duf_option_source_t source )
{
  DUF_STARTR( r );
  if ( extended )
  {
/* 
 * duf_parse_option_long return 
 *            =0  for other option
 *   errorcode<0  for error
 * */
    DUF_TRACE( options, 55, "@@@fully clarify option: [--%s='%s'] stage:%s; `noo`:%d; source:%s",
               extended ? extended->o.name : "?", optargg, duf_optstage_name( istage ), noo, duf_optsource_name( source ) );
    DOR( r, duf_xoption_clarify_typed( extended, optargg, xtable, noo, istage, source ) );
    DUF_TRACE( options, 55, "@clarified typed: --%s / [%s]; (%d:%s)", extended->o.name, duf_xoption_description_tmp( -1, extended ), r,
               mas_error_name_i( r ) );

#if 0
    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_PARSED ) && !noo )
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
  DUF_TRACE( options, +170, "@[%s]; arg:%s; istage:%d; `noo`:%d", duf_xoption_description_tmp( -1, extended ), optargg, istage, noo );
  DUF_ENDR( r );
}

#if 0
# ifdef MAS_WRAP_FUNC
int
DUF_WRAPPED( duf_xoption_clarify ) ( const duf_longval_extended_t * extended, const char *optargg,
                                     const duf_longval_extended_vtable_t * xtable, unsigned noo, duf_option_stage_t istage,
                                     duf_option_source_t source )
{
  DUF_STARTR( r );

  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_PARSED );
  DOR( r, duf_xoption_clarify( extended, optargg, xtable, noo, istage, source ) );
/* T("@noo:%d", noo); */
  DUF_TRACE( options, +150, "@@@@clarified xcmd full: xname:%s; arg:%s; noo:%d (%d:%s)", duf_xoption_description_tmp( -1, extended ), optargg, noo, r,
             mas_error_name_i( r ) );
  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_PARSED );
  DUF_ENDR( r );
}
# endif
#endif

/* 20150924.144102 */
int
duf_argv_clarify( mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos )
{
  DUF_STARTR( r );
  mas_del_argv( ptarg->argc, ptarg->argv, 0 );
  ptarg->argc = 0;
  ptarg->argv = NULL;

  ptarg->argc = mas_add_argv_argv( ptarg->argc, &ptarg->argv, pcarg->argc, pcarg->argv, pos );
  DUF_TRACE( options, 150, "@@@@@argc:%d", ptarg->argc );

/* targ.argv becomes valid here - may init pdi etc. */
  DUF_ENDR( r );
}
