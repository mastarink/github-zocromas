#include <stddef.h>

#include <mastar/multiconfig/muc_config_cli_structs.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

/* #include "duf_optimpl_sccb.h"                                        (* TODO : temp for duf_optimpl_O_db_open               *) */

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_str[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_str,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_options = {
  .name = "options",
  .xlist =                                                           /* */
  {

#if 0                                                                /* 20160221.132947 */
   {.o = {DO_Q( "history-file" ) /*      */ , DO_A_R /* */ , DO_V( HISTORY_FILE )} /*      */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, pcli->history_filename ) /*                                      */ , DO_H( history filename ) /*                 */ },
#else
   {.o = {DO_Q( "history-file" ) /*      */ , DO_A_R /* */ , DO_V( HISTORY_FILE )} /*      */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OI( STR, history_filename ) /*                                            */ , DO_H( history filename ) /*                 */ },
#endif

#if 0                                                                /* 20160221.132919 */
   {.o = {DO_Q( "option-delimiter" ) /* */ , DO_A_R /* */ , DO_V( OPTION_DELIMITER )} /*   */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( XCHR, pcli->option_delimiter ) /*                                     */ , DO_H( history filename ) /*                 */ },
#else
   {.o = {DO_Q( "option-delimiter" ) /* */ , DO_A_R /* */ , DO_V( OPTION_DELIMITER )} /*   */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OI( XCHR, option_delimiter ) /*                                           */ , DO_H( history filename ) /*                 */ },
#endif

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
