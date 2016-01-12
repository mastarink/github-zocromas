#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"

#include "duf_optimpl_misc.h"
#include "duf_optimpl_help.h"
#include "duf_optimpl_version.h"
/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_info[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_info,
	 ...

	NULL
      };
*/
const duf_longval_extended_table_t optable_info = {
  .name = "info",
  .table =                      /* */
  {
   {.o = {DO_Q( "version" ) /*          */ , DO_A_N /* */ , DO_V( DUFVERSION )} /*         */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( version ), DO_AT_STAGE( SETUP ) /*                               */ , DO_H( version info ) /*                     */ },
   {.o = {DO_Q( "flags" ) /*            */ , DO_A_N /* */ , DO_V( FLAG_INFO )} /*          */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( showflags ), DO_AT_STAGE( SETUP ) /*                             */ , DO_H( flags info ) /*                       */ },

   /* list-options, list-extended :: see help-set   */
   {.o = {DO_Q( "list-options" ) /*     */ , DO_A_O /* */ , DO_V( LIST_OPTIONS )} /*       */ , DO_CL( NODESC ) /*  */
    , DO_N_CALL( list_options ) /*                                                          */ , DO_H(  ... ) /*                             */ },   
   {.o = {DO_Q( "list-extended" ) /*     */ , DO_A_O /* */ , DO_V( LIST_XTENDED )} /*      */ , DO_CL( NODESC ) /*  */
    , DO_S_CALL( list_xtended ), DO_AT_STAGE( SETUP ) /*                                    */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "error-level" ) /*      */ , DO_A_O /* */ , DO_V( ERROR_LEVEL )} /*        */ , DO_CL( NODESC ) /*  */
    , DO_N_CALL( error_level ), DO_AT_STAGE( SETUP ) /*                                     */ , DO_H(  ... ) /*                             */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
