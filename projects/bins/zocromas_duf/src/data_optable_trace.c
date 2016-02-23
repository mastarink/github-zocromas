#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_trace[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
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
const duf_longval_extended_table_t optable_trace = {
  .name = "trace_t",
  DO_STAGE_ALL,
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "trace-fun-width" ) /* */ , DO_A_O /* */ , DO_V( TRACE_FUN_WIDTH )} /*   */ , DO_CL( PRINT ) /*   */ ,
    /*      */ DO_OT( NUM, fun_width ) /*    */ , DO_AT_STAGE( DEBUG ) /*        */ , DO_H( output fun width ) /*                 */ },

   {.o = {DO_Q( "trace-all" ) /*        */ , DO_A_O /* */ , DO_V( ALL_TRACE )} /*          */ , DO_CL( TRACE ) /*   */
    /*                                                                                      */ , DO_H( trace .... ) /*                       */ },
# if 0
   DUF_TRACE_OPT( TEMP, temp ),
 /* {.o = {DO_Q( "trace-temp" ) (*       *) , DO_A_O (* *) , DO_V( TEMP_TRACE )} (*        *) , DO_CL( TRACE ) (*   *) ,                            */
 /*  (*      *) DO_OT(UPLUS,temp), DO_SET_STAGE( DEBUG, MAX) (*                              *) , DO_H( trace .... ) (*                       *) }, */
   {.o = {DO_Q( "trace-collect" ) /*    */ , DO_A_O /* */ , DO_V( COLLECT_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, collect ) /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-any" ) /*        */ , DO_A_O /* */ , DO_V( ANY_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, any ), DO_SET_STAGE( DEBUG, MAX ) /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-nonew" ) /*      */ , DO_A_O /* */ , DO_V( NONEW_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, nonew ), DO_SET_STAGE( DEBUG, MAX ) /*                             */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-trace" ) /*      */ , DO_A_O /* */ , DO_V( TRACE_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, trace ) /*    */ , DO_STAGE_SPLS /*                   */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-flags" ) /*      */ , DO_A_O /* */ , DO_V( FLAGS_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, flags ) /*    */ , DO_STAGE_SPLS /*                   */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-pdi" ) /*        */ , DO_A_O /* */ , DO_V( PDI_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, pdi ) /*    */ , DO_STAGE_SPLS /*                     */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-levinfo" ) /*    */ , DO_A_O /* */ , DO_V( LEVINFO_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, levinfo ) /*    */ , DO_STAGE_SPLS /*                 */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-dry" ) /*        */ , DO_A_N /* */ , DO_V( DRY_RUN_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, dry_run ) /*   */ , DO_STAGE_SPLS /*                  */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-action" ) /*     */ , DO_A_O /* */ , DO_V( ACTION_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, action ) /*    */ , DO_STAGE_SPLS /*                  */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-sccb" ) /*        */ , DO_A_O /* */ , DO_V( SCCB_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sccb ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                      */ },

   {.o = {DO_Q( "trace-sccbh" ) /*        */ , DO_A_O /* */ , DO_V( SCCBH_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sccbh ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-calls" ) /*      */ , DO_A_O /* */ , DO_V( CALLS_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, calls ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-depth" ) /*      */ , DO_A_O /* */ , DO_V( CALLS_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, depth ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-crc32" ) /*      */ , DO_A_O /* */ , DO_V( CRC32_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, crc32 ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-todo" ) /*       */ , DO_A_O /* */ , DO_V( TODO_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, todo ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-temporary" ) /*  */ , DO_A_O /* */ , DO_V( TEMPORARY_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, temporary ), DO_STAGE_SPLS /*                         */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-deleted" ) /*    */ , DO_A_O /* */ , DO_V( DELETED_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, deleted ), DO_STAGE_SPLS /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-dirent" ) /*     */ , DO_A_O /* */ , DO_V( DIRENT_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, dirent ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-errorr" ) /*      */ , DO_A_O /* */ , DO_V( ERRORR_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, errorr ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-errors" ) /*      */ , DO_A_O /* */ , DO_V( ERRORS_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, errors ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-handle-errors" ) /*  */ , DO_A_O /* */ , DO_V( HERROR_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, handle_error ), DO_STAGE_SPLS /*                      */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-exif" ) /*       */ , DO_A_O /* */ , DO_V( EXIF_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, exif ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-explain" ) /*    */ , DO_A_O /* */ , DO_V( EXPLAIN_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, explain ), DO_STAGE_SPLS /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "explain" ) /*          */ , DO_A_O /* */ , DO_V( EXPLAIN_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, explain ), DO_STAGE_SPLS /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-fs" ) /*         */ , DO_A_O /* */ , DO_V( FS_TRACE )} /*           */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, fs ), DO_STAGE_SPLS /*                                */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-insert" ) /*     */ , DO_A_O /* */ , DO_V( INSERT_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, insert ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-match" ) /*      */ , DO_A_O /* */ , DO_V( MATCH_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, match ), DO_STAGE_SPLS /*                             */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-md5" ) /*        */ , DO_A_O /* */ , DO_V( MD5_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, md5 ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sha1" ) /*        */ , DO_A_O /* */ , DO_V( SHA1_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sha1 ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-mdpath" ) /*     */ , DO_A_O /* */ , DO_V( MDPATH_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, mdpath ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-mime" ) /*       */ , DO_A_O /* */ , DO_V( MIME_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, mime ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-options" ) /*    */ , DO_A_O /* */ , DO_V( OPTIONS_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, options ), DO_STAGE_SPLS /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-config" ) /*     */ , DO_A_O /*  */ , DO_V( CONFIG_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, config ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-path" ) /*       */ , DO_A_O /* */ , DO_V( PATH_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, path ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-mod" ) /*        */ , DO_A_O /* */ , DO_V( MOD_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, mod ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sample" ) /*      */ , DO_A_O /* */ , DO_V( SAMPLE_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sample ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sampupd" ) /*     */ , DO_A_O /* */ , DO_V( SAMPUPD_TRACE )} /*     */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sampupd ), DO_STAGE_SPLS /*                           */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-scandir" ) /*     */ , DO_A_O /* */ , DO_V( SCAN_DIR_TRACE )} /*    */ , DO_CL( TRACE ) /*       */ ,
    /*      */ DO_OT( UPLUS, scan_dir ), DO_STAGE_SPLS /*                          */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-scan" ) /*        */ , DO_A_O /* */ , DO_V( SCAN_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, scan ), DO_STAGE_SPLS /*                              */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-scanreg" ) /*     */ , DO_A_O /* */ , DO_V( SCAN_REG_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, scan_reg ), DO_STAGE_SPLS /*                          */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sd5" ) /*         */ , DO_A_O /* */ , DO_V( SD5_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sd5 ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-select" ) /*      */ , DO_A_O /* */ , DO_V( SELECT_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, select ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-seq" ) /*         */ , DO_A_O /* */ , DO_V( SEQ_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, seq ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-db" ) /*        */ , DO_A_O /* */ , DO_V( DB_TRACE )} /*            */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, db ), DO_STAGE_SPLS /*                                */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sql" ) /*        */ , DO_A_O /* */ , DO_V( SQL_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sql ), DO_STAGE_SPLS /*                               */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-sqlite" ) /*     */ , DO_A_O /* */ , DO_V( SQL_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, sqlite ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-update" ) /*     */ , DO_A_O /* */ , DO_V( UPDATE_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, update ), DO_STAGE_SPLS /*                            */ , DO_H( trace .... ) /*                       */ },
   {.o = {DO_Q( "trace-io" ) /*         */ , DO_A_O /* */ , DO_V( IO_TRACE )} /*           */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( UPLUS, io ), DO_STAGE_SPLS /*                                */ , DO_H( trace .... ) /*                       */ },
# else
#  define ENUM_WRAPP(_n, _rf, _rf2) \
   DUF_TRACE_OPTT(  _n, _rf2 ),
 /* {.o = {DO_Q( "trace-" #_rf2), DO_A_O, DO_V( _n )}, DO_CL( TRACE ), DO_OT(UPLUS,_rf2)  , DO_SET_STAGE( DEBUG, MAX)  , DO_H( trace # _rf2 )  }, */
#  include "duf_options_enum_trace.def"
#  undef ENUM_WRAPP

# endif

   {.o = {DO_Q( "trace-file" ) /*       */ , DO_A_R /* */ , DO_V( TRACE_FILE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, output ), DO_AT_STAGE( DEBUG ) /*                     */ , DO_H( trace output to file ) /*                       */ },
   {.o = {DO_Q( "trace-overwrite-file" ) /*  */ , DO_A_N /* */ , DO_V( TRACE_FILE_OVERWRITE )} /**/, DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FLAG, output.v ), DO_FL( output, overwrite ), DO_AT_STAGE( DEBUG ) /**/, DO_H( trace output to file ) /*               */ },
   {.o = {DO_Q( "trace-append-file" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_FILE_APPEND )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FLAG, output.v ), DO_FL( output, append ), DO_AT_STAGE( DEBUG ) /*  */ , DO_H( trace output to file ) /*               */ },
   {.o = {DO_Q( "trace-stderr" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_STDERR )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, output ),.call = {.value = {.u = 2}}, DO_AT_STAGE( DEBUG ) /* */ , DO_H( trace output to stderr ) /*             */ },
   {.o = {DO_Q( "trace-stdout" ) /*     */ , DO_A_N /* */ , DO_V( TRACE_STDOUT )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OT( FILE, output ),.call = {.value = {.u = 1}}, DO_AT_STAGE( DEBUG ) /* */ , DO_H( trace output to stdout ) /*             */ },
   {.o = {.name = NULL}}
   }
};
#endif
/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
