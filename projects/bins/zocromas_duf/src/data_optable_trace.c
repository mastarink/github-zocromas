#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_config_trace_enum.h"
#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

/*
At duf_xtended_table.c:
  1. extern const muc_longval_extended_table_t optable_trace[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_trace,
	 ...

	NULL
      };
*/

#define DUF_TRACE_OPTT(_unamet,_lname) \
   {.o = {DO_Q( "trace-" #_lname), DO_A_O, DO_V( _unamet  )}, DO_CL( TRACE ), DO_ATT(UPLUS,_lname,DUF_TRACE_LEVEL_ ## _lname)  , DO_STAGE_ALL  , DO_H( trace .... )  }
#define DUF_TRACE_OPT(_uname,_lname) DUF_TRACE_OPTT(_uname ## _TRACE, _lname)

#ifdef MAS_TRACING_OPTIONS
const muc_longval_extended_table_t optable_trace = {
  .name = "trace_t",
  DO_STAGE_ALL,
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "trace-fun-width" ) /* */ , DO_A_O /* */ , DO_V( TRACE_FUN_WIDTH )} /*   */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OT( NUM, fun_width ) /*    */ , DO_AT_STAGE( DEBUG ) /*        */ , DO_H( output fun width ) /*                 */ },

   {.o = {DO_Q( "trace-all" ) /*        */ , DO_A_O /* */ , DO_V( ALL_TRACE )} /*          */ , DO_CL( TRACE ) /*   */
    /*                                                                                      */ , DO_H( trace .... ) /*                       */ },

# define ENUM_WRAPP(_n, _rf, _rf2) \
   DUF_TRACE_OPTT(  _n, _rf2 ),
 /* {.o = {DO_Q( "trace-" #_rf2), DO_A_O, DO_V( _n )}, DO_CL( TRACE ), DO_OT(UPLUS,_rf2)  , DO_SET_STAGE( DEBUG, MAX)  , DO_H( trace # _rf2 )  }, */
# include "duf_optimpl_enum_trace.def"
# undef ENUM_WRAPP

   {.o = {DO_Q( "trace-file" ) /*       */ , DO_A_R /* */ , DO_V( TRACE_FILE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, stream ), DO_AT_STAGE( DEBUG ) /*                     */ , DO_H( trace output to file ) /*                */ },
   {.o = {DO_Q( "trace-overwrite-file" ) /*  */ , DO_A_N /* */ , DO_V( TRACE_FILE_OVERWRITE )} /**/, DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( BFLAG, stream.v ), DO_BFL( output, overwrite ), DO_AT_STAGE( DEBUG ) /**/, DO_H( trace output to file ) /*        */ },
   {.o = {DO_Q( "trace-append-file" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_FILE_APPEND )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( BFLAG, stream.v ), DO_BFL( output, append ), DO_AT_STAGE( DEBUG ) /*  */ , DO_H( trace output to file ) /*        */ },
   {.o = {DO_Q( "trace-stderr" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_STDERR )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, stream ),.call = {.value = {.u = 2}}, DO_AT_STAGE( DEBUG ) /* */ , DO_H( trace output to stderr ) /*      */ },
   {.o = {DO_Q( "trace-stdout" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_STDOUT )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, stream ),.call = {.value = {.u = 1}}, DO_AT_STAGE( DEBUG ) /* */ , DO_H( trace output to stdout ) /*      */ },
   {.o = {DO_Q( "trace-color" ) /*      */ , DO_A_N /* */ } /*                             */ , DO_CL( TRACE ) /* */ ,
    /*      */ DO_OT( NOBFLAG, stream.v ), DO_BFL( output, nocolor ) /*                       */ , DO_H(  ... ) /*                      */ },
   {.o = {DO_Q( "trace-force-color" ) /* */ , DO_A_N /* */ } /*                            */ , DO_CL( TRACE ) /* */ ,
    /* */ DO_OT( BFLAG, stream.v ), DO_BFL( output, force_color ), DO_STAGE_ALL /*            */ , DO_H(  ... ) /*                      */ },
   {.o = {DO_Q( "trace-output-level" ) /* */ , DO_A_O /* */ } /*                           */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( NUM, stream.level ) /* */ , DO_AT_STAGE( DEBUG ) /*                   */ , DO_H( output level ) /*              */ },
   {.o = {.name = NULL}}
   }
};
#endif
/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
