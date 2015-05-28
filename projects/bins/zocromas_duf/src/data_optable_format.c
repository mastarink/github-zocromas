#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_optable_def.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_main[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_main,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_format = {
  .table =                      /* */
  {
   {.o = {DO_Q( "bformat" ) /*          */ , DO_A_R /* */ , DO_V( BFORMAT )} /*            */ , DO_CL( PRINT ) /*   */
    /*                                                                                      */ , DO_H( print w / bformat ) /*                */ },

   {.o = {DO_Q( "format-dirs-tree" ) /*  */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_TREE )} /*  */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_dirs_tree ) /*                                */ , DO_H( format ) /*                           */ },
   {.o = {DO_Q( "format-files-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_TREE )} /* */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_files_tree ) /*                               */ , DO_H( format ) /*                           */ },

   {.o = {DO_Q( "format-prefix-gen-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_GEN_TREE )} /* */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_prefix_gen_tree ) /*                          */ , DO_H( format ) /*                           */ },
   {.o = {DO_Q( "format-prefix-files-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_FILES_TREE )} /* */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_prefix_files_tree ) /*                        */ , DO_H( format ) /*                           */ },
   {.o = {DO_Q( "format-prefix-dirs-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_DIRS_TREE )} /* */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_prefix_dirs_tree ) /*                         */ , DO_H( format ) /*                           */ },


   {.o = {DO_Q( "format-dirs-list" ) /*  */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_LIST )} /*  */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_dirs_list ) /*                                */ , DO_H( format ) /*                           */ },
   {.o = {DO_Q( "format-files-list" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_LIST )} /* */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_files_list ) /*                               */ , DO_H( format ) /*                           */ },


   {.o = {DO_Q( "format-dirs" ) /*       */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_GEN )} /*   */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_dirs_gen ) /*                                 */ , DO_H( format ) /*                           */ },
   {.o = {DO_Q( "format-files" ) /*      */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_GEN )} /*  */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat_files_gen ) /*                                */ , DO_H( format ) /*                           */ },

   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
