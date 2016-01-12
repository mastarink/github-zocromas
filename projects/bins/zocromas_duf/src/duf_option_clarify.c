#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"
/* #include "duf_status_ref.h" */

#include "duf_option_find.h"
#include "duf_option_lfind.h"
#include "duf_option_descr.h"
/* #include "duf_option_extended.h" */
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
               extended ? extended->o.name : "?", optargg, duf_optstage_name( istage ), no, duf_optsource_name(source) );

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
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
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
/*
 * return 
 *               =0  for normal options
 * or errorcode (<0) for error
 * */
static int
_duf_lcoption_clarify( int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_table_t *xtable = NULL;
  int no = 0;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  DUF_TRACE( options, 50, "@to clarify option: cv:%d; li:%d; stage:%s; source:%d", codeval, longindex, duf_optstage_name( istage ), source );
  if ( longindex < 0 )
  {
    extended = duf_coption_xfind_at_stdx( codeval, &xtable, &r );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, 50, "@@@@@@@%s found (by cv) of option %d (%c) => [--%s] (%d:%s)", extended ? "" : "not", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", r, mas_error_name_i( r ) );
  }
  else if ( !extended )
  {
    extended = duf_loption_xfind_at_stdx( longindex, &xtable, &no );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, 50, "@@@@@@@found (by li) of option %d (%c) => [--%s] (%d:%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", r, mas_error_name_i( r ) );
  }
  DUF_TRACE( options, 50, "@@@@@@@found {option cv: %d (%c) li:%d} [--%s] (%d:%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, extended ? extended->o.name : "?", r, mas_error_name_i( r ) );
  if ( DUF_NOERROR( r ) )
  {
    if ( extended )
    {
      DUF_TRACE( options, 55, "@@to fully clarify option: cv:%d; li:%d; [--%s] stage:%s; source:%s", codeval, longindex,
                 extended ? extended->o.name : "?", duf_optstage_name( istage ), duf_optsource_name(source) );
      DOR( r, DUF_WRAPPED( duf_xoption_clarify ) ( extended, optargg, istage, xtable, no, source ) );
      DUF_TRACE( options, 57, "@@after fully clarify option: cv:%d; li:%d; [--%s] stage:%s; source:%d (%d:%s)", codeval, longindex,
                 extended ? extended->o.name : "?", duf_optstage_name( istage ), source, r, mas_error_name_i( r ) );
    }
    else                        /* switch is useless !... ?? */
      switch ( ( int ) codeval )
      {
      case ':':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_VALUE );
        DUF_TEST_R( r );
        DUF_TRACE( options, +150, "* options r: %d", r );
        break;
      case '?':
        DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION, "* options r: %d", r );
        DUF_TEST_R( r );
        DUF_TRACE( options, +150, "* options r: %d", r );
        break;
      default:
        DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION, "* options r: %d; codeval:%d; longindex:%d;", r, codeval, longindex );
        DUF_TEST_R( r );
        DUF_TRACE( options, +150, "* options r: %d; codeval:%d;", r, codeval );
        break;
      }
  }
  DEBUG_ENDR( r );
}

/* 20150924.144106 */
int
duf_lcoption_clarify( int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  char *oa;

  DUF_TRACE( options, 50, "@clarify option (x): cv:%-4d; li:%d; stage:%s; source:%d", codeval, longindex, duf_optstage_name( istage ), source );

  oa = duf_string_options_expand( optargg, '?' );
  DOR( r, _duf_lcoption_clarify( longindex, codeval, oa, istage, source ) ); /* => duf_xoption_clarify */
  mas_free( oa );
  DEBUG_ENDR( r );
}

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
