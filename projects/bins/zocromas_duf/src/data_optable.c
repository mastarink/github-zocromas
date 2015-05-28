#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"

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
  .table =                      /* */
  {
   {.o = {DO_Q( "include-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_INCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /*                                                                                      */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "exclude-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_EXCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /* */ , DO_H(  ... ) /*              */ },




   {.o = {DO_N( depth ) /*              */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OPDI( PDINUM, depth ), DO_AT_STAGE( 1 ) /*                                */ , DO_H( pdi cd ) /*                           */ },
   {.o = {DO_N( maxdepth ) /*           */ , DO_A_O} /*                                    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OPDI( PDINUM, maxdepth ), DO_AT_STAGE( 1 ) /*                                           */ , DO_H( pdi cd ) /*             */ },


   {.o = {DO_Q( "max-rel-depth" ) /*    */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*                                               */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-depth" ) /*        */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( NUM, max_rel_depth ) /*                                               */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-depth" ) /*        */ , DO_A_R /* */ , DO_V( MAXRELDEPTH )} /*        */ , DO_CL( NODESC ) /*  */ ,
    /*      */ DO_OU( PNUM, max_rel_depth ), DO_AT_STAGE( 1 ) /*                                               */ ,
    DO_H(  .... ) /*              */ },
   {.o = {DO_Q( "max-seq" ) /*          */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( NUM, max_seq ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-seq" ) /*          */ , DO_A_R /* */ , DO_V( MAXSEQ )} /*             */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OU( PNUM, max_seq ), DO_AT_STAGE( 1 ) /*                                                      */ ,
    DO_H(  .... ) /*              */ },


   {.o = {DO_Q( "output-level" ) /*     */ , DO_A_O /* */ , DO_V( OUTPUT_LEVEL )} /*       */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( NUM, cli.output.level ) /*                                            */ , DO_H( output level ) /*                     */ },
   {.o = {DO_Q( "output-file" ) /*      */ , DO_A_R /* */ , DO_V( OUTPUT_FILE )} /*        */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, cli.output ) /*                                                 */ , DO_H( output to file ) /*                   */ },
   {.o = {DO_Q( "output-stderr" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDERR )} /*      */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, cli.output ),.call = {.value = {.u = 2}} /*                     */ , DO_H( output to stderr ) /*                 */ },
   {.o = {DO_Q( "output-stdout" ) /*    */ , DO_A_N /* */ , DO_V( OUTPUT_STDOUT )} /*      */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OC( FILE, cli.output ),.call = {.value = {.u = 1}} /*                     */ , DO_H( output to stdout ) /*                 */ },

   {.o = {DO_Q( "test-option" ) /*      */ , DO_A_R /* */ , DO_V( TEST )} /*               */ , DO_CL( DEBUG ) /*   */ , DO_H(  .... ) /*    */ },

   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
