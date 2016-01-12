#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"

#include "duf_optimpl_help.h"
#include "duf_optimpl_smart_help.h"
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
  .name = "help",
  DO_STG_NOT( FIRST ),
  .table =                      /* */
  {


   {.o = {DO_Q( "help-class-all" ) /*   */ , DO_A_N /*  */ , DO_V( HELP_CL_ALL )} /*       */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help_all, ALL ), DO_AT_STAGE( PRESETUP ) /*             */ , DO_H( help on all ) /*                      */ },
   {.o = {DO_Q( "help-class-collect" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_COLLECT )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, COLLECT ), DO_AT_STAGE( PRESETUP ) /*             */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-class-filter" ) /* */ , DO_A_N /* */ , DO_V( HELP_CL_FILTER )} /*    */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, FILTER ), DO_AT_STAGE( PRESETUP ) /*              */ , DO_H( help on collect ) /*                  */ },
   {.o = {DO_Q( "help-class-control" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_CONTROL )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, CONTROL ), DO_AT_STAGE( PRESETUP ) /*             */ , DO_H( help on control ) /*                  */ },
   DO_CMD( "help-class-db", N, HELP_CL_DB, DO_CL( HELP ), DO_EIA_CALLCL( smart_help, DB ), DO_AT_STAGE( PRESETUP ), DO_H( help on db control ) ),
   DO_CMD( "help-class-fs", N, HELP_CL_FS, DO_CL( HELP ), DO_EIA_CALLCL( smart_help, FS ), DO_AT_STAGE( PRESETUP ), DO_H( help on fs control ) ),
   {.o = {DO_Q( "help-class-debug" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_DEBUG )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, DEBUG ), DO_AT_STAGE( PRESETUP ) /*               */ , DO_H( help on debug ) /*                    */ },
   {.o = {DO_Q( "help-class-help" ) /*  */ , DO_A_N /*  */ , DO_V( SMART_HELP )} /*        */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, HELP ), DO_AT_STAGE( PRESETUP ) /*                */ , DO_H( help on help ) /*                     */ },
   {.o = {DO_Q( "help-class-nodesc" ) /* */ , DO_A_N /* */ , DO_V( HELP_CL_NODESC )} /*    */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, NODESC ), DO_AT_STAGE( PRESETUP ) /*              */ , DO_H( help on nodesc ) /*                   */ },
   {.o = {DO_Q( "help-class-obsolete" ) /**/, DO_A_N /* */ , DO_V( HELP_CL_OBSOLETE )} /*  */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, OBSOLETE ), DO_AT_STAGE( PRESETUP ) /*            */ , DO_H( help on obsolete ) /*                 */ },
   {.o = {DO_Q( "help-class-none" ) /*  */ , DO_A_N /*  */ , DO_V( HELP_CL_NONE )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, NONE ), DO_AT_STAGE( PRESETUP ) /*                */ , DO_H( help on none ) /*                     */ },
   {.o = {DO_Q( "help-class-print" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_PRINT )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, PRINT ), DO_AT_STAGE( PRESETUP ) /*               */ , DO_H( help on print ) /*                    */ },
   {.o = {DO_Q( "help-class-reference" ), DO_A_N /*     */ , DO_V( HELP_CL_REFERENCE )} /* */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, REFERENCE ), DO_AT_STAGE( PRESETUP ) /*           */ , DO_H( help on reference ) /*                */ },
   {.o = {DO_Q( "help-class-request" ) /**/, DO_A_N /*  */ , DO_V( HELP_CL_REQUEST )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, REQUEST ), DO_AT_STAGE( PRESETUP ) /*              */ , DO_H( help on request ) /*                  */ },
   {.o = {DO_Q( "help-class-scan" ) /*  */ , DO_A_N /*  */ , DO_V( HELP_CL_SCAN )} /*      */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, SCAN ), DO_AT_STAGE( PRESETUP ) /*                 */ , DO_H( help on scan ) /*                     */ },
   {.o = {DO_Q( "help-class-system" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_SYSTEM )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, SYSTEM ), DO_AT_STAGE( PRESETUP ) /*               */ , DO_H( help on system ) /*                   */ },
   {.o = {DO_Q( "help-class-trace" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_TRACE )} /*     */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, TRACE ), DO_AT_STAGE( PRESETUP ) /*                */ , DO_H( help on trace ) /*                    */ },
   {.o = {DO_Q( "help-class-update" ) /* */ , DO_A_N /*  */ , DO_V( HELP_CL_UPDATE )} /*   */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EIA_CALLCL( smart_help, UPDATE ), DO_AT_STAGE( PRESETUP ) /*               */ , DO_H( help on update ) /*                   */ },



#if 0
   {.o = {DO_Q( "help-set" ) /*         */ , DO_A_O /*  */ , DO_V( HELP_SET )} /*          */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_S_CALL( help_set ), DO_AT_STAGE( PRESETUP ) /*                            */ , DO_H( help on set / optfile ) /*            */ },
#elif 1
   /* see help-set :: see list-options, list-extended   */
   DO_CMD( "help-set", O, HELP_SET, DO_CL( HELP ), DO_S_CALL( help_set ), DO_AT_STAGE( PRESETUP ), DO_H( help on set / optfile ) ),
#endif



   {.o = {DO_Q( "help" ) /*              */ , DO_A_N /*  */ , DO_V( HELP )} /*             */ , DO_CL( HELP ) /*       */ ,
    /*      */ DO_EV_CALL( help ), DO_AT_STAGE( PRESETUP ) /*                               */ , DO_H( show short options help ) /*                             */ },
   {.o = {DO_Q( "help-examples" ) /*    */ , DO_A_N /* */ , DO_V( EXAMPLES )} /*           */ , DO_CL( HELP ) /*      */ ,
    /*      */ DO_EV_CALL( examples ) /*                                                    */ , DO_H( show short options examples ) /*                         */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
