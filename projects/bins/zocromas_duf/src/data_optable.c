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

const duf_longval_extended_t optable_main[] = {
  {.o = {DO_Q( "format" ), /*         */ DO_A_R, /* */ DO_V( FORMAT )} /*         */ , DO_CL( PRINT ) /*   */ , DO_H( print format ) /*      */ },

  {.o = {DO_Q( "include" ), /*        */ DO_A_R, /* */ DO_V( GLOB_INCLUDE_FILES )}, DO_CL( NODESC ) /*     */ , DO_H(  ... ) /*              */ },
  {.o = {DO_Q( "exclude" ), /*        */ DO_A_R, /* */ DO_V( GLOB_EXCLUDE_FILES )}, DO_CL( NODESC ) /*     */ , DO_H(  ... ) /*              */ },




  {.o = {DO_N( quit ), /*             */ DO_A_N} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( NOFLAG, cli.act.v ), DO_FL( act, interactive ), DO_STAGE( 1 ) /*                       */ , DO_H( get dir info ) /*      */ },
  {.o = {DO_N( depth ), /*            */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_OPDI( PDINUM, depth ), DO_STAGE( 1 ) /*                                                */ , DO_H( pdi cd ) /*               */ },
  {.o = {DO_N( maxdepth ), /*         */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_OPDI( PDINUM, maxdepth ), DO_STAGE( 1 ) /*                                                */ , DO_H( pdi cd ) /*            */ },
  {.o = {DO_N( cd ), /*               */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISTR, pdi ), DO_STAGE( 1 ) /*                                                        */ , DO_H( pdi cd ) /*            */ },
  {.o = {DO_N( pwd ), /*              */ DO_A_N} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISTR, pdi ), DO_STAGE( 1 ) /*                                                        */ , DO_H( pdi cd ) /*            */ },

  {.o = {DO_N( lsfiles ), /*          */ DO_A_O} /*                               */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( PDISCCB, pdi ), DO_STAGE( 1 ),.call = {.fdesc =
                                                           {.hi =
                                                            {.sccb = &duf_print_dir_callbacks}}} /*        */ , DO_H( pdi cd ) /*            */ },

  {.o = {DO_Q( "list-sccb" ), /*       */ DO_A_N}, /*                                                      */
   /*      */ DO_T( VIFUN ), DO_VIFUN( duf_list_sccb, 0 ), DO_STAGE( 1 ) /*                                */ , DO_H( help on all ) /*       */ },



  {.o = {DO_Q( "max-rel-depth" ), /*  */ DO_A_R, /* */ DO_V( MAXRELDEPTH )} /*    */ , DO_CL( NODESC ) /*  */ ,
   /*      */ DO_OU( NUM, max_rel_depth ) /*                                                               */ , DO_H(  .... ) /*             */ },
  {.o = {DO_Q( "max-depth" ), /*      */ DO_A_R, /* */ DO_V( MAXRELDEPTH )} /*    */ , DO_CL( NODESC ) /*  */ ,
   /*      */ DO_OU( NUM, max_rel_depth ) /*                                                               */ , DO_H(  .... ) /*             */ },
  {.o = {DO_Q( "max-depth" ), /*      */ DO_A_R, /* */ DO_V( MAXRELDEPTH )} /*    */ , DO_CL( NODESC ) /*  */ ,
   /*      */ DO_OU( PNUM, max_rel_depth ), DO_STAGE( 1 ) /*                                               */ ,
   DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-seq" ), /*        */ DO_A_R, /* */ DO_V( MAXSEQ )} /*         */ , DO_CL( NODESC ) /*   */ ,
   /*      */ DO_OU( NUM, max_seq ) /*                                                                      */ , DO_H(  .... ) /*            */ },
  {.o = {DO_Q( "max-seq" ), /*        */ DO_A_R, /* */ DO_V( MAXSEQ )} /*         */ , DO_CL( NODESC ) /*   */ ,
   /*      */ DO_OU( PNUM, max_seq ), DO_STAGE( 1 ) /*                                                      */ ,
   DO_H(  .... ) /*              */ },


  {.o = {DO_Q( "output-level" ), /*   */ DO_A_O, /* */ DO_V( OUTPUT_LEVEL )} /*   */ , DO_CL( PRINT ) /*   */ ,
   /*      */ DO_O( NUM, cli.output.level ) /*                                                             */ , DO_H( output level ) /*      */ },
  {.o = {DO_Q( "output-file" ), /*    */ DO_A_R, /* */ DO_V( OUTPUT_FILE )} /*    */ , DO_CL( PRINT ) /*   */ ,
   /*      */ DO_O( FILE, cli.output ) /*                                                                  */ , DO_H( output to file ) /*    */ },
  {.o = {DO_Q( "output-stderr" ), /*  */ DO_A_N, /* */ DO_V( OUTPUT_STDERR )} /*  */ , DO_CL( PRINT ) /*   */ ,
   /*      */ DO_O( FILE, cli.output ),.call = {.value =
                                                {.u = 2}} /*                                             */ , DO_H( output to stderr ) /*    */ },
  {.o = {DO_Q( "output-stdout" ), /*  */ DO_A_N, /* */ DO_V( OUTPUT_STDOUT )} /*  */ , DO_CL( PRINT ) /*   */ ,
   /*      */ DO_O( FILE, cli.output ),.call = {.value =
                                                {.u = 1}} /*                                             */ , DO_H( output to stdout ) /*    */ },



  {.o = {DO_Q( "test-option" ), /*    */ DO_A_R, /* */ DO_V( TEST )} /*           */ , DO_CL( DEBUG ) /*   */ , DO_H(  .... ) /*             */ },

  {.o = {.name = NULL}}
};



/* vi: ft=c colorcolumn=3,40,54,84,109,145,146
*/
