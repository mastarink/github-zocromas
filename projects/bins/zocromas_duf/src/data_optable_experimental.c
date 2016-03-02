#include <stddef.h>

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

#include "duf_optimpl_misc.h"
#include "duf_optimpl_sccb.h"                                        /* TODO : temp for duf_option_O_cd               */

/*
At duf_options_table.c:
  1. extern const muc_longval_extended_table_t optable_main[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_main,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_experimental = {
  .name = "experimental",
/* DO_STG_NOT( LOOP ), */
  .xlist =                                                           /* */
  {
#if 0
   {.o = {DO_Q( "include-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_INCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /*                                                                                      */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "exclude-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_EXCL_FS_FILES )} /* */ , DO_CL( NODESC ) /*     */
    /*                                                                                      */ , DO_H(  ... ) /*                             */ },
#else
 /* FIXME in fact only 1 works...? XXX */
   {.o = {DO_Q( "include-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_INCL_FS_FILES )} /* */ , DO_CL( FS ) /*     */ ,
    /*      */ DO_OU( ARGV, globx.include_fs_files ) /*                                     */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "exclude-fs" ) /*       */ , DO_A_R /* */ , DO_V( GLOB_EXCL_FS_FILES )} /* */ , DO_CL( FS ) /*     */ ,
    /*      */ DO_OU( ARGV, globx.exclude_fs_files ) /*                                     */ , DO_H(  ... ) /*                             */ },
#endif

#if 0
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISTR, pdi ) /*                                                      */ , DO_H( pdi cd ) /*                           */ },
   {.o = {DO_N( pwd ) /*                 */ , DO_A_N, DO_V( PWD )} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISTR, pdi ) /*                                    */ , DO_H( pdi cd ) /*                           */ },
#elif 0
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_S_CALL( cd ), DO_STAGE_SPLS, DO_STG_NOT( FIRST ), DO_STG_NOT( LOOP ) /*                                      */ ,
    DO_H( pdi cd ) /*                           */ },
#else
   {.o = {DO_N( cd ) /*                 */ , DO_A_O /*  */ , DO_V( CD )} /*                */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_S_CALL( cd ), DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                                      */ ,
    DO_H( pdi cd ) /*                           */ },
#endif
   {.o = {DO_Q( "cd-id" ) /*            */ , DO_A_O /* */ , DO_V( CD_ID )} /*              */ , DO_CL( NODESC ) /*  */
    , DO_N_CALL( cdid ), DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*          */ , DO_H(  ... ) /*                             */ },

#if 0                                                                /* 20160221.132919 */
   {.o = {DO_Q( "option-delimiter" ) /* */ , DO_A_R /* */ , DO_V( OPTION_DELIMITER )} /*   */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( XCHR, pcli->option_delimiter ) /*                                     */ , DO_H( history filename ) /*                 */ },
#else
   {.o = {DO_Q( "option-delimiter" ) /* */ , DO_A_R /* */ , DO_V( OPTION_DELIMITER )} /*   */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OI( XCHR, option_delimiter ) /*                                           */ , DO_H( history filename ) /*                 */ },
#endif

   {.o = {DO_Q( "echo" ) /*             */ , DO_A_R /* */ , DO_V( ECHO )} /*               */ , DO_CL( NODESC ) /*  */
    , DO_S_CALL( echo ) /* */ , DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*   */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "dir-priority" ) /*     */ , DO_A_R /* */ , DO_V( DIR_PRIORITY )} /*       */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OC( NUM, vars.dir_priority ) /*                                           */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "test-option" ) /*      */ , DO_A_R /* */ , DO_V( TEST )} /*               */ , DO_CL( DEBUG ) /*   */ , DO_H(  .... ) /*    */ },

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
