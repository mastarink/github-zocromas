#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"


#include "duf_option_help.h"
#include "duf_option_sccb.h"    /* FIXME : temp for duf_option_$_cd               */

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

const duf_longval_extended_table_t optable_experimental = {
  /* DO_STG_NOT( LOOP ), */
  .table =                      /* */
  {
#if 0
   {.o = {DO_Q( "include-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_INCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /*                                                                                      */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "exclude-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_EXCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /*                                                                                      */ , DO_H(  ... ) /*                             */ },
#else
   /* FIXME in fact only 1 works...? XXX */
   {.o = {DO_Q( "include-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_INCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */ ,
    /*      */ DO_OU( ARGV, globx.include_fs_files ) /*                                      */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "exclude-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_EXCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */ ,
    /*      */ DO_OU( ARGV, globx.exclude_fs_files ) /*                                      */ , DO_H(  ... ) /*                             */ },
#endif


#if 0
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISTR, pdi ) /*                                                      */ , DO_H( pdi cd ) /*                           */ },
   {.o = {DO_N( pwd ) /*                 */ , DO_A_N, DO_V( PWD )} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISTR, pdi ) /*                                    */ , DO_H( pdi cd ) /*                           */ },
#elif 0
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_S_CALL( cd ), DO_STAGE_ANY, DO_STG_NOT( FIRST ), DO_STG_NOT( LOOP ) /*                                      */ ,
    DO_H( pdi cd ) /*                           */ },
#else
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_S_CALL( cd ), DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                                      */ ,
    DO_H( pdi cd ) /*                           */ },
#endif

   {.o = {DO_Q( "option-delimiter" ) /* */ , DO_A_R /* */ , DO_V( OPTION_DELIMITER )} /*   */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( XCHR, cli.option_delimiter ) /*                                       */ , DO_H( history filename ) /*                 */ },

   {.o = {DO_Q( "echo" ) /*             */ , DO_A_R /* */ , DO_V( ECHO )} /*               */ , DO_CL( NODESC ) /*  */
    , DO_S_CALL( echo ) /*       */ , DO_SET_STAGE( FIRST, LOOP ) /*                        */ , DO_H(  ... ) /*                             */ },


   {.o = {DO_Q( "test-option" ) /*      */ , DO_A_R /* */ , DO_V( TEST )} /*               */ , DO_CL( DEBUG ) /*   */ , DO_H(  .... ) /*    */ },

   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
