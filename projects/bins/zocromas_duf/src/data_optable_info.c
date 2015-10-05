#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"

#include "duf_optable_def.h"

#include "duf_option_misc.h"
#include "duf_option_help.h"
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
   {.o = {DO_Q( "version" ) /*          */ , DO_A_N /* */ } /*                            */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( version ) /*                                                     */ , DO_H( version info ) /*                     */ },
   {.o = {DO_Q( "flags" ) /*            */ , DO_A_N /* */ } /*                            */ , DO_CL( REFERENCE ) /*  */ ,
    /*      */ DO_EV_CALL( showflags ) /*                                                   */ , DO_H( version info ) /*                     */ },
   {.o = {DO_Q( "help-examples" ) /*    */ , DO_A_N /* */ , DO_V( EXAMPLES )} /*          */ , DO_CL( HELP ) /*      */ ,
    /*      */ DO_EV_CALL( examples ) /*                                                    */ , DO_H( examples ) /*                         */ },
   {.o = {DO_Q( "list-options" ) /*     */ , DO_A_O /* */ , DO_V( LIST_OPTIONS )} /*      */ , DO_CL( NODESC ) /*  */
    , DO_N_CALL( list_options ) /*                                                          */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "error-level" ) /*      */ , DO_A_O /* */ , DO_V( ERROR_LEVEL )} /*        */ , DO_CL( NODESC ) /*  */
    , DO_N_CALL( error_level ), DO_AT_STAGE( SETUP ) /*                                     */ , DO_H(  ... ) /*                             */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
