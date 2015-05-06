#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
const duf_longval_extended_t optable_sflag[] = {
  {.o = {DO_Q( "disable-calculate" ), DO_A_N, /*    */ DO_VF( DISABLE_CALCULATE )}, DO_CL( COLLECT ) /*    */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_FL( disable, calculate ) /*                                   */ , DO_H(  .... ) /*             */ },
  {.o = {DO_Q( "disable-insert" ), /* */ DO_A_N, /* */ DO_VF( DISABLE_INSERT )}, DO_CL( COLLECT ) /*       */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_FL( disable, insert ) /*                                      */ , DO_H(  .... ) /*             */ },
  {.o = {DO_Q( "disable-update" ), /* */ DO_A_N, /* */ DO_VF( DISABLE_UPDATE )}, DO_CL( COLLECT ) /*       */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_FL( disable, update ) /*                                      */ , DO_H(  .... ) /*             */ },

  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,40,54,84,109,145,146
*/