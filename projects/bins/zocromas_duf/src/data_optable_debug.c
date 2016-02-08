#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h" 

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_debug[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_debug,
	 ...

	NULL
      };
*/


const duf_longval_extended_table_t optable_debug = {
  .name = "debug",
  .xlist =                      /* */
  {
   {.o = {DO_Q( "min-dbg-lines" ) /*    */ , DO_A_R /* */ , DO_V( MIN_DBGLINE )} /*        */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( NUM, opt.dbg.min_line ) /*                                            */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "max-dbg-lines" ) /*    */ , DO_A_R /* */ , DO_V( MAX_DBGLINE )} /*        */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( NUM, opt.dbg.max_line ) /*                                            */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "debug" ) /*            */ , DO_A_O /* */ , DO_V( DEBUG )} /*              */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( UPLUS, opt.dbg.debug ) /*                                             */ , DO_H( debug ) /*                            */ },
   {.o = {DO_Q( "verbose" ) /*          */ , DO_A_O /* */ , DO_V( VERBOSE )} /*            */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( UPLUS, opt.dbg.verbose ), DO_AT_STAGE( PRESETUP ) /*                  */ , DO_H( verbose ) /*                          */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
