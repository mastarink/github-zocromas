#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"


#include "duf_option_sccb.h"    /* FIXME : temp for duf_option_$_cd               */

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_main[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_main,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_io = {
  .table =                      /* */
  {


   {.o = {DO_Q( "output-level" ) /*     */ , DO_A_O /* */ , DO_V( OUTPUT_LEVEL )} /*       */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( NUM, cli.output.level ) /*        */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output level ) /*                     */ },
   {.o = {DO_Q( "output-fun-width" ) /* */ , DO_A_O /* */ , DO_V( OUTPUT_FUN_WIDTH )} /*   */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( NUM, cli.output.fun_width ) /*    */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output fun width ) /*                 */ },
   {.o = {DO_Q( "output-header" ) /*    */ , DO_A_O /* */ , DO_V( OUTPUT_HEADER )} /*      */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( STR, cli.output.header ) /*       */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output header ) /*                    */ },

   {.o = {DO_Q( "output-file" ) /*      */ , DO_A_R /* */ , DO_V( OUTPUT_FILE )} /*        */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, cli.output ) /*             */ , DO_AT_STAGE( SETUP ) /*        */ , DO_H( output to file ) /*                   */ },
   {.o = {DO_Q( "set-output-file" ) /*  */ , DO_A_R /* */ , DO_V( OUTPUT_FILE )} /*        */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, cli.output ) /*   */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*    */ , DO_H( output to file ) /*                   */ },

   {.o = {DO_Q( "output-stderr" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDERR )} /*      */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 2}}, /* */ DO_OC( FILE, cli.output ) /* */ , DO_AT_STAGE( SETUP ) /* */ , DO_H( output to stderr ) /*           */ },
   {.o = {DO_Q( "set-output-stderr" ) /* */ , DO_A_N /* */ , DO_V( OUTPUT_STDERR )} /*     */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 2}}, /* */ DO_OC( FILE, cli.output ), DO_SET_STAGE( FIRST, INTERACTIVE ) /* */ , DO_H( output to stderr ) /*     */},

   {.o = {DO_Q( "output-stdout" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDOUT )} /*      */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 1}}, /* */ DO_OC( FILE, cli.output ), DO_AT_STAGE( SETUP ) /*  */ , DO_H( output to stdout ) /*                 */ },
   {.o = {DO_Q( "set-output-stdout" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDOUT )} /*  */ , DO_CL( PRINT ) /* */
    ,.call = {.value = {.u = 1}}, /* */ DO_OC( FILE, cli.output ), DO_SET_STAGE( FIRST, INTERACTIVE ) /* */ , DO_H( output to stdout ) /*    */ },


   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
