#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"

#include "duf_option_help.h"


extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;

/*
void duf_list_sccb( int ) __attribute__ ( ( weak ) );
void duf_list_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
void duf_add_targ( int *targc, char ***targv, const char *s ) __attribute__ ( ( weak ) );
void duf_clear_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
*/
const duf_longval_extended_t optable_interactive[] = {

  /*
   * DO_N:	quit
   * DO_A_N:	valued: no
   * DO_OC:	set: .m_hasoff=1, .m_offset = offsetof(duf_config_t, cli.act.v)
   * DO_FL:	.afl.act={.interactive=1}
   * */
  {.o = {DO_N( quit ) /*                */ , DO_A_N} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
   /*      */ DO_OC( NOFLAG, cli.act.v ), DO_INTERACTIVE, DO_AT_STAGE( 1 ) /*              */ , DO_H( get dir info ) /*                     */ },
  {.o = {DO_N( cd ) /*                  */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
   /*      */ DO_OC( PDISTR, pdi ), DO_AT_STAGE( 1 ) /*                                    */ , DO_H( pdi cd ) /*                           */ },
  {.o = {DO_N( pwd ) /*                 */ , DO_A_N} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
   /*      */ DO_OC( PDISTR, pdi ), DO_AT_STAGE( 1 ) /*                                    */ , DO_H( pdi cd ) /*                           */ },

  {.o = {DO_N( lsfiles ) /*             */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
   /*      */ DO_OC( PDISCCB, pdi ), DO_AT_STAGE( 1 ),.call = {.fdesc = {.hi = {.sccb = &duf_print_dir_callbacks}}}, DO_H( pdi cd ) /*      */ },

  {.o = {DO_N( list-sccb ) /*           */ , DO_A_O}, DO_TF( VI_CALL, list_sccb, 0 ), DO_AT_STAGE( 1 ) /*                     */ },
  {.o = {DO_N( list-targ ) /*           */ , DO_A_O}, DO_TF( TN_CALL, list_targ ), DO_AT_STAGE( 1 ) /*                        */ },
  {.o = {DO_N( add-targ ) /*            */ , DO_A_O}, DO_TF( TS_CALL, add_targ ), DO_AT_STAGE( 1 ) /*                                      */ },
  {.o = {DO_N( clear-targ ) /*          */ , DO_A_O}, DO_T( TN_CALL ), DO_TN_CALL( clear_targ ), DO_AT_STAGE( 1 ) /*                       */ },


  {.o = {.name = NULL}}
};



/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
