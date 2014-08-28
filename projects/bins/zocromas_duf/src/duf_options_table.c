#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

#include "duf_config_types.h"
#include "duf_option_types.h"
#include "duf_options_def.h"
#include "duf_cli_types.h"

#define DO_CL( cl )  .oclass = DUF_OPTION_CLASS_ ## cl
#define DO_V( vid )  .val = DUF_OPTION_ ## vid
#define DO_VF( vid )  .val = DUF_OPTION_FLAG_ ## vid
#define DO_A_O .has_arg = optional_argument
#define DO_A_R .has_arg = required_argument
#define DO_A_N .has_arg = no_argument
#define DO_N( n ) .name = # n
#define DO_Q( n ) .name = n
#define DO_H( h ) .help = # h
#define DO_O(vt, v) .vtype=DUF_OPTION_VTYPE_ ## vt,.mf=1,.m = offsetof(duf_config_t,v)
#define DO_F1(t,fld) .afl.t={.fld=1}
/* man getopt_long */

static const duf_longval_extended_t _lo_extended[] = {
  {.o = {DO_Q( "add-path" ), /*       */ DO_A_N, /* */ DO_VF( ADD_PATH )} /*      */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, add_path ) /*                                             */ , DO_H( reg.init path ) /*      */ },
  {.o = {DO_Q( "collect" ), /*        */ DO_A_N, /* */ DO_VF( COLLECT )} /*       */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, collect ) /*                                              */ , DO_H( collect mode ) /*       */ },
  {.o = {DO_Q( "crc32" ), /*          */ DO_A_N, /* */ DO_VF( CRC32 )} /*         */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, crc32 ) /*                                                */ , DO_H( collect crc32 data ) /* */ },
  {.o = {DO_Q( "crc32id" ), /*        */ DO_A_R, /* */ DO_V( CRC32ID )} /*        */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "create-tables" ), /*  */ DO_A_N, /* */ DO_VF( CREATE_TABLES )} /* */ , DO_CL( SYSTEM ) /*    */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, create_tables ) /*                                        */ , DO_H( create tables ) /*      */ },
  {.o = {DO_Q( "db-directory" ), /*   */ DO_A_R, /* */ DO_V( DB_DIRECTORY )} /*   */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( STR, db.dir ) /*                                                                         */ , DO_H( db directory ) /*       */ },
  {.o = {DO_Q( "db-name-adm" ), /*    */ DO_A_R, /* */ DO_V( DB_NAME_ADM )} /*    */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( STR, db.adm.name ) /*                                                                    */ , DO_H( db name adm ) /*        */ },
  {.o = {DO_Q( "db-name" ), /*        */ DO_A_R, /* */ DO_V( DB_NAME_MAIN )} /*   */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( STR, db.main.name ) /*                                                                   */ , DO_H( db name main ) /*       */ },
  {.o = {DO_Q( "debug" ), /*          */ DO_A_O, /* */ DO_V( DEBUG )} /*          */ , DO_CL( DEBUG ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.dbg.debug ) /*                                                                */ , DO_H( verbose ) /*            */ },
  {.o = {DO_Q( "dirent" ), /*         */ DO_A_N, /* */ DO_VF( DIRENT )} /*        */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, dirent ) /*                                               */ , DO_H( dirent ) /*             */ },
  {.o = {DO_Q( "directories" ), /*    */ DO_A_N, /* */ DO_VF( DIRS )} /*          */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, dirs ) /*                                                 */ , DO_H( collect dir info ) /*   */ },
  {.o = {DO_Q( "dirs" ), /*           */ DO_A_N, /* */ DO_VF( DIRS )} /*          */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, dirs ) /*                                                 */ , DO_H( get dir info ) /*       */ },
  {.o = {DO_Q( "disable-calculate" ), DO_A_N, /*    */ DO_VF( DISABLE_CALCULATE )}, DO_CL( COLLECT ) /*      */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_F1( disable, calculate ) /*                                      */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "disable-insert" ), /* */ DO_A_N, /* */ DO_VF( DISABLE_INSERT )}, DO_CL( COLLECT ) /*         */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_F1( disable, insert ) /*                                         */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "disable-update" ), /* */ DO_A_N, /* */ DO_VF( DISABLE_UPDATE )}, DO_CL( COLLECT ) /*         */ ,
   /*      */ DO_O( SFLAG, cli.disable ), DO_F1( disable, update ) /*                                         */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "drop-tables" ), /*    */ DO_A_N, /* */ DO_VF( DROP_TABLES )} /*   */ , DO_CL( SYSTEM ) /*    */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, drop_tables ) /*                                          */ , DO_H( drop tables ) /*        */ },
  {.o = {DO_Q( "exif" ), /*           */ DO_A_N, /* */ DO_VF( EXIF )} /*          */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, exif ) /*                                                 */ , DO_H( collect exif data ) /*  */ },
  {.o = {DO_Q( "exifid" ), /*         */ DO_A_R, /* */ DO_V( EXIFID )} /*         */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "explain" ), /*        */ DO_A_O, /* */ DO_V( EXPLAIN_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "filedata" ), /*       */ DO_A_N, /* */ DO_VF( FILEDATA )} /*      */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, filedata ) /*                                             */ , DO_H( get filedata ) /*       */ },
  {.o = {DO_Q( "filenames" ), /*      */ DO_A_N, /* */ DO_VF( FILENAMES )} /*     */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, filenames ) /*                                            */ , DO_H( get filenames ) /*      */ },
  {.o = {DO_Q( "files" ), /*          */ DO_A_N, /* */ DO_VF( FILES )} /*         */ , DO_CL( COLLECT ) /*   */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, files ) /*                                                */ , DO_H( get file info ) /*      */ },
  {.o = {DO_Q( "format" ), /*         */ DO_A_R, /* */ DO_V( FORMAT )} /*         */ , DO_CL( PRINT ) /*     */ , DO_H( print format ) /*       */ },

  {.o = {DO_Q( "help-all" ), /*       */ DO_A_N, /* */ DO_V( HELP_ALL )} /*       */ , DO_CL( HELP ) /*      */ , DO_H( help on all ) /*        */ },
  {.o = {DO_Q( "help-collect" ), /*   */ DO_A_N, /* */ DO_V( HELP_COLLECT )} /*   */ , DO_CL( HELP ) /*      */ , DO_H( help on collect ) /*    */ },
  {.o = {DO_Q( "help-control" ), /*   */ DO_A_N, /* */ DO_V( HELP_CONTROL )} /*   */ , DO_CL( HELP ) /*      */ , DO_H( help on control ) /*    */ },
  {.o = {DO_Q( "help-debug" ), /*     */ DO_A_N, /* */ DO_V( HELP_DEBUG )} /*     */ , DO_CL( HELP ) /*      */ , DO_H( help on debug ) /*      */ },
  {.o = {DO_Q( "help-examples" ), /*  */ DO_A_N, /* */ DO_V( EXAMPLES )} /*       */ , DO_CL( HELP ) /*      */ , DO_H( examples ) /*           */ },
  {.o = {DO_Q( "help" ), /*           */ DO_A_O, /* */ DO_V( SMART_HELP )} /*     */ , DO_CL( HELP ) /*      */ , DO_H( smart help ) /*         */ },
  {.o = {DO_Q( "help-help" ), /*      */ DO_A_N, /* */ DO_V( HELP_HELP )} /*      */ , DO_CL( HELP ) /*      */ , DO_H( help on help ) /*       */ },
  {.o = {DO_Q( "help-nodesc" ), /*    */ DO_A_N, /* */ DO_V( HELP_NODESC )} /*    */ , DO_CL( HELP ) /*      */ , DO_H( help on nodesc ) /*     */ },
  {.o = {DO_Q( "help-obsolete" ), /*  */ DO_A_N, /* */ DO_V( HELP_OBSOLETE )} /*  */ , DO_CL( HELP ) /*      */ , DO_H( help on obsolete ) /*   */ },
  {.o = {DO_Q( "help-none" ), /*      */ DO_A_N, /* */ DO_V( HELP_NONE )} /*      */ , DO_CL( HELP ) /*      */ , DO_H( help on none ) /*       */ },
  {.o = {DO_Q( "help-print" ), /*     */ DO_A_N, /* */ DO_V( HELP_PRINT )} /*     */ , DO_CL( HELP ) /*      */ , DO_H( help on print ) /*      */ },
  {.o = {DO_Q( "help-reference" ), /* */ DO_A_N, /* */ DO_V( HELP_REFERENCE )} /* */ , DO_CL( HELP ) /*      */ , DO_H( help on reference ) /*  */ },
  {.o = {DO_Q( "help-request" ), /*   */ DO_A_N, /* */ DO_V( HELP_REQUEST )} /*   */ , DO_CL( HELP ) /*      */ , DO_H( help on request ) /*    */ },
  {.o = {DO_Q( "help-scan" ), /*      */ DO_A_N, /* */ DO_V( HELP_SCAN )} /*      */ , DO_CL( HELP ) /*      */ , DO_H( help on scan ) /*       */ },
  {.o = {DO_Q( "help-short" ), /*     */ DO_A_N, /* */ DO_V( HELP )} /*           */ , DO_CL( HELP ) /*      */ , DO_H( help ) /*               */ },
  {.o = {DO_Q( "help-system" ), /*    */ DO_A_N, /* */ DO_V( HELP_SYSTEM )} /*    */ , DO_CL( HELP ) /*      */ , DO_H( help on system ) /*     */ },
  {.o = {DO_Q( "help-trace" ), /*     */ DO_A_N, /* */ DO_V( HELP_TRACE )} /*     */ , DO_CL( HELP ) /*      */ , DO_H( help on trace ) /*      */ },
  {.o = {DO_Q( "help-update" ), /*    */ DO_A_N, /* */ DO_V( HELP_UPDATE )} /*    */ , DO_CL( HELP ) /*      */ , DO_H( help on update ) /*     */ },

  {.o = {DO_Q( "include" ), /*        */ DO_A_R, /* */ DO_V( GLOB_INCLUDE_FILES )}, DO_CL( NODESC ) /*       */ , DO_H(  ... ) /*               */ },
  {.o = {DO_Q( "exclude" ), /*        */ DO_A_R, /* */ DO_V( GLOB_EXCLUDE_FILES )}, DO_CL( NODESC ) /*       */ , DO_H(  ... ) /*               */ },
  {.o = {DO_Q( "info" ), /*           */ DO_A_N, /* */ DO_VF( INFO )} /*          */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, info ) /*                                                 */ , DO_H( db info ) /*            */ },
  {.o = {DO_Q( "information" ), /*    */ DO_A_N, /* */ DO_VF( INFO )} /*          */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, info ) /*                                                 */ , DO_H( db info ) /*            */ },
  {.o = {DO_Q( "dry-run" ), /*        */ DO_A_N, /* */ DO_VF( DRY_RUN )} /*       */ , DO_CL( DEBUG ) /* */ ,
   /*      */ DO_O( FLAG, cli.v ), DO_F1( cli, dry_run ) /*                                                  */ , DO_H( dry ) /*            */ },
  {.o = {DO_Q( "integrity" ), /*      */ DO_A_N, /* */ DO_VF( INTEGRITY )} /*     */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, integrity ) /*                                            */ , DO_H( db integrity ... ) /*   */ },
  {.o = {DO_Q( "min-dbg-lines" ), /*  */ DO_A_R, /* */ DO_V( MIN_DBGLINE )} /*    */ , DO_CL( DEBUG ) /*     */ ,
   /*      */ DO_O( NUM, cli.dbg.min_line ) /*                                                               */ , DO_H( verbose ) /*            */ },
  {.o = {DO_Q( "max-dbg-lines" ), /*  */ DO_A_R, /* */ DO_V( MAX_DBGLINE )} /*    */ , DO_CL( DEBUG ) /*     */ ,
   /*      */ DO_O( NUM, cli.dbg.max_line ) /*                                                               */ , DO_H( verbose ) /*            */ },
  {.o = {DO_Q( "max-rel-depth" ), /*  */ DO_A_R, /* */ DO_V( MAXRELDEPTH )} /*    */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.max_rel_depth ) /*                                                                */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-depth" ), /*      */ DO_A_R, /* */ DO_V( MAXRELDEPTH )} /*    */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.max_rel_depth ) /*                                                                */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-dirfiles" ), /*   */ DO_A_R, /* */ DO_V( MAXDIRFILES )} /*    */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-dirs" ), /*       */ DO_A_R, /* */ DO_V( MAXITEMS_DIRS )} /*  */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-files" ), /*      */ DO_A_R, /* */ DO_V( MAXITEMS_FILES )} /* */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-items" ), /*      */ DO_A_R, /* */ DO_V( MAXITEMS )} /*       */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "min-same" ), /*       */ DO_A_R, /* */ DO_V( MINSAME )} /*        */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.same.min ) /*                                                                   */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-same" ), /*       */ DO_A_R, /* */ DO_V( MAXSAME )} /*        */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.same.max ) /*                                                                   */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "same" ), /*           */ DO_A_R, /* */ DO_V( SAME )} /*           */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( MINMAX, u.same.min ) /*                                                                */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-seq" ), /*        */ DO_A_R, /* */ DO_V( MAXSEQ )} /*         */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.max_seq ) /*                                                                      */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "size" ), /*           */ DO_A_R, /* */ DO_V( SIZE )} /*           */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( MINMAXLL, u.size.min ) /*                                                                */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "min-size" ), /*       */ DO_A_R, /* */ DO_V( MINSIZE )} /*        */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.size.min ) /*                                                                     */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "max-size" ), /*       */ DO_A_R, /* */ DO_V( MAXSIZE )} /*        */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( NUM, u.size.max ) /*                                                                     */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "md5" ), /*            */ DO_A_N, /* */ DO_VF( MD5 )} /*           */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, md5 ) /*                                                  */ , DO_H( collect md5 data ) /*   */ },
  {.o = {DO_Q( "md5id" ), /*          */ DO_A_R, /* */ DO_V( MD5ID )} /*          */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "mdpath" ), /*         */ DO_A_N, /* */ DO_VF( MDPATH )} /*        */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, mdpath ) /*                                               */ , DO_H(  ... ..... ) /*         */ },
  {.o = {DO_Q( "memusage" ), /*       */ DO_A_N, /* */ DO_V( MEMUSAGE )} /*       */ , DO_CL( DEBUG ) /*     */ , DO_H( verbose ) /*            */ },
  {.o = {DO_Q( "mime" ), /*           */ DO_A_N, /* */ DO_VF( MIME )} /*          */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, mime ) /*                                                 */ , DO_H( collect mime data ) /*  */ },
  {.o = {DO_Q( "mimeid" ), /*         */ DO_A_R, /* */ DO_V( MIMEID )} /*         */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "min-dirfiles" ), /*   */ DO_A_R, /* */ DO_V( MINDIRFILES )} /*    */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "output-file" ), /*    */ DO_A_R, /* */ DO_V( OUTPUT_FILE )} /*    */ , DO_CL( PRINT ) /*     */ , DO_H( output to file ) /*     */ },
  {.o = {DO_Q( "output-level" ), /*   */ DO_A_O, /* */ DO_V( OUTPUT_LEVEL )} /*   */ , DO_CL( PRINT ) /*     */ , DO_H( output level ) /*       */ },
  {.o = {DO_Q( "print" ), /*          */ DO_A_N, /* */ DO_VF( PRINT )} /*         */ , DO_CL( PRINT ) /*     */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, print ) /*                                                */ , DO_H( print ) /*              */ },
  {.o = {DO_Q( "progress" ), /*       */ DO_A_N, /* */ DO_VF( PROGRESS )} /*      */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, progress ) /*                                             */ , DO_H( progress ) /*           */ },
  {.o = {DO_Q( "recursive" ), /*      */ DO_A_N, /* */ DO_VF( RECURSIVE )} /*     */ , DO_CL( CONTROL ) /*   */ ,
   /*      */ DO_O( SFLAG, u.v ), DO_F1( u, recursive ) /*                                                    */ , DO_H( recursive ) /*          */ },
  {.o = {DO_Q( "remove-database" ), /* */ DO_A_N, /* */ DO_VF( REMOVE_DATABASE )}, DO_CL( SYSTEM ) /*        */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, remove_database ) /*                                      */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "sample" ), /*         */ DO_A_O, /* */ DO_V( SAMPLE )} /*         */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( UPLUS, cli.act.sample ) /*                                                            */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "sampupd" ), /*        */ DO_A_O, /* */ DO_V( SAMPUPD )} /*        */ , DO_CL( NODESC ) /*    */ ,
   /*      */ DO_O( UPLUS, cli.act.sampupd ) /*                                                            */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "sd5" ), /*            */ DO_A_N, /* */ DO_VF( SD5 )} /*           */ , DO_CL( SCAN ) /*      */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, sd5 ) /*                                                  */ , DO_H( collect sd5 data ) /*   */ },
  {.o = {DO_Q( "sd5id" ), /*          */ DO_A_R, /* */ DO_V( SD5ID )} /*          */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "summary" ), /*        */ DO_A_N, /* */ DO_VF( SUMMARY )} /*       */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, summary ) /*                                              */ , DO_H( summary ) /*            */ },

  {.o = {DO_Q( "test-option" ), /*    */ DO_A_R, /* */ DO_V( TEST )} /*           */ , DO_CL( DEBUG ) /*     */ , DO_H(  .... ) /*              */ },

  {.o = {DO_Q( "trace-all" ), /*      */ DO_A_O, /* */ DO_V( ALL_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },

  {.o = {DO_Q( "trace-dry" ), /*      */ DO_A_N, /* */ DO_V( DRY_RUN_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.dry_run ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-action" ), /*   */ DO_A_O, /* */ DO_V( ACTION_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.action ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-any" ), /*      */ DO_A_O, /* */ DO_V( ANY_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.any ) /*                                                                */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-calls" ), /*    */ DO_A_O, /* */ DO_V( CALLS_TRACE )} /*    */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.calls ) /*                                                              */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-collect" ), /*  */ DO_A_O, /* */ DO_V( COLLECT_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.collect ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-crc32" ), /*    */ DO_A_O, /* */ DO_V( CRC32_TRACE )} /*    */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.crc32 ) /*                                                              */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-current" ), /*  */ DO_A_O, /* */ DO_V( CURRENT_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.current ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-deleted" ), /*  */ DO_A_O, /* */ DO_V( DELETED_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.deleted ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-dirent" ), /*   */ DO_A_O, /* */ DO_V( DIRENT_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.dirent ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-error" ), /*    */ DO_A_O, /* */ DO_V( ERROR_TRACE )} /*    */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.error ) /*                                                              */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-exif" ), /*     */ DO_A_O, /* */ DO_V( EXIF_TRACE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.exif ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-explain" ), /*  */ DO_A_O, /* */ DO_V( EXPLAIN_TRACE )} /*  */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.explain ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-file" ), /*     */ DO_A_R, /* */ DO_V( TRACE_FILE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.file ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-fs" ), /*       */ DO_A_O, /* */ DO_V( FS_TRACE )} /*       */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.fs ) /*                                                                 */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-insert" ), /*   */ DO_A_O, /* */ DO_V( INSERT_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.insert ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-integrity" ), /* */ DO_A_O, /* */ DO_V( INTEGRITY_TRACE )} /* */ , DO_CL( TRACE ) /*   */ ,
   /*      */ DO_O( UPLUS, cli.trace.integrity ) /*                                                          */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-match" ), /*    */ DO_A_O, /* */ DO_V( MATCH_TRACE )} /*    */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.match ) /*                                                              */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-md5" ), /*      */ DO_A_O, /* */ DO_V( MD5_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.md5 ) /*                                                                */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-mdpath" ), /*   */ DO_A_O, /* */ DO_V( MDPATH_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.mdpath ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-mime" ), /*     */ DO_A_O, /* */ DO_V( MIME_TRACE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.mime ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-nonew" ), /*    */ DO_A_O, /* */ DO_V( TRACE_NONEW )} /*    */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.nonew ) /*                                                              */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-options" ), /*  */ DO_A_O, /* */ DO_V( OPTIONS_TRACE )} /*  */ , DO_CL( TRACE ) /*    */ ,
   /*      */ DO_O( UPLUS, cli.trace.options ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-path" ), /*     */ DO_A_O, /* */ DO_V( PATH_TRACE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.path ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-sample" ), /*   */ DO_A_O, /* */ DO_V( SAMPLE_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.sample ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-sampupd" ), /*   */ DO_A_O, /* */ DO_V( SAMPUPD_TRACE )} /*  */ , DO_CL( TRACE ) /*    */ ,
   /*      */ DO_O( UPLUS, cli.trace.sampupd ) /*                                                            */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-scandir" ), /*  */ DO_A_O, /* */ DO_V( SCAN_DE_DIR_TRACE )}, DO_CL( TRACE ) /*         */ ,
   /*      */ DO_O( UPLUS, cli.trace.scan_de_dir ) /*                                                        */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-scan" ), /*     */ DO_A_O, /* */ DO_V( SCAN_TRACE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.scan ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-scanreg" ), /*  */ DO_A_O, /* */ DO_V( SCAN_DE_REG_TRACE )}, DO_CL( TRACE ) /*         */ ,
   /*      */ DO_O( UPLUS, cli.trace.scan_de_reg ) /*                                                        */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-sd5" ), /*      */ DO_A_O, /* */ DO_V( SD5_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.sd5 ) /*                                                                */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-select" ), /*   */ DO_A_O, /* */ DO_V( SELECT_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.select ) /*                                                             */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-seq" ), /*      */ DO_A_O, /* */ DO_V( SEQ_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.seq ) /*                                                                */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-sql" ), /*      */ DO_A_O, /* */ DO_V( SQL_TRACE )} /*      */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.sql ) /*                                                                */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-temp" ), /*     */ DO_A_O, /* */ DO_V( TEMP_TRACE )} /*     */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.temp ) /*                                                               */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-update" ), /*   */ DO_A_O, /* */ DO_V( UPDATE_TRACE )} /*   */ , DO_CL( TRACE ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.trace.update ) /*                                                             */ , DO_H( trace .... ) /*         */ },

  {.o = {DO_Q( "trace-stderr" ), /*   */ DO_A_N, /* */ DO_V( TRACE_STDERR )} /*   */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },
  {.o = {DO_Q( "trace-stdout" ), /*   */ DO_A_N, /* */ DO_V( TRACE_STDOUT )} /*   */ , DO_CL( TRACE ) /*     */ , DO_H( trace .... ) /*         */ },

  {.o = {DO_Q( "tree2db" ), /*        */ DO_A_N, /* */ DO_V( TREE_TO_DB )} /*     */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "tree" ), /*           */ DO_A_N, /* */ DO_VF( TREE )} /*          */ , DO_CL( PRINT ) /*     */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, tree ) /*                                                 */ , DO_H( tree ) /*               */ },
  {.o = {DO_Q( "tree-to-db" ), /*     */ DO_A_N, /* */ DO_V( TREE_TO_DB )} /*     */ , DO_CL( NODESC ) /*    */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "uni-scan" ), /*       */ DO_A_N, /* */ DO_VF( UNI_SCAN )} /*      */ , DO_CL( OBSOLETE ) /*  */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, uni_scan ) /*                                             */ , DO_H(  .... ) /*              */ },
  {.o = {DO_Q( "vacuum" ), /*         */ DO_A_N, /* */ DO_VF( VACUUM )} /*        */ , DO_CL( SYSTEM ) /*    */ ,
   /*      */ DO_O( FLAG, cli.act.v ), DO_F1( act, vacuum ) /*                                               */ , DO_H( vacuum db ) /*          */ },
  {.o = {DO_Q( "verbose" ), /*        */ DO_A_O, /* */ DO_V( VERBOSE )} /*        */ , DO_CL( DEBUG ) /*     */ ,
   /*      */ DO_O( UPLUS, cli.dbg.verbose ) /*                                                              */ , DO_H( verbose ) /*            */ },
  {.o =
   {DO_Q( "version" ), /*        */ DO_A_N, /* */ DO_V( VERSION )} /*        */ , DO_CL( REFERENCE ) /* */ , DO_H( version info ) /*            */ },
  {.o = {DO_Q( "zero-db" ), /*        */ DO_A_N, /* */ DO_VF( ZERO_DB )} /*       */ , DO_CL( SYSTEM ) /*    */ , DO_H( zero db ) /*            */ },
  /*{.o = {DO_N(),.has_arg = 0,DO_V(LIMIT)},.help = "....",.oclass = ...}, */
