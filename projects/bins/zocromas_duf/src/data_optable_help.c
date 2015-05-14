#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"

#include "duf_option_help.h"
const duf_longval_extended_t optable_help[] = {
  {.o = {DO_Q( "version" ) /*           */ , DO_A_N /* */ } /*                             */ , DO_CL( REFERENCE ) /*  */ ,
   /*      */ DO_T( A_CALL ), DO_A_CALL( version ) /*     */ , DO_STAGE_SAME( 1 ) /*          */ , DO_H( version info ) /*                   */ },
  {.o = {DO_Q( "flags" ) /*             */ , DO_A_N /* */ } /*                             */ , DO_CL( REFERENCE ) /*  */ ,
   /*      */ DO_T( A_CALL ), DO_A_CALL( showflags ) /*   */ , DO_STAGE_SAME( 1 ) /*          */ , DO_H( version info ) /*                   */ },
  {.o = {DO_Q( "help-examples" ) /*     */ , DO_A_N /* */ , DO_V( EXAMPLES )} /*            */ , DO_CL( HELP ) /*      */ ,
   /*      */ DO_T( A_CALL ), DO_A_CALL( examples ) /*                                        */ , DO_H( examples ) /*                       */ },

  {.o = {DO_Q( "help-all" ) /*          */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALL( smart_help_all, DUF_OPTION_CLASS_ALL ) /*        */ , DO_H( help on all ) /*                      */ },
  {.o = {DO_Q( "help-collect" ) /*      */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( COLLECT ) /*                                    */ , DO_H( help on collect ) /*                  */ },

  {.o = {DO_Q( "help-control" ) /*      */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( CONTROL ) /*                                    */ , DO_H( help on control ) /*                  */ },
  {.o = {DO_Q( "help-debug" ) /*        */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( DEBUG ) /*                                      */ , DO_H( help on debug ) /*                    */ },

  {.o = {DO_Q( "help-help" ) /*         */ , DO_A_N, DO_V( SMART_HELP )} /*                */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( HELP ) /*                                       */ , DO_H( help on help ) /*                     */ },
  {.o = {DO_Q( "help-short" ) /*        */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( A_CALL ), DO_A_CALL( help ) /*                                          */ , DO_H( help ) /*                             */ },
  /* {.o = {DO_Q( "help" ), (*        *) DO_A_O} (* *) , DO_CL( HELP ) (*      *) , DO_H( smart help ) (*         *) }, */


  {.o = {DO_Q( "help-nodesc" ) /*       */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( NODESC ) /*                                     */ , DO_H( help on nodesc ) /*                   */ },
  {.o = {DO_Q( "help-obsolete" ) /*     */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( OBSOLETE ) /*                                   */ , DO_H( help on obsolete ) /*                 */ },
  {.o = {DO_Q( "help-none" ) /*         */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( NONE ) /*                                       */ , DO_H( help on none ) /*                     */ },
  {.o = {DO_Q( "help-print" ) /*        */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( PRINT ) /*                                      */ , DO_H( help on print ) /*                    */ },
  {.o = {DO_Q( "help-reference" ) /*    */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( REFERENCE ) /*                                  */ , DO_H( help on reference ) /*                */ },
  {.o = {DO_Q( "help-request" ) /*      */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( REQUEST ) /*                                    */ , DO_H( help on request ) /*                  */ },
  {.o = {DO_Q( "help-scan" ) /*         */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( SCAN ) /*                                       */ , DO_H( help on scan ) /*                     */ },
  {.o = {DO_Q( "help-system" ) /*       */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( SYSTEM ) /*                                     */ , DO_H( help on system ) /*                   */ },
  {.o = {DO_Q( "help-trace" ) /*        */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( TRACE ) /*                                      */ , DO_H( help on trace ) /*                    */ },
  {.o = {DO_Q( "help-update" ) /*       */ , DO_A_N /* */ } /*                             */ , DO_CL( HELP ) /*       */ ,
   /*      */ DO_T( VI_CALL ), DO_VI_CALLH( UPDATE ) /*                                     */ , DO_H( help on update ) /*                   */ },

  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
