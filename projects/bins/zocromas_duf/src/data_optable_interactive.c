#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"

extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;

void duf_list_sccb( int ) __attribute__ ( ( weak ) );

const duf_longval_extended_t optable_interactive[] = {

  {.o = {DO_N( quit ), /*             */ DO_A_N} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( NOFLAG, cli.act.v ), DO_FL( act, interactive ), DO_STAGE( 1 ) /*                       */ , DO_H( get dir info ) /*      */ },
  {.o = {DO_N( cd ), /*               */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISTR, pdi ), DO_STAGE( 1 ) /*                                                        */ , DO_H( pdi cd ) /*            */ },
  {.o = {DO_N( pwd ), /*              */ DO_A_N} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISTR, pdi ), DO_STAGE( 1 ) /*                                                        */ , DO_H( pdi cd ) /*            */ },

  {.o = {DO_N( lsfiles ), /*          */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISCCB, pdi ), DO_STAGE( 1 ),.call = {.fdesc = {.hi = {.sccb = &duf_print_dir_callbacks}}}, DO_H( pdi cd ) /*           */ },

  {.o = {DO_Q( "list-sccb" ), /*       */ DO_A_N}, DO_T( VIFUN ), DO_VIFUN( duf_list_sccb, 0 ), DO_STAGE( 1 )},


  {.o = {.name = NULL}}
};



/* vi: ft=c colorcolumn=3,40,54,84,109,145,146
*/