/*{.o = {DO_N(add-to-group), 		 DO_A_N,		       .name = "",.has_arg = 0,DO_V(ADD_TO_GROUP)},.help = "...",.oclass = ...}, */
/*{.o = {DO_N(group), 		 DO_A_R, 		 DO_V(GROUP)},.help = "help on all",DO_CL(HELP)}, */
/*{.o = {DO_N(limit), 		 DO_A_R, 		 DO_V(LIMIT)},.help = "trace ....",DO_CL(TRACE)}, */
  /* .name = "print-dirs",                  .has_arg = no_argument,         */
  /* .name = "print-duplicates",                    .has_arg = no_argument, */
  /* .name = "print-files",                 .has_arg = no_argument,         */
  /* .name = "print-paths",                 .has_arg = no_argument,         */
  /* .name = "remove-from-group",                   .has_arg = no_argument, */
  /* .name = "same-exif",                           .has_arg = no_argument, */
  /* .name = "same-files",                  .has_arg = no_argument,         */
  /* .name = "same-md5",                            .has_arg = no_argument, */
  /* .name = "zero-duplicates",                     .has_arg = no_argument, */
  /* .name = "update-exif",                 .has_arg = no_argument,         */
  /* .name = "update-mdpath",                       .has_arg = no_argument, */
  /* .name = "update-mdpath-selective",             .has_arg = no_argument, */
  /* .name = "update-path",                 .has_arg = no_argument,         */
  {0}
};

