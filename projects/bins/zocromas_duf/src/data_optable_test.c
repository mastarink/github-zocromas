#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h"

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

const duf_longval_extended_table_t optable_test = {
  .name = "test",
//DO_AT_STAGE( SETUP ),
  .table =                      /* */
  {

   {.o = {DO_Q( "test-act-flag" ) /*            */ , DO_A_N /* */ , DO_VF( TEST_ACT_FLAG )} /*              */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( FLAG, opt.act.v ), DO_FL( act, testflag ), DO_STAGE_ALL /*                                  */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-iflag" ) /*           */ , DO_A_N /* */ , DO_VF( TEST_ACT_IFLAG )} /*             */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( FLAG, opt.act.v ), DO_FN( act, testiflag ), DO_STAGE_ALL /*                                 */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-noflag" ) /*          */ , DO_A_N /* */ , DO_VF( TEST_ACT_NOFLAG )} /*            */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( NOFLAG, opt.act.v ), DO_FL( act, testnoflag ), DO_STAGE_ALL /*                              */ , DO_H(  ... ) /*            */ },

   {.o = {DO_Q( "test-recetc-flag" ) /*         */ , DO_A_N /* */ , DO_VF( TEST_RECETC_FLAG )} /*           */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( SFLAG, v ), DO_FL( rec_etc, testflag ), DO_STAGE_SPLS /*                               */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-iflag" ) /*        */ , DO_A_N /* */ , DO_VF( TEST_RECETC_IFLAG )} /*          */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( SFLAG, v ), DO_FN( rec_etc, testiflag ), DO_STAGE_SPLS /*                              */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-noflag" ) /*       */ , DO_A_N /* */ , DO_VF( TEST_RECETC_NOFLAG )} /*         */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( NOSFLAG, v ), DO_FL( rec_etc, testnoflag ), DO_STAGE_SPLS /*                           */ , DO_H( sflag ) /*           */ },

   {.o = {DO_Q( "test-disable-flag" ) /*        */ , DO_A_N, DO_VF( TEST_DISABLE_FLAG )} /*                 */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( SFLAG, opt.disable ), DO_FL( disable, testflag ) /*                                    */ , DO_H(  .... ) /*           */ },

   {.o = {DO_Q( "test-disable-iflag" ) /*        */ , DO_A_N, DO_VF( TEST_DISABLE_IFLAG )} /*               */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( SFLAG, opt.disable ), DO_FN( disable, testiflag ) /*                                   */ , DO_H(  .... ) /*           */ },

   {DOO_Q( "test-disable-noflag" ), DOO_VF( TEST_DISABLE_NOFLAG ), DOO_FL( NOSFLAG, opt, disable, testnoflag ), DO_CL( TEST ), DO_H(  .... ) /**/},


   {.o = {DO_Q( "test-cfg-num" ) /*             */ , DO_A_R /* */ , DO_V( TEST_CFG_NUM )} /*                */ , DO_CL( TEST ) /*  */ ,
    /*      */ DO_OC( NUM, testnum ) /*          */ , DO_AT_STAGE( SETUP ) /*                                */ , DO_H(  .... ) /*           */ },

   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,52,67,68,112,113,131,145,146
*/
