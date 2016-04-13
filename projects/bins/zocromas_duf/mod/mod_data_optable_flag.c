#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_mod_types.h"

#include "duf_ufilter_structs.h"


/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_flag[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_flag,
	 ...

	NULL
      };
*/
static muc_longval_extended_table_t optable;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"optab", &optable},
  {NULL, NULL}
};

static muc_longval_extended_table_t optable = {
  .name = "flag",
  .xlist =                                                           /* */
  {
#if 0
 /* SEE --disable-memusage */
   {.o = {DO_Q( "memusage" ) /*          */ , DO_A_N /* */ , DO_V( MEMUSAGE )} /*   */ , DO_CL( DEBUG ) /*   */ , DO_H( debug ) /*        */ },
#endif

   {.o = {DO_Q( "quit" ) /*             */ , DO_A_N /* */ , DO_VF( QUIT )} /*              */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( NOBFLAG, opt.act.v ), DO_BFL( act, interactive ) /*                   */ , DO_H( quit ) /*                             */ },
   {.o = {DO_Q( "add-path" ) /*         */ , DO_A_N /* */ , DO_VF( ADD_PATH )} /*          */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, add_path ) /*                        */ , DO_H( reg.init path ) /*                    */ },
   {.o = {DO_Q( "interactive" ) /*      */ , DO_A_N /* */ , DO_VF( INTERACTIVE )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, interactive ), DO_STAGE_SPLS /*      */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "force-color" ) /*      */ , DO_A_N /* */ , DO_VF( FORCE_COLOR )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /* */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, force_color ), DO_STAGE_ALL /*  */ , DO_H(  ... ) /*                      */ },

/* DO_OC(NOBBFLAG, ...) + DO_BFL eq. to DO_OC(FLAG, ...) + DO_BFN */
   {.o = {DO_Q( "color" ) /*            */ , DO_A_N /* */ , DO_VF( NOCOLOR )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( NOBFLAG, opt.output.stream.v ), DO_BFL( output, nocolor ) /*                   */ , DO_H(  ... ) /*                      */ },
   {.o = {DO_Q( "escapecoloring" ) /*   */ , DO_A_N /* */ , DO_VF( NOCOLOR )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFN( output, nocolor ) /*                     */ , DO_H(  ... ) /*                      */ },

 /* {.o = {DO_Q( "use-binformat" ) (*        *) , DO_A_N (* *) , DO_VF( USE_BINFORMAT )} (* *) , DO_CL( PRINT ) (*   *) ,                    */
 /*  (*      *) DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, use_binformat ) (*               *) , DO_H( use bin format ) (*     *) }, */
   {.o = {DO_Q( "progress" ) /*         */ , DO_A_N /* */ , DO_VF( PROGRESS )} /*          */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, progress ) /*             */ , DO_H( progress ) /*                         */ },

   {.o = {DO_Q( "fast" ) /*             */ , DO_A_N /* */ , DO_VF( FAST )} /*              */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, fast ) /*                              */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "fresh" ) /*            */ , DO_A_N /* */ , DO_VF( FRESH )} /*             */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, fresh ) /*                             */ , DO_HQ( "only process for unset data" ) /*   */ },
   {.o = {DO_Q( "recursive" ) /*        */ , DO_A_N /* */ , DO_VF( RECURSIVE )} /*         */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( puz, recursive ), DO_STAGE_SPLS /*             */ , DO_H( recursive ) /*                        */ },

#if 0
   {.o = {DO_Q( "linear" ) /*           */ , DO_A_N /* */ , DO_VF( LINEAR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( puz, linear ), DO_STAGE_SPLS /*                */ , DO_H( linear ) /*                           */ },
#else
   {.o = {DO_Q( "linear" ) /*           */ , DO_A_N /* */ , DO_VF( LINEAR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( puz, linear ), DO_STAGE_SPLS /*              */ , DO_H( linear ) /*                           */ },
#endif

   {.o = {DO_Q( "dirent" ) /*           */ , DO_A_N /* */ , DO_VF( DIRENT )} /*            */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, dirent ) /*                            */ , DO_H( dirent ) /*                           */ },

   {.o = {DO_Q( "info" ) /*             */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "information" ) /*      */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "dry-run" ) /*          */ , DO_A_N /* */ , DO_VF( DRY_RUN )} /*           */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( BFLAG, opt.flow ), DO_BFL( flow, dry_run ) /*                               */ ,
    DO_H( dry ) /*                              */ },

   {.o = {DO_Q( "summary" ) /*          */ , DO_A_N /* */ , DO_VF( SUMMARY )} /*           */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, summary ) /*                           */ , DO_H( summary ) /*                          */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
