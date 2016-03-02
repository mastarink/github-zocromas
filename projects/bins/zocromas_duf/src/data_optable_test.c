#include <stddef.h>

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_main[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_main,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_test = {
  .name = "test",
//DO_AT_STAGE( SETUP ),
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "test-act-flag" ) /*            */ , DO_A_N /* */ , DO_VF( TEST_ACT_FLAG )} /*              */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, testflag ), DO_STAGE_ALL /*                                  */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-iflag" ) /*           */ , DO_A_N /* */ , DO_VF( TEST_ACT_IFLAG )} /*             */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( BFLAG, opt.act.v ), DO_BFN( act, testiflag ), DO_STAGE_ALL /*                                 */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-noflag" ) /*          */ , DO_A_N /* */ , DO_VF( TEST_ACT_NOFLAG )} /*            */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( NOBFLAG, opt.act.v ), DO_BFL( act, testnoflag ), DO_STAGE_ALL /*                              */ , DO_H(  ... ) /*            */ },

   {.o = {DO_Q( "test-act-bflag" ) /*           */ , DO_A_N /* */ } /*              */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, testflag ), DO_STAGE_ALL /*                                */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-ibflag" ) /*          */ , DO_A_N /* */ } /*              */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( BFLAG, opt.act.v ), DO_BFN( act, testflag ), DO_STAGE_ALL /*                                */ , DO_H(  ... ) /*            */ },
   {.o = {DO_Q( "test-act-nobflag" ) /*         */ , DO_A_N /* */ } /*              */ , DO_CL( TEST ) /* */ ,
    /* */ DO_OC( NOBFLAG, opt.act.v ), DO_BFL( act, testnoflag ), DO_STAGE_ALL /*                            */ , DO_H(  ... ) /*            */ },

   {.o = {DO_Q( "test-recetc-flag" ) /*         */ , DO_A_N /* */ , DO_VF( TEST_RECETC_FLAG )} /*           */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( rec_etc, testflag ), DO_STAGE_SPLS /*                               */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-iflag" ) /*        */ , DO_A_N /* */ , DO_VF( TEST_RECETC_IFLAG )} /*          */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFN( rec_etc, testiflag ), DO_STAGE_SPLS /*                              */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-noflag" ) /*       */ , DO_A_N /* */ , DO_VF( TEST_RECETC_NOFLAG )} /*         */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( NOBSFLAG, v ), DO_BFL( rec_etc, testnoflag ), DO_STAGE_SPLS /*                           */ , DO_H( sflag ) /*           */ },

   {.o = {DO_Q( "test-recetc-bflag" ) /*        */ , DO_A_N /* */ } /*           */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( rec_etc, testflag ), DO_STAGE_SPLS /*                             */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-ibflag" ) /*       */ , DO_A_N /* */ } /*                                      */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFN( rec_etc, testiflag ), DO_STAGE_SPLS /*                            */ , DO_H( sflag ) /*           */ },
   {.o = {DO_Q( "test-recetc-nobflag" ) /*      */ , DO_A_N /* */ } /*                                      */ , DO_CL( TEST ) /* */ ,
    /*      */ DO_OU( NOBSFLAG, v ), DO_BFL( rec_etc, testnoflag ), DO_STAGE_SPLS /*                         */ , DO_H( sflag ) /*           */ },

   {.o = {DO_Q( "test-disable-flag" ) /*        */ , DO_A_N, DO_VF( TEST_DISABLE_FLAG )} /*                 */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ), DO_BFL( disable, testflag ) /*                                    */ , DO_H(  .... ) /*           */ },
   {.o = {DO_Q( "test-disable-iflag" ) /*       */ , DO_A_N, DO_VF( TEST_DISABLE_IFLAG )} /*                */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ), DO_BFN( disable, testiflag ) /*                                   */ , DO_H(  .... ) /*           */ },
 /* new macro; also: without ID! : "DOO_VF( TEST_DISABLE_NOFLAG )" */
   {DOO_Q( "test-disable-noflag" ), DOO_BFL( NOBSFLAG, opt, disable, testnoflag ), DOO_CL( TEST ), DOO_H(  .... )},

   {.o = {DO_Q( "test-disable-bflag" ) /*       */ , DO_A_N, DO_VF( TEST_DISABLE_FLAG )} /*                 */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ), DO_BFL( disable, testflag ) /*                                  */ , DO_H(  .... ) /*           */ },
   {.o = {DO_Q( "test-disable-ibflag" ) /*      */ , DO_A_N, DO_VF( TEST_DISABLE_IFLAG )} /*                */ , DO_CL( TEST ) /*    */ ,
    /*      */ DO_OC( BSFLAG, opt.disable ), DO_BFN( disable, testiflag ) /*                                 */ , DO_H(  .... ) /*           */ },
   {DOO_Q( "test-disable-nobflag" ), DOO_BFL( NOBSFLAG, opt, disable, testnoflag ), DOO_CL( TEST ) /*       */ , DOO_H(  .... )},

   {DOO_Q( "test-cfg-num" ) /*                  */ , DOO_A_R /* */ , DOO_V( TEST_CFG_NUM ) /*               */ , DOO_CL( TEST ) /*  */ ,
    /*      */ DOO_OC( NUM, testnum ) /*          */ , DOO_AT_STAGE( SETUP ) /*                              */ , DOO_H(  .... ) /*          */ },

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,4,52,67,68,112,113,131,145,146
*/
