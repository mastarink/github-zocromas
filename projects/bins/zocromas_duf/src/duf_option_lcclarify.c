#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"

#include "duf_option_find.h"
#include "duf_option_cfind.h"
#include "duf_option_lfind.h"

#include "duf_option_descr.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

#include "duf_option_clarify.h"
/* ###################################################################### */
#include "duf_option_lcclarify.h"
/* ###################################################################### */

#if 0 /* not used with new options system , see duf_option_clarify_new.c */

/*
 * return 
 *               =0  for normal options
 * or errorcode (<0) for error
 * */
static
SR( OPTIONS, lcoption_clarify_expanded, int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage,
    duf_option_source_t source )
{
  /* DEBUG_STARTR( r ); */
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_table_t *xtable = NULL;
  int no = 0;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  DUF_TRACE( options, 50, "@to clarify option: cv:%d; li:%d; stage:%s; source:%s", codeval, longindex, duf_optstage_name( istage ),
             duf_optsource_name( source ) );
  if ( longindex < 0 )
  {
    extended = duf_coption_xfind_at_stdx( codeval, &xtable, QPERRIND );
    DUF_TRACE( options, 50, "@@@@@@@%s found (by cv) of option %d (%c) => [--%s] (%d:%s)", extended ? "" : "not", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", QERRIND, QERRNAME );
  }
  else if ( !extended )
  {
    extended = duf_loption_xfind_at_stdx( longindex, &xtable, &no );
    DUF_TRACE( options, 50, "@@@@@@@found (by li) of option %d (%c) => [--%s] (%d:%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", QERRIND, QERRNAME );
  }
  DUF_TRACE( options, 50, "@@@@@@@found {option cv: %d (%c) li:%d} [--%s] (%d:%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, extended ? extended->o.name : "?", QERRIND, QERRNAME );
  if ( QNOERR )
  {
    if ( extended )
    {
      DUF_TRACE( options, 55, "@@to fully clarify option: cv:%d; li:%d; [--%s] stage:%s; source:%s", codeval, longindex,
                 extended ? extended->o.name : "?", duf_optstage_name( istage ), duf_optsource_name( source ) );
#if 0
      DOR( QERRIND, DUF_WRAPPED( duf_xoption_clarify ) ( extended, optargg, istage, xtable, no, source ) );
#else
      CRW( xoption_clarify, extended, optargg, istage, xtable, no, source );
#endif
      DUF_TRACE( options, 57, "@@after fully clarify option: cv:%d; li:%d; [--%s] stage:%s; source:%s (%d:%s)", codeval, longindex,
                 extended ? extended->o.name : "?", duf_optstage_name( istage ), duf_optsource_name( source ), QERRIND, QERRNAME );
    }
    else                        /* switch is useless !... ?? */
      switch ( ( int ) codeval )
      {
      case ':':
        DUF_MAKE_ERROR( QERRIND, DUF_ERROR_OPTION_VALUE );
        DUF_TEST_R( QERRIND );
        DUF_TRACE( options, +150, "* options r: %d", QERRIND );
        break;
      case '?':
        DUF_MAKE_ERRORM( QERRIND, DUF_ERROR_OPTION, "* options r: %d", QERRIND );
        DUF_TEST_R( QERRIND );
        DUF_TRACE( options, +150, "* options r: %d", QERRIND );
        break;
      default:
        DUF_MAKE_ERRORM( QERRIND, DUF_ERROR_OPTION, "* options r: %d; codeval:%d; longindex:%d;", QERRIND, codeval, longindex );
        DUF_TEST_R( QERRIND );
        DUF_TRACE( options, +150, "* options r: %d; codeval:%d;", QERRIND, codeval );
        break;
      }
  }
  /* DEBUG_ENDR( r ); */
  ER( OPTIONS, lcoption_clarify_expanded, int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage,
      duf_option_source_t source );
}

/* 20150924.144106 */
SR( OPTIONS, lcoption_clarify, int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  /* DEBUG_STARTR( r ); */
  char *oa;

  DUF_TRACE( options, 50, "@clarify option (x): cv:%-4d; li:%d; stage:%s; source:%s", codeval, longindex, duf_optstage_name( istage ),
             duf_optsource_name( source ) );

  oa = duf_string_options_expand( optargg, '?' );
  CR( lcoption_clarify_expanded, longindex, codeval, oa, istage, source ); /* => duf_xoption_clarify */
  mas_free( oa );
  /* DEBUG_ENDR( r ); */
  ER( OPTIONS, lcoption_clarify, int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage,
      duf_option_source_t source );
}
#endif
