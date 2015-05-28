#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"

#include "duf_option_help.h"
/*
 * At duf_options_table.c:
 * 1. extern const duf_longval_extended_table_t optable_help[];
 * 2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
 *    ...
 *     optable_help,
 *    ...
 *   NULL
 *  };
*/

const duf_longval_extended_table_t optable_help = {
  DO_STG_NOT( FIRST ),
  .table =                      /* */
  {


   {.o = {DO_Q( "help-all" ) /*         */ , DO_A_N /*  */ , DO_V( HELP_ALL )} /*          */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help_all, ALL ) /*                                        */ , DO_H( help on all ) /*                      */ },
   {.o = {DO_Q( "help-collect" ) /*     */ , DO_A_N /*  */ , DO_V( HELP_COLLECT )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, COLLECT ) /*                                        */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-filter" ) /*     */ , DO_A_N /*  */ , DO_V( HELP_FILTER )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, FILTER ) /*                                        */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-control" ) /*     */ , DO_A_N /*  */ , DO_V( HELP_CONTROL )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, CONTROL ) /*                                        */ , DO_H( help on control ) /*                  */ },
   {.o = {DO_Q( "help-debug" ) /*       */ , DO_A_N /*  */ , DO_V( HELP_DEBUG )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, DEBUG ) /*                                          */ , DO_H( help on debug ) /*                    */ },
   {.o = {DO_Q( "help-help" ) /*        */ , DO_A_N /*  */ , DO_V( SMART_HELP )} /*         */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, HELP ) /*                                           */ , DO_H( help on help ) /*                     */ },
   {.o = {DO_Q( "help-nodesc" ) /*      */ , DO_A_N /*  */ , DO_V( HELP_NODESC )} /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, NODESC ) /*                                         */ , DO_H( help on nodesc ) /*                   */ },
   {.o = {DO_Q( "help-obsolete" ) /*    */ , DO_A_N /*  */ , DO_V( HELP_OBSOLETE )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, OBSOLETE ) /*                                       */ , DO_H( help on obsolete ) /*                 */ },
   {.o = {DO_Q( "help-none" ) /*        */ , DO_A_N /*  */ , DO_V( HELP_NONE )} /*         */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, NONE ) /*                                           */ , DO_H( help on none ) /*                     */ },
   {.o = {DO_Q( "help-print" ) /*       */ , DO_A_N /*  */ , DO_V( HELP_PRINT )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, PRINT ) /*                                          */ , DO_H( help on print ) /*                    */ },
   {.o = {DO_Q( "help-reference" ) /*   */ , DO_A_N /*  */ , DO_V( HELP_REFERENCE )} /*    */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, REFERENCE ) /*                                      */ , DO_H( help on reference ) /*                */ },
   {.o = {DO_Q( "help-request" ) /*     */ , DO_A_N /*  */ , DO_V( HELP_REQUEST )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, REQUEST ) /*                                        */ , DO_H( help on request ) /*                  */ },
   {.o = {DO_Q( "help-scan" ) /*        */ , DO_A_N /*  */ , DO_V( HELP_SCAN )} /*         */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, SCAN ) /*                                           */ , DO_H( help on scan ) /*                     */ },
   {.o = {DO_Q( "help-system" ) /*      */ , DO_A_N /*  */ , DO_V( HELP_SYSTEM )} /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, SYSTEM ) /*                                         */ , DO_H( help on system ) /*                   */ },
   {.o = {DO_Q( "help-trace" ) /*       */ , DO_A_N /*  */ , DO_V( HELP_TRACE )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, TRACE ) /*                                          */ , DO_H( help on trace ) /*                    */ },
   {.o = {DO_Q( "help-update" ) /*      */ , DO_A_N /*  */ , DO_V( HELP_UPDATE )} /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VIA_CALLCL( smart_help, UPDATE ) /*                                         */ , DO_H( help on update ) /*                   */ },


   {.o = {DO_Q( "help" ) /*              */ , DO_A_N /*  */ , DO_V( HELP )} /*              */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_VV_CALL( help ) /*                                                         */ , DO_H( help ) /*                             */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
