#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_optable_def.h"
#include "duf_options_enum.h" 

#include "duf_optimpl_sccb.h"    /* TODO : temp for duf_option_O_db_open               */

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

   {.o = {DO_Q( "tag-file" ) /*         */ , DO_A_R /* */ , DO_V( TAG_FILE )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, tag.file ) /*                                                    */ , DO_H( tag file ) /*                         */ },
   {.o = {DO_Q( "tag-dir" ) /*          */ , DO_A_R /* */ , DO_V( TAG_DIR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, tag.dir ) /*                                                     */ , DO_H( tag dir ) /*                          */ },

   {.o = {DO_Q( "save-directory" ) /*   */ , DO_A_R /* */ , DO_V( SAVE_DIRECTORY )} /*     */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( STR, save.path ) /*                                                   */ , DO_H( save path ) /*                        */ },

   {.o = {DO_Q( "history-file" ) /*      */ , DO_A_R /* */ , DO_V( HISTORY_FILE )} /*      */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, cli.history_filename ) /*                                        */ , DO_H( history filename ) /*                 */ },

   {.o = {DO_Q( "config-dir" ) /*      */ , DO_A_R /* */ , DO_V( CONFIG_DIR )} /*          */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, config_dir ), DO_AT_STAGE( PRESETUP ) /*                         */ , DO_H( config directory ) /*                 */ },
   {.o = {DO_Q( "cmds-dir" ) /*      */ , DO_A_R /* */ , DO_V( CMDS_DIR )} /*              */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( STR, cmds_dir ), DO_AT_STAGE( SETUP ) /*                            */ , DO_H( config directory ) /*                 */ },


   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
