#include <stddef.h>

#include "duf_optable_def.h"

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */
#include "duf_optimpl_sccb.h"                                        /* TODO : temp for duf_option_O_db_open               */

/*
At duf_options_table.c:
  1. extern const muc_longval_extended_table_t optable_db[];
  2.  static const muc_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_db,
	 ...

	NULL
      };
*/

const muc_longval_extended_table_t optable_db = {
  .name = "db",
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "db-directory" ) /*     */ , DO_A_R /* */ , DO_V( DB_DIRECTORY )} /*       */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( PATH, db.dir_x ) /*                                                   */ , DO_H( db directory ) /*                     */ },
   {.o = {DO_Q( "db-subdirectory" ) /*  */ , DO_A_R /* */ , DO_V( DB_SUBDIRECTORY )} /*    */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( CSTR, db.subdir_x ) /*                                                */ , DO_H( db directory ) /*                     */ },
   {.o = {DO_Q( "db-adm-name" ) /*      */ , DO_A_R /* */ , DO_V( DB_NAME_ADM )} /*        */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( CSTR, db.adm.name_x ) /*                                              */ , DO_H( db name adm ) /*                      */ },
   {.o = {DO_Q( "db-temp-name" ) /*     */ , DO_A_R /* */ , DO_V( DB_NAME_TEMP )} /*       */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( CSTR, db.tempo.name_x ) /*                                             */ , DO_H( db name tempo ) /*                    */ },
   {.o = {DO_Q( "db-selected-name" ) /* */ , DO_A_R /* */ , DO_V( DB_NAME_SELECTED )} /*   */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( CSTR, db.selected.name_x ) /*                                         */ , DO_H( db name selected ) /*                 */ },
   {.o = {DO_Q( "db-name" ) /*          */ , DO_A_R /* */ , DO_V( DB_NAME_MAIN )} /*       */ , DO_CL( DB ) /* */ ,
    /*      */ DO_OC( CSTR, db.main.name_x ) /*                                             */ , DO_H( db name main ) /*                     */ },
   {.o = {DO_Q( "open-db" ) /*          */ , DO_A_N /*  */ , DO_V( DB_OPEN )} /*           */ , DO_CL( NODESC ) /*  */
    , DO_EV_CALL( db_open ), DO_STAGE_IPLS /*                                                */ , DO_H(  ... ) /*                             */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
