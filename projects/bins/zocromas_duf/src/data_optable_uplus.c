#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h" 

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_uplus[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_uplus,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_uplus = {
  .name = "uplus",
  .xlist =                      /* */
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
