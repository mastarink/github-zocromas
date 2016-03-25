#include <stddef.h>

#include "duf_optimpl_class.h"

#include "duf_config_structs.h"
#include "duf_config_pointers.h"                                     /* duf_get_( .* )_pointer */

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

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

const muc_longval_extended_table_t optable_flag_allow = {
  .name = "flag_allow",
  .xlist =                                                           /* */
  {

   {.o = {DO_Q( "allow-remove-database" ), DO_A_N /* */ , DO_VF( ALLOW_REMOVE_DATABASE )}, DO_CL( SYSTEM ) /*      */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_remove_database ) /*             */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "allow-create-database" ), DO_A_N /* */ , DO_VF( ALLOW_CREATE_DB )} /*     */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_create_database ) /*             */ , DO_H( create db ) /*                        */ },
   {.o = {DO_Q( "allow-create-tables" ), DO_A_N /* */ , DO_VF( ALLOW_CREATE_TABLES )} /*   */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_create_tables ) /*               */ , DO_H( allow create tables ) /*              */ },
   {.o = {DO_Q( "allow-drop-tables" ), DO_A_N /* */ , DO_VF( ALLOW_DROP_TABLES )} /*       */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_drop_tables ) /*                 */ , DO_H( allow drop tables ) /*                */ },
   {.o = {DO_Q( "allow-clean-tables" ), DO_A_N /* */ , DO_VF( ALLOW_CLEAN_TABLES )} /*     */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_clean_tables ) /*                */ , DO_H( allow clean tables ) /*               */ },
   {.o = {DO_Q( "allow-vacuum" ) /*     */ , DO_A_N /* */ , DO_VF( ALLOW_VACUUM )} /*      */ , DO_CL( SYSTEM ) /*  */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_vacuum ) /*                      */ , DO_H( vacuum db ) /*                        */ },

   {.o = {DO_Q( "allow-directories" ), DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /*       */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_dirs ), DO_STAGE_SPLS /*         */ , DO_H( collect dir info ) /*                 */ },
   {.o = {DO_Q( "allow-dirs" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_dirs ) /*                        */ , DO_H( get dir info ) /*                     */ },
   {.o = {DO_Q( "allow-sub" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_SUB )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_sub ) /*                        */ , DO_H( get sub info ) /*                      */ },
 /* renames files --> allow-files */
   {.o = {DO_Q( "allow-files" ) /*            */ , DO_A_N /* */ , DO_VF( ALLOW_FILES )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_files ), DO_STAGE_SPLS /*        */ , DO_H( get file info ) /*                   */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
