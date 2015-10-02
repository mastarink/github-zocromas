#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"

#include "duf_option_sccb.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_sccb[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_sccb,
	 ...

	NULL
      };
*/


const duf_longval_extended_table_t optable_sccb = {
  /* DO_SET_STAGE( DUF_OPTION_STAGE_SETUP, DUF_OPTION_STAGE_INTERACTIVE ), */
  DO_STG_NOT( SETUP ),
  .table =                      /* */
  {
   {.o = {DO_Q( "list-sccbs" ) /*       */ , DO_A_N /*  */ , DO_V( LIST_SCCBS )} /*        */ , DO_CL( NODESC ) /*  */
    , DO_EV_CALL( list_sccbs ) /*                                                           */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "evaluate-sccb" ) /*    */ , DO_A_R /*  */ , DO_V( EVALUATE_SCCB )} /*     */ , DO_CL( NODESC ) /*  */
    , DO_S_CALL( evaluate_sccb ) /*       */ , DO_SET_STAGE( FIRST, LOOP ) /*               */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "lsfiles" ) /*               */ , DO_A_N /*  */ , DO_V( LSFILES )} /*      */ , DO_CL( NODESC ) /*  */
    , DO_SAS_CALL( evaluate_sccb, "listing" ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*  */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "tree" ) /*             */ , DO_A_N /*  */ , DO_V( TREE )} /*              */ , DO_CL( NODESC ) /*  */
    , DO_SAS_CALL( evaluate_sccb, "tree" ) /* */ , DO_SET_STAGE( FIRST, INTERACTIVE ) /*     */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "call-file" ) /*        */ , DO_A_R /*  */ , DO_V( CALL_FILE )} /*         */ , DO_CL( NODESC ) /*  */
    , DO_S_CALL( call_file ) /*         */ , DO_SET_STAGE( FIRST, LOOP ) /*                 */ , DO_H(  ... ) /*                             */ },



   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
