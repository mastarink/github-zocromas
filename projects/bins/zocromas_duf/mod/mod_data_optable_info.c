#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_optimpl_misc.h"
#include "duf_optimpl_help.h"
#include "duf_optimpl_version.h"

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_info[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_info,
	 ...

	NULL
      };
*/
static muc_longval_extended_table_t optable;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"optab", &optable},
  {NULL, NULL}
};

static muc_longval_extended_table_t optable = {
  .name = "info",
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "version" ) /*          */ , DO_A_N /* */ , DO_V( DUFVERSION )} /*         */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( version ), DO_AT_STAGE( SETUP ) /*                               */ , DO_H( version info ) /*                     */ },
   {.o = {DO_Q( "flags" ) /*            */ , DO_A_N /* */ , DO_V( FLAG_INFO )} /*          */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( showflags ), DO_AT_STAGE( SETUP ), DO_STG_YES( IA ) /*                             */ ,
    DO_H( flags info ) /*                       */ },

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
