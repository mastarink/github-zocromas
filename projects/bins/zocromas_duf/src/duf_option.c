#include <string.h>


#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* #include "duf_config_ref.h" */
#include "duf_status.h"
#include "duf_status_ref.h"

/* #include "duf_option_defs.h" */
#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_typed.h"

#include "duf_option_old.h"
/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */

DUF_WRAPSTATIC int
duf_clarify_xcmd_full( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
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
    DUF_TRACE( options, +2, "xname:%s; arg:%s; istage:%d; no:%d", extended ? extended->o.name : "?", optargg, istage, no );

/* TODO : register  extended + optargg for further use */
    global_status_register_xcmd( extended, optargg, istage, no, source );
    DOR( r, duf_clarify_xcmd_typed( extended, optargg, istage, xtable, no ) );
    DUF_TRACE( options, +3, "parsed typed:`%s`   %s", extended->o.name, duf_error_name( r ) );


    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_PARSED ) && !no )
    {
      DUF_TRACE( explain, 1, "@old opt for %s", extended ? extended->o.name : NULL );
      DOZR( r, duf_clarify_xcmd_old( extended, optargg, istage, xtable ) );
    }
    else
    {
      DUF_TRACE( explain, 1, "@no old opt for %s", extended ? extended->o.name : NULL );
    }
    DUF_TRACE( options, +4, "cli options r: %d", r );
    DUF_TRACE( options, +3, "parsed CLI option:  %s  %s", duf_option_description_x_tmp( -1, extended, NULL ), duf_error_name( r ) );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
  DUF_TRACE( options, 7, "@(%s) xname:%s; arg:%s; istage:%d; no:%d", duf_error_name( r ), extended ? extended->o.name : "?", optargg, istage, no );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
DUF_WRAPPED( duf_clarify_xcmd_full ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                       const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source )
{
  DEBUG_STARTR( r );

  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_PARSED );
  DOR( r, duf_clarify_xcmd_full( extended, optargg, istage, xtable, no, source ) );
  DUF_TRACE( options, +2, "xname:%s; arg:%s; no:%d", extended ? extended->o.name : "?", optargg, no );
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
duf_parse_exec_option( duf_option_code_t codeval, int longindex, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_table_t *xtable = NULL;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  DUF_TRACE( options, +2, "parse option longindex:%d", longindex );
  if ( longindex < 0 )
  {
    extended = duf_find_codeval_extended_std( codeval, &xtable, &r );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +2, "@@%s found by codeval of option %d (%c) => [--%s] (%s)", extended ? "" : "not", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name( r ) );
  }
  else if ( !extended )
  {
    extended = duf_longindex2extended( longindex, &xtable, &r );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +2, "@@found by codeval of option %d (%c) => [--%s] (%s)", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', extended ? extended->o.name : "?", duf_error_name( r ) );
  }
  DUF_TRACE( options, +2, "parse option codeval: %d (%c) longindex:%d [--%s] (%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, extended ? extended->o.name : "?", duf_error_name( r ) );
  if ( DUF_NOERROR( r ) )
  {
    if ( extended )
    {
      DOR( r, DUF_WRAPPED( duf_clarify_xcmd_full ) ( extended, optargg, istage, xtable, 0 /* no */ , source ) );
    }
    else                        /* switch is useless !... */
      switch ( ( int ) codeval )
      {
      case ':':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_VALUE );
        DUF_TEST_R( r );
        DUF_TRACE( options, +0, "* options r: %d", r );
        break;
      case '?':
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION );
        DUF_TEST_R( r );
        DUF_TRACE( options, +0, "* options r: %d", r );
        break;
      default:
        DUF_MAKE_ERROR( r, DUF_ERROR_OPTION );
        DUF_TEST_R( r );
        DUF_TRACE( options, +0, "* options r: %d; codeval:%d;", r, codeval );
        break;
      }
  }
  DEBUG_ENDR( r );
}
