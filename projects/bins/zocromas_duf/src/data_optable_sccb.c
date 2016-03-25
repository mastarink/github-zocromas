#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_optimpl_sccb.h"

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_sccb[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_sccb,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_sccb = {
  .name = "sccb",
/* DO_SET_STAGE( DUF_OPTION_STAGE_SETUP, DUF_OPTION_STAGE_INTERACTIVE ), */
  DO_STG_NOT( SETUP ),
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "list-sccbs" ) /*       */ , DO_A_N /*  */ , DO_V( LIST_SCCBS )} /*        */ , DO_CL( SCCB ) /*  */
    , DO_EV_CALL( list_sccbs ) /*                                                           */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "evaluate-sccb" ) /*    */ , DO_A_R /*  */ , DO_V( EVALUATE_SCCB )} /*     */ , DO_CL( SCCB ) /*  */
    , DO_S_CALL( evaluate_sccb ) /*     */ , DO_SET_STAGE( FIRST, LOOP ) /*                 */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "lsfiles" ) /*          */ , DO_A_N /*  */ , DO_V( LSFILES )} /*      */ , DO_CL( SCCB ) /*  */
    , DO_SAS_CALL( evaluate_sccb, "listing" ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /* */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "tree" ) /*             */ , DO_A_N /*  */ , DO_V( TREE )} /*              */ , DO_CL( SCCB ) /*  */
    , DO_SAS_CALL( evaluate_sccb, "tree" ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*    */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "call-file" ) /*        */ , DO_A_R /*  */ , DO_V( CALL_FILE )} /*         */ , DO_CL( SCCB ) /*  */
    , DO_S_CALL( call_file ) /*         */ , DO_SET_STAGE( FIRST, LOOP ) /*                 */ , DO_H(  ... ) /*                             */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
