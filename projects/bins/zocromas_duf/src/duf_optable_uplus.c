#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
const duf_longval_extended_t optable_uplus[] = {

  {.o = {DO_Q( "sample" ), /*         */ DO_A_O, /* */ DO_V( SAMPLE )} /*         */ , DO_CL( NODESC ) /*  */ ,
   /*      */ DO_O( UPLUS, cli.act.sample ) /*                                                             */ , DO_H(  .... ) /*             */ },
  {.o = {DO_Q( "sampupd" ), /*        */ DO_A_O, /* */ DO_V( SAMPUPD )} /*        */ , DO_CL( NODESC ) /*  */ ,
   /*      */ DO_O( UPLUS, cli.act.sampupd ) /*                                                            */ , DO_H(  .... ) /*             */ },

  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,40,54,84,109,145,146
*/
