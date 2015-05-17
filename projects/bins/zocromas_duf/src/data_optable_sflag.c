#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_sflag[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_sflag,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_sflag = {
  .table =                      /* */
  {
   {.o = {DO_Q( "disable-calculate" ) /* */ , DO_A_N /* */ , DO_VF( DISABLE_CALCULATE )} /* */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( SFLAG, cli.disable ) /*             */ , DO_FL( disable, calculate ) /* */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "disable-insert" ) /*    */ , DO_A_N /* */ , DO_VF( DISABLE_INSERT )} /*    */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( SFLAG, cli.disable ) /*             */ , DO_FL( disable, insert ) /*    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "disable-update" ) /*    */ , DO_A_N /* */ , DO_VF( DISABLE_UPDATE )} /*    */ , DO_CL( COLLECT ) /*    */ ,
    /*      */ DO_OC( SFLAG, cli.disable ) /*             */ , DO_FL( disable, update ) /*    */ , DO_H(  .... ) /*                            */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