const duf_longval_extended_t *lo_extended = _lo_extended;
unsigned lo_extended_count = sizeof( _lo_extended ) / sizeof( _lo_extended[0] );

/* static const duf_option_t duf_longopts_table[] = {                                                                                             */
/*   {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ADD_PATH},                                                                 */
/* (*{.name = "add-to-group",                      .has_arg = no_argument,                         .val = DUF_OPTION_ADD_TO_GROUP}, *)            */
/*   {.name = "collect",.has_arg = no_argument,.val = DUF_OPTION_FLAG_COLLECT},                                                                   */
/*   {.name = "crc32",.has_arg = no_argument,.val = DUF_OPTION_FLAG_CRC32},                                                                       */
/*   {.name = "crc32id",.has_arg = required_argument,.val = DUF_OPTION_CRC32ID},                                                                  */
/*   {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_FLAG_CREATE_TABLES},                                                       */
/*   {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},                                                        */
/*   {.name = "db-name-adm",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME_ADM},                                                          */
/*   {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME_MAIN},                                                             */
/*   {.name = "debug",.has_arg = optional_argument,.val = DUF_OPTION_DEBUG},                                                                      */
/*   {.name = "directories",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRS},                                                                  */
/*   {.name = "dirent",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRENT},                                                                     */
/*   {.name = "dirs",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRS},                                                                         */
/*   {.name = "disable-calculate",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_CALCULATE},                                               */
/*   {.name = "disable-insert",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_INSERT},                                                     */
/*   {.name = "disable-update",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_UPDATE},                                                     */
/*   {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DROP_TABLES},                                                           */
/*   {.name = "dry-run",.has_arg = no_argument,.val = DUF_OPTION_DRY_RUN_TRACE},                                                                  */
/*   {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},                                                       */
/*   {.name = "exif",.has_arg = no_argument,.val = DUF_OPTION_FLAG_EXIF},                                                                         */
/*   {.name = "exifid",.has_arg = required_argument,.val = DUF_OPTION_EXIFID},                                                                    */
/*   {.name = "explain",.has_arg = optional_argument,.val = DUF_OPTION_EXPLAIN_TRACE},                                                            */
/*   {.name = "filedata",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILEDATA},                                                                 */
/*   {.name = "filenames",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILENAMES},                                                               */
/*   {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILES},                                                                       */
/*   {.name = "format",.has_arg = required_argument,.val = DUF_OPTION_FORMAT},                                                                    */
/* (*{.name = "group",                             .has_arg = required_argument,                           .val = DUF_OPTION_GROUP}, *)           */
/*   {.name = "help-all",.has_arg = no_argument,.val = DUF_OPTION_HELP_ALL},                                                                      */
/*   {.name = "help-collect",.has_arg = no_argument,.val = DUF_OPTION_HELP_COLLECT},                                                              */
/*   {.name = "help-control",.has_arg = no_argument,.val = DUF_OPTION_HELP_CONTROL},                                                              */
/*   {.name = "help-debug",.has_arg = no_argument,.val = DUF_OPTION_HELP_DEBUG},                                                                  */
/*   {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_EXAMPLES},                                                                 */
/*   {.name = "help",.has_arg = optional_argument,.val = DUF_OPTION_SMART_HELP},                                                                  */
/*   {.name = "help-help",.has_arg = no_argument,.val = DUF_OPTION_HELP_HELP},                                                                    */
/*   {.name = "help-nodesc",.has_arg = no_argument,.val = DUF_OPTION_HELP_NODESC},                                                                */
/*   {.name = "help-print",.has_arg = no_argument,.val = DUF_OPTION_HELP_PRINT},                                                                  */
/*   {.name = "help-reference",.has_arg = no_argument,.val = DUF_OPTION_HELP_REFERENCE},                                                          */
/*   {.name = "help-request",.has_arg = no_argument,.val = DUF_OPTION_HELP_REQUEST},                                                              */
/*   {.name = "help-scan",.has_arg = no_argument,.val = DUF_OPTION_HELP_SCAN},                                                                    */
/*   {.name = "help-short",.has_arg = no_argument,.val = DUF_OPTION_HELP},                                                                        */
/*   {.name = "help-system",.has_arg = no_argument,.val = DUF_OPTION_HELP_SYSTEM},                                                                */
/*   {.name = "help-trace",.has_arg = no_argument,.val = DUF_OPTION_HELP_TRACE},                                                                  */
/*   {.name = "help-update",.has_arg = no_argument,.val = DUF_OPTION_HELP_UPDATE},                                                                */
/*   {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},                                                       */
/*   {.name = "info",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INFO},                                                                         */
/*   {.name = "information",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INFO},                                                                  */
/*   {.name = "integrity",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INTEGRITY},                                                               */
/* (*{.name = "limit",                             .has_arg = required_argument,                           .val = DUF_OPTION_LIMIT}, *)           */
/*   {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},                                                        */
/*   {.name = "max-depth",.has_arg = required_argument,.val = DUF_OPTION_MAXDEPTH},                                                               */
/*   {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},                                                         */
/*   {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},                                                           */
/*   {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},                                                         */
/*   {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},                                                               */
/*   {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},                                                                 */
/*   {.name = "max-seq",.has_arg = required_argument,.val = DUF_OPTION_MAXSEQ},                                                                   */
/*   {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},                                                                 */
/*   {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MD5},                                                                           */
/*   {.name = "md5id",.has_arg = required_argument,.val = DUF_OPTION_MD5ID},                                                                      */
/*   {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MDPATH},                                                                     */
/*   {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},                                                                      */
/*   {.name = "mime",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MIME},                                                                         */
/*   {.name = "mimeid",.has_arg = required_argument,.val = DUF_OPTION_MIMEID},                                                                    */
/*   {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},                                                        */
/*   {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},                                                         */
/*   {.name = "min-same",.has_arg = required_argument,.val = DUF_OPTION_MINSAME},                                                                 */
/*   {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},                                                                 */
/*   {.name = "output-file",.has_arg = required_argument,.val = DUF_OPTION_OUTPUT_FILE},                                                          */
/*   {.name = "output-level",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT_LEVEL},                                                        */
/* (*{.name = "print-dirs",                        .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DIRS},   *)            */
/* (*{.name = "print-duplicates",                  .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DUPLICATES}, *)        */
/* (*{.name = "print-files",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_FILES}, *)             */
/*   {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_FLAG_PRINT},                                                                       */
/* (*{.name = "print-paths",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_PATHS}, *)             */
/*   {.name = "progress",.has_arg = no_argument,.val = DUF_OPTION_FLAG_PROGRESS},                                                                 */
/*   {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_FLAG_RECURSIVE},                                                               */
/*   {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_FLAG_REMOVE_DATABASE},                                                   */
/* (*{.name = "remove-from-group",                 .has_arg = no_argument,                         .val = DUF_OPTION_REMOVE_FROM_GROUP}, *)       */
/* (*{.name = "same-exif",                         .has_arg = no_argument,                         .val = DUF_OPTION_SAME_EXIF},   *)             */
/* (*{.name = "same-files",                        .has_arg = no_argument,                         .val = DUF_OPTION_SAME_FILES}, *)              */
/*   {.name = "same",.has_arg = required_argument,.val = DUF_OPTION_SAME},                                                                        */
/* (*{.name = "same-md5",                          .has_arg = no_argument,                         .val = DUF_OPTION_SAME_MD5},     *)            */
/*   {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},                                                                    */
/*   {.name = "sampupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD},                                                                  */
/*   {.name = "sd5",.has_arg = no_argument,.val = DUF_OPTION_FLAG_SD5},                                                                           */
/*   {.name = "sd5id",.has_arg = required_argument,.val = DUF_OPTION_SD5ID},                                                                      */
/*   {.name = "size",.has_arg = required_argument,.val = DUF_OPTION_SIZE},                                                                        */
/*   {.name = "summary",.has_arg = no_argument,.val = DUF_OPTION_FLAG_SUMMARY},                                                                   */
/*   {.name = "test-option",.has_arg = required_argument,.val = DUF_OPTION_TEST},                                                                 */
/*   {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},                                                        */
/*   {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_ALL_TRACE},                                                              */
/*   {.name = "trace-any",.has_arg = optional_argument,.val = DUF_OPTION_ANY_TRACE},                                                              */
/*   {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},                                                          */
/*   {.name = "trace-collect",.has_arg = optional_argument,.val = DUF_OPTION_COLLECT_TRACE},                                                      */
/*   {.name = "trace-crc32",.has_arg = optional_argument,.val = DUF_OPTION_CRC32_TRACE},                                                          */
/*   {.name = "trace-current",.has_arg = optional_argument,.val = DUF_OPTION_CURRENT_TRACE},                                                      */
/*   {.name = "trace-deleted",.has_arg = optional_argument,.val = DUF_OPTION_DELETED_TRACE},                                                      */
/*   {.name = "trace-dirent",.has_arg = optional_argument,.val = DUF_OPTION_DIRENT_TRACE},                                                        */
/*   {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},                                                          */
/*   {.name = "trace-exif",.has_arg = optional_argument,.val = DUF_OPTION_EXIF_TRACE},                                                            */
/*   {.name = "trace-explain",.has_arg = optional_argument,.val = DUF_OPTION_EXPLAIN_TRACE},                                                      */
/*   {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},                                                            */
/*   {.name = "trace-fs",.has_arg = optional_argument,.val = DUF_OPTION_FS_TRACE},                                                                */
/*   {.name = "trace-insert",.has_arg = optional_argument,.val = DUF_OPTION_INSERT_TRACE},                                                        */
/*   {.name = "trace-integrity",.has_arg = optional_argument,.val = DUF_OPTION_INTEGRITY_TRACE},                                                  */
/*   {.name = "trace-match",.has_arg = optional_argument,.val = DUF_OPTION_MATCH_TRACE},                                                          */
/*   {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},                                                              */
/*   {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},                                                        */
/*   {.name = "trace-mime",.has_arg = optional_argument,.val = DUF_OPTION_MIME_TRACE},                                                            */
/*   {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},                                                          */
/*   {.name = "trace-options",.has_arg = optional_argument,.val = DUF_OPTION_OPTIONS_TRACE},                                                      */
/*   {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},                                                            */
/*   {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},                                                        */
/*   {.name = "trace-samupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD_TRACE},                                                       */
/*   {.name = "trace-scandir",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_DE_DIR_TRACE},                                                  */
/*   {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},                                                            */
/*   {.name = "trace-scanreg",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_DE_REG_TRACE},                                                  */
/*   {.name = "trace-sd5",.has_arg = optional_argument,.val = DUF_OPTION_SD5_TRACE},                                                              */
/*   {.name = "trace-select",.has_arg = optional_argument,.val = DUF_OPTION_SELECT_TRACE},                                                        */
/*   {.name = "trace-seq",.has_arg = optional_argument,.val = DUF_OPTION_SEQ_TRACE},                                                              */
/*   {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},                                                              */
/*   {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},                                                              */
/*   {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},                                                              */
/*   {.name = "trace-temp",.has_arg = optional_argument,.val = DUF_OPTION_TEMP_TRACE},                                                            */
/*   {.name = "trace-update",.has_arg = optional_argument,.val = DUF_OPTION_UPDATE_TRACE},                                                        */
/*   {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                     */
/*   {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_FLAG_TREE},                                                                         */
/*   {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                  */
/*   {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_FLAG_UNI_SCAN},                                                                 */
/* (*{.name = "update-exif",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_EXIF}, *)             */
/* (*{.name = "update-mdpath",                     .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH}, *)           */
/* (*{.name = "update-mdpath-selective",           .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, *) */
/* (*{.name = "update-path",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_PATH}, *)             */
/*   {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_FLAG_VACUUM},                                                                     */
/*   {.name = "verbose",.has_arg = optional_argument,.val = DUF_OPTION_VERBOSE},                                                                  */
/*   {.name = "version",.has_arg = no_argument,.val = DUF_OPTION_VERSION},                                                                        */
/*   {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ZERO_DB},                                                                   */
/* (*{.name = "zero-duplicates",                   .has_arg = no_argument,                         .val = DUF_OPTION_ZERO_DUPLICATES}, *)         */
/*   {0},                                                                                                                                         */
/* };                                                                                                                                             */
/*                                                                                                                                                */
/* const duf_option_t *duf_longopts = &duf_longopts_table[0];                                                                                     */
/* unsigned duf_longopts_count = sizeof( duf_longopts_table ) / sizeof( duf_longopts_table[0] );                                                  */
