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
  1. extern const duf_longval_extended_table_t optable_trace[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_trace,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_trace = {
  .table =                      /* */
  {
   {.o = {DO_Q( "trace-all" ) /*             */ , DO_A_O /* */ , DO_V( ALL_TRACE )} /*     */ , DO_CL( TRACE ) /*   */
    /*                                                                                      */ , DO_H( trace .... ) /*                       */ },

   {.o = {DO_Q( "trace-dry" ) /*        */ , DO_A_N /* */ , DO_V( DRY_RUN_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.dry_run ) /*   */ , DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-action" ) /*     */ , DO_A_O /* */ , DO_V( ACTION_TRACE )} /*   */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.action ) /*    */ , DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-any" ) /*        */ , DO_A_O /* */ , DO_V( ANY_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.any ), DO_STAGE_ANY /*                                            */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-calls" ) /*      */ , DO_A_O /* */ , DO_V( CALLS_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.calls ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-depth" ) /*      */ , DO_A_O /* */ , DO_V( CALLS_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.depth ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-collect" ) /*    */ , DO_A_O /* */ , DO_V( COLLECT_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.collect ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-crc32" ) /*      */ , DO_A_O /* */ , DO_V( CRC32_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.crc32 ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-current" ) /*    */ , DO_A_O /* */ , DO_V( CURRENT_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.current ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-deleted" ) /*    */ , DO_A_O /* */ , DO_V( DELETED_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.deleted ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-dirent" ) /*     */ , DO_A_O /* */ , DO_V( DIRENT_TRACE )} /*   */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.dirent ), DO_STAGE_ANY /*                                         */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-error" ) /*      */ , DO_A_O /* */ , DO_V( ERROR_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.error ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-exif" ) /*       */ , DO_A_O /* */ , DO_V( EXIF_TRACE )} /*     */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.exif ), DO_STAGE_ANY /*                                           */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-explain" ) /*    */ , DO_A_O /* */ , DO_V( EXPLAIN_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.explain ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "explain" ) /*          */ , DO_A_O /* */ , DO_V( EXPLAIN_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.explain ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-fs" ) /*         */ , DO_A_O /* */ , DO_V( FS_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.fs ), DO_STAGE_ANY /*                                             */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-insert" ) /*     */ , DO_A_O /* */ , DO_V( INSERT_TRACE )} /*   */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.insert ), DO_STAGE_ANY /*                                         */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-integrity" ) /*  */ , DO_A_O /* */ , DO_V( INTEGRITY_TRACE )} /* */ , DO_CL( TRACE ) /* */ ,
    /*      */ DO_OC( UPLUS, cli.trace.integrity ), DO_STAGE_ANY /*                                      */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-match" ) /*      */ , DO_A_O /* */ , DO_V( MATCH_TRACE )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.match ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-md5" ) /*        */ , DO_A_O /* */ , DO_V( MD5_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.md5 ), DO_STAGE_ANY /*                                            */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-mdpath" ) /*     */ , DO_A_O /* */ , DO_V( MDPATH_TRACE )} /*   */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.mdpath ), DO_STAGE_ANY /*                                         */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-mime" ) /*       */ , DO_A_O /* */ , DO_V( MIME_TRACE )} /*     */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.mime ), DO_STAGE_ANY /*                                           */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-nonew" ) /*      */ , DO_A_O /* */ , DO_V( TRACE_NONEW )} /*    */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.nonew ), DO_STAGE_ANY /*                                          */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-options" ) /*    */ , DO_A_O /* */ , DO_V( OPTIONS_TRACE )} /*  */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.options ), DO_STAGE_ANY /*                                        */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-config" ) /*     */ , DO_A_O /*  */ , DO_V( CONFIG_TRACE )} /*       */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.config ), DO_STAGE_ANY /*                         */ , DO_H( trace .... ) /*                        */ },
   {.o = {DO_Q( "trace-path" ) /*       */ , DO_A_O /* */ , DO_V( PATH_TRACE )} /*     */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.path ), DO_STAGE_ANY /*                                           */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-mod" ) /*        */ , DO_A_O /* */ , DO_V( MOD_TRACE )} /*          */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.mod ), DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-sample" ) /*      */ , DO_A_O /* */ , DO_V( SAMPLE_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.sample ), DO_STAGE_ANY /*                            */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-sampupd" ) /*     */ , DO_A_O /* */ , DO_V( SAMPUPD_TRACE )} /*     */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.sampupd ), DO_STAGE_ANY /*                           */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-scandir" ) /*     */ , DO_A_O /* */ , DO_V( SCAN_DE_DIR_TRACE )}, DO_CL( TRACE ) /*       */ ,
    /*      */ DO_OC( UPLUS, cli.trace.scan_de_dir ), DO_STAGE_ANY /*                       */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-scan" ) /*        */ , DO_A_O /* */ , DO_V( SCAN_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.scan ), DO_STAGE_ANY /*                              */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-scanreg" ) /*     */ , DO_A_O /* */ , DO_V( SCAN_DE_REG_TRACE )} /* */ , DO_CL( TRACE ) /*       */ ,
    /*      */ DO_OC( UPLUS, cli.trace.scan_de_reg ), DO_STAGE_ANY /*                       */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-sd5" ) /*         */ , DO_A_O /* */ , DO_V( SD5_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.sd5 ), DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-select" ) /*      */ , DO_A_O /* */ , DO_V( SELECT_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.select ), DO_STAGE_ANY /*                            */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-seq" ) /*         */ , DO_A_O /* */ , DO_V( SEQ_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.seq ), DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-sql" ) /*         */ , DO_A_O /* */ , DO_V( SQL_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.sql ), DO_STAGE_ANY /*                               */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-sqlite" ) /*      */ , DO_A_O /* */ , DO_V( SQL_TRACE )} /*         */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.sqlite ), DO_STAGE_ANY /*                            */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-temp" ) /*        */ , DO_A_O /* */ , DO_V( TEMP_TRACE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.temp ), DO_STAGE_ANY /*                              */ , DO_H( trace .... ) /*        */ },
   {.o = {DO_Q( "trace-update" ) /*      */ , DO_A_O /* */ , DO_V( UPDATE_TRACE )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( UPLUS, cli.trace.update ), DO_STAGE_ANY /*                            */ , DO_H( trace .... ) /*        */ },

   {.o = {DO_Q( "trace-file" ) /*        */ , DO_A_R /* */ , DO_V( TRACE_FILE )} /*        */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( FILE, cli.trace.output ), DO_STAGE_ANY /*                             */ , DO_H( trace output to file )},
   {.o = {DO_Q( "trace-stderr" ) /*      */ , DO_A_N /* */ , DO_V( TRACE_STDERR )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( FILE, cli.trace.output ),.call = {.value = {.u = 2}} /*               */ , DO_H( trace output to stderr )},
   {.o = {DO_Q( "trace-stdout" ) /*      */ , DO_A_N /* */ , DO_V( TRACE_STDOUT )} /*      */ , DO_CL( TRACE ) /*   */ ,
    /*      */ DO_OC( FILE, cli.trace.output ),.call = {.value = {.u = 1}}, DO_STAGE_ANY /* */ , DO_H( trace .... )},

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
