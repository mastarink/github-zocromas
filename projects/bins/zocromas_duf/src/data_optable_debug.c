#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
const duf_longval_extended_t optable_debug[] = {
  {.o = {DO_Q( "min-dbg-lines" ), /*  */ DO_A_R, /* */ DO_V( MIN_DBGLINE )} /*    */ , DO_CL( DEBUG ) /*   */ ,
   /*      */ DO_O( NUM, cli.dbg.min_line ) /*                                                             */ , DO_H(  ... ) /*              */ },
  {.o = {DO_Q( "max-dbg-lines" ), /*  */ DO_A_R, /* */ DO_V( MAX_DBGLINE )} /*    */ , DO_CL( DEBUG ) /*   */ ,
   /*      */ DO_O( NUM, cli.dbg.max_line ) /*                                                             */ , DO_H(  ... ) /*              */ },
  {.o = {DO_Q( "debug" ), /*          */ DO_A_O, /* */ DO_V( DEBUG )} /*          */ , DO_CL( DEBUG ) /*   */ ,
   /*      */ DO_O( UPLUS, cli.dbg.debug ) /*                                                              */ , DO_H( debug ) /*             */ },
  {.o = {DO_Q( "verbose" ), /*        */ DO_A_O, /* */ DO_V( VERBOSE )} /*        */ , DO_CL( DEBUG ) /*   */ ,
   /*      */ DO_O( UPLUS, cli.dbg.verbose ) /*                                                            */ , DO_H( verbose ) /*           */ },

  {.o = {DO_Q( "memusage" ), /*       */ DO_A_N, /* */ DO_V( MEMUSAGE )} /*       */ , DO_CL( DEBUG ) /*   */ , DO_H( debug ) /*             */ },


  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,40,54,84,109,145,146
*/
