#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"


#include "duf_option_sccb.h"    /* TODO : temp for duf_option_$_cd               */

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

const duf_longval_extended_table_t optable_main = {
  .name = "main",
  .table =                      /* */
  {

#if 0
   {.o = {DO_N( depth ) /*              */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_ODI( PDINUM, depth ) /*             */ , DO_AT_STAGE( SETUP ) /*           */ , DO_H( pdi cd ) /*                           */ },
   {.o = {DO_N( maxdepth ) /*           */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_ODI( PDINUM, maxdepth ) /*          */ , DO_AT_STAGE( SETUP ) /*           */ , DO_H( pdi cd ) /*                           */ },
#endif

   {.o = {DO_Q( "max-rel-depth" ) /*    */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-rel-depth" ) /**/, DO_A_R /*  */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "max-depth" ) /*        */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-depth" ) /*    */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*         */ ,
    DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "max-seq" ) /*          */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, max_seq ) /*                */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-seq" ) /*      */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, max_seq ) /*  */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*         */ , DO_H(  .... ) /*                            */ },

   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
