#include <stddef.h>

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

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
  .xlist =                                                           /* */
  {

#if 0
   {.o = {DO_N( depth ) /*              */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_ODI( PDINUM, depth ) /*             */ , DO_AT_STAGE( SETUP ) /*           */ , DO_H( pdi cd ) /*                          */ },
   {.o = {DO_N( maxdepth ) /*           */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_ODI( PDINUM, maxdepth ) /*          */ , DO_AT_STAGE( SETUP ) /*           */ , DO_H( pdi cd ) /*                          */ },
#endif

   {.o = {DO_Q( "max-rel-depth" ) /*    */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-rel-depth" ) /**/, DO_A_R /*  */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_SET_STAGE( FIRST, IA ) /*    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "max-depth" ) /*        */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-depth" ) /*    */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*          */ , DO_SET_STAGE( FIRST, IA ) /*    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "max-seq" ) /*          */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, max_seq ) /*                */ , DO_AT_STAGE( SETUP ) /*         */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "set-max-seq" ) /*      */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, max_seq ) /*                */ , DO_SET_STAGE( FIRST, IA ) /*    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "std-leaf-set-num" ) /* */ , DO_A_R /* */ , DO_V( STD_LEAF_SET_NUM )} /*   */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, std_leaf_set_num ) /*       */ , DO_SET_STAGE( FIRST, IA ) /*    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "std-node-set-num" ) /* */ , DO_A_R /* */ , DO_V( STD_NODE_SET_NUM )} /*   */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, std_node_set_num ) /*       */ , DO_SET_STAGE( FIRST, IA ) /*    */ , DO_H(  .... ) /*                            */ },

   {DOO_Q( "std-leaf-set-name" ) /*     */ , DOO_A_R /*                                    */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( STR, std_leaf_set_name ) /*      */ , DO_STAGES( FIRST, IA ) /*       */ , DO_H(  .... ) /*                            */ },
   {DOO_Q( "std-node-set-name" ) /*     */ , DOO_A_R /*                                    */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( STR, std_node_set_name ) /*      */ , DO_STAGES( FIRST, IA ) /*       */ , DO_H(  .... ) /*                            */ },

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
