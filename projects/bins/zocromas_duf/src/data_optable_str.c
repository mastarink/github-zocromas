#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"

#include "duf_option_sccb.h"    /* TODO : temp for duf_option_$_db_open               */

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_str[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_str,
	 ...

	NULL
      };
*/


const duf_longval_extended_table_t optable_str = {
  .name = "str",
  .table =                      /* */
  {
   {.o = {DO_Q( "db-directory" ) /*     */ , DO_A_R /* */ , DO_V( DB_DIRECTORY )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.dir ) /*                                                      */ , DO_H( db directory ) /*                     */ },
   {.o = {DO_Q( "db-subdirectory" ) /*  */ , DO_A_R /* */ , DO_V( DB_SUBDIRECTORY )} /*    */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.subdir ) /*                                                   */ , DO_H( db directory ) /*                     */ },
   {.o = {DO_Q( "db-adm-name" ) /*      */ , DO_A_R /* */ , DO_V( DB_NAME_ADM )} /*        */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.adm.name ) /*                                                 */ , DO_H( db name adm ) /*                      */ },
   {.o = {DO_Q( "db-temp-name" ) /*     */ , DO_A_R /* */ , DO_V( DB_NAME_TEMP )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.tempo.name ) /*                                               */ , DO_H( db name tempo ) /*                    */ },
   {.o = {DO_Q( "db-selected-name" ) /* */ , DO_A_R /* */ , DO_V( DB_NAME_SELECTED )} /*   */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.selected.name ) /*                                           */ , DO_H( db name selected ) /*                 */ },
   {.o = {DO_Q( "db-name" ) /*          */ , DO_A_R /* */ , DO_V( DB_NAME_MAIN )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( CSTR, db.main.name ) /*                                               */ , DO_H( db name main ) /*                     */ },
   {.o = {DO_Q( "open-db" ) /*          */ , DO_A_N /*  */ , DO_V( DB_OPEN )} /*           */ , DO_CL( NODESC ) /*  */
    , DO_EV_CALL( db_open ), DO_STAGE_ANY /*                                                */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "tag-file" ) /*         */ , DO_A_R /* */ , DO_V( TAG_FILE )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, tag.file ) /*                                                    */ , DO_H( tag file ) /*                         */ },
   {.o = {DO_Q( "tag-dir" ) /*          */ , DO_A_R /* */ , DO_V( TAG_DIR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, tag.dir ) /*                                                     */ , DO_H( tag dir ) /*                          */ },

   {.o = {DO_Q( "save-directory" ) /*   */ , DO_A_R /* */ , DO_V( SAVE_DIRECTORY )} /*     */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, save.path ) /*                                                   */ , DO_H( save path ) /*                        */ },

   {.o = {DO_Q( "history-file" ) /*      */ , DO_A_R /* */ , DO_V( HISTORY_FILE )} /*      */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, cli.output.history_filename ) /*                                 */ , DO_H( history filename ) /*                         */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
