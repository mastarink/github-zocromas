#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h" 

#include "duf_optimpl_misc.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_interactive[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_interactive,
	 ...

	NULL
      };
*/

extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute__( ( weak ) ) */ ;

/*
void duf_list_sccb( int ) __attribute__ ( ( weak ) );
void duf_list_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
void duf_add_targ( int *targc, char ***targv, const char *s ) __attribute__ ( ( weak ) );
void duf_clear_targ( int *targc, char ***targv, long n ) __attribute__ ( ( weak ) );
*/
const duf_longval_extended_table_t optable_interactive = {
  .name = "interactive",
  /* DO_AT_STAGE( DUF_OPTION_STAGE_INTERACTIVE ), */
  .xlist =                      /* */
  {

   /*
    * DO_N: quit
    * DO_A_N: valued: no
    * DO_OC: set: .m_hasoff=1, .m_offset = offsetof(duf_config_t, cli.act.v)
    * DO_FL: .afl.act={.interactive=1}
    * */
#if 0
   {.o = {DO_N( lsfiles ) /*             */ , DO_A_O, DO_V( LSFILES )} /*                                    */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( PDISCCB, pdi ),.call = {.fdesc = {.hi = {.sccb = &duf_print_dir_callbacks}}}, DO_H( pdi cd ) /*      */ },
#endif
   /* {.o = {DO_N( list-sccb ) (*           *) , DO_A_O}, DO_TF( VI_CALL, list_sccb, 0 ) (*                     *) }, */
#if 0
   {.o = {DO_Q( "list-targ" ) /*        */ , DO_A_O, DO_V( LIST_TARG )}, DO_TF( TN1_CALL, list_targ1 ) /*                                    */ },
   {.o = {DO_Q( "clear-targ" ) /*       */ , DO_A_O, DO_V( CLEAR_TARG )}, DO_T( TN1_CALL ), DO_TN1_CALL( clear_targ1 ) /*                    */ },
   {.o = {DO_Q( "add-targ" ) /*         */ , DO_A_O, DO_V( ADD_TARG )}, DO_TF( TS1_CALL, add_targ1 ) /*                                      */ },
   {.o = {DO_Q( "history" ) /*          */ , DO_A_O, DO_V( HISTORY )}, DO_TF( EV_CALL, history ) /*                                          */ },
#else
   {.o = {DO_Q( "list-targ" ) /*        */ , DO_A_O, DO_V( LIST_TARG )}, DO_TN1_CALL( list_targ1 ) /*                                    */ },
   {.o = {DO_Q( "clear-targ" ) /*       */ , DO_A_O, DO_V( CLEAR_TARG )}, DO_TN1_CALL( clear_targ1 ) /*                    */ },
   {.o = {DO_Q( "add-targ" ) /*         */ , DO_A_O, DO_V( ADD_TARG )}, DO_TS1_CALL( add_targ1 ) /*                                      */ },
   {.o = {DO_Q( "history" ) /*          */ , DO_A_O, DO_V( HISTORY )}, DO_EV_CALL( history ) /*                                          */ },
#endif


   {.o = {.name = NULL}}
   }
};



/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
