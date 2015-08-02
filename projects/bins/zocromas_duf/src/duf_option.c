#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_utils.h"

#include "duf_config_ref.h"

#include "duf_option_defs.h"
#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_typed.h"

#include "duf_option_old.h"
/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */


DUF_WRAPSTATIC int
duf_parse_option_long_full( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                            const duf_longval_extended_table_t * xtable, int no )
{
  DEBUG_STARTR( r );
  if ( extended )
  {
/* 
 * duf_parse_option_long return 
 *            =0  for other option
 *   errorcode<0  for error
 * */
    DUF_TRACE( options, 1, "xname:%s; arg:%s; no:%d", extended ? extended->o.name : "?", optargg, no );
    DOR( r, duf_parse_option_long_typed( extended, optargg, istage, xtable, no ) );
    DUF_TRACE( options, 1, "parsed typed:`%s`   %s", extended->o.name, duf_error_name( r ) );
    if ( r == DUF_ERROR_OPTION_NOT_PARSED && !no )
      DOZR( r, duf_parse_option_long_old( extended, optargg, istage, xtable ) );
    DUF_TRACE( options, 2, "cli options r: %d", r );
    DUF_TRACE( options, 1, "parsed CLI option:  %s  %s", duf_option_description_x_tmp( -1, extended, NULL ), duf_error_name( r ) );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
duf_parse_option_long_full_wrap( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                 const duf_longval_extended_table_t * xtable, int no )
{
  DEBUG_STARTR( r );

  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_PARSED );
  DOR( r, duf_parse_option_long_full( extended, optargg, istage, xtable, no ) );
  DEBUG_E_YES( DUF_ERROR_OPTION_NOT_PARSED );
  DEBUG_ENDR( r );
}
#endif
/*
 * return 
 *               =0  for normal options
 * or errorcode (<0) for error
 * */
int
duf_parse_option( duf_option_code_t codeval, int longindex, const char *optargg, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_table_t *xtable = NULL;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  DUF_TRACE( options, 1, "parse option longindex:%d", longindex );
  if ( longindex < 0 )
  {
    extended = duf_find_codeval_extended_std( codeval, &xtable, &r );
    DUF_TEST_R( r );
    DUF_TRACE( options, 0, "@@found by codeval of option %d (%c) => [--%s] (%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name( r ) );
  }
  else if ( !extended )
  {
    extended = duf_longindex2extended( longindex, &xtable, &r );
    DUF_TEST_R( r );
    DUF_TRACE( options, 0, "@@found by codeval of option %d (%c) => [--%s] (%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name( r ) );
  }
  DUF_TRACE( options, 1, "parse option codeval: %d (%c) longindex:%d [--%s] (%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, extended ? extended->o.name : "?", duf_error_name( r ) );
  if ( r >= 0 )
  {
    if ( extended )
    {
      DOR( r, DUF_WRAPPED( duf_parse_option_long_full ) ( extended, optargg, istage, xtable, 0 /* no */  ) );
    }
    else                        /* switch is useless !... */
      switch ( ( int ) codeval )
      {
      case ':':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_VALUE );
        DUF_TRACE( options, 3, "cli options r: %d", r );
        break;
      case '?':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION );
        DUF_TRACE( options, 3, "cli options r: %d", r );
        break;
      default:
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION );
        DUF_TRACE( options, 3, "cli options r: %d; codeval:%d;", r, codeval );
        break;
      }
  }
  DUF_TRACE( options, 3, "cli options r: %d; codeval:%d;", r, codeval );
  DEBUG_ENDR( r );
}
