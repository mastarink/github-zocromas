#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_uplus[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_uplus,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_uplus = {
  .name = "uplus",
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "sample" ) /*             */ , DO_A_O /* */ , DO_V( SAMPLE )} /*         */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OC( UPLUS, opt.act.sample ) /*                                                             */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "sampupd" ) /*            */ , DO_A_O /* */ , DO_V( SAMPUPD )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OC( UPLUS, opt.act.sampupd ) /*                                                            */ , DO_H(  .... ) /*             */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
