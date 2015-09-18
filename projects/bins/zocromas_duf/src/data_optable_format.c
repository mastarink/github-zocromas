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
   {.o = {DO_Q( "bformat" ) /*                  */ , DO_A_R /* */ , DO_V( BFORMAT )} /*                     */ , DO_CL( PRINT ) /*   */
    /*                                                                                                       */ , DO_H( print w / bformat ) /* */ },

   {.o = {DO_Q( "format-dirs-tree" ) /*         */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_TREE )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.dirs_tree ) /*                                                 */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-tree" ) /*        */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_TREE )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.files_tree ) /*                                                */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-prefix-gen-tree" ) /*   */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_GEN_TREE )} /*     */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.prefix_gen_tree ) /*                                           */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-prefix-files-tree" ) /* */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_FILES_TREE )} /*   */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.prefix_files_tree ) /*                                         */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-prefix-dirs-tree" ) /*  */ , DO_A_R /* */ , DO_V( SFORMAT_PREFIX_DIRS_TREE )} /*    */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.prefix_dirs_tree ) /*                                          */ , DO_H( format ) /*          */ },


   {.o = {DO_Q( "format-dirs-list" ) /*         */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_LIST )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.dirs_list ) /*                                                 */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-list" ) /*        */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_LIST )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.files_list ) /*                                                */ , DO_H( format ) /*          */ },


   {.o = {DO_Q( "format-dirs-gen" ) /*              */ , DO_A_R /* */ , DO_V( SFORMAT_DIRS_GEN )} /*            */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.dirs_gen ) /*                                                  */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-files-gen" ) /*             */ , DO_A_R /* */ , DO_V( SFORMAT_FILES_GEN )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( STR, cli.output.sformat.files_gen ) /*                                                 */ , DO_H( format ) /*          */ },

   {.o = {DO_Q( "format-tree-files" ) /*        */ , DO_A_R /* */ , DO_V( ASFORMAT_TREE_FILES )} /*         */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.tree.files ) /*                                            */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-tree-dirs" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_TREE_DIRS )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.tree.dirs ) /*                                             */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-list-files" ) /*        */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_FILES )} /*         */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.list.files ) /*                                            */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-list-dirs" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_LIST_DIRS )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.list.dirs ) /*                                             */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-gen-files" ) /*         */ , DO_A_R /* */ , DO_V( ASFORMAT_GEN_FILES )} /*          */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.gen.files ) /*                                             */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "format-gen-dirs" ) /*          */ , DO_A_R /* */ , DO_V( ASFORMAT_GEN_DIRS )} /*           */ , DO_CL( PRINT ) /* */ ,
    /*      */ DO_OC( ARGV, cli.output.as_formats.gen.dirs ) /*                                              */ , DO_H( format ) /*          */ },
   {.o = {DO_Q( "use-format" ) /*               */ , DO_A_R /* */ , DO_V( ASFORMAT_USE )} /*                */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OC( NUM, cli.output.as_formats.use ) /*                                                    */ , DO_H(  .... ) /*           */ },
   
   {.o = {DO_Q( "max-width" ) /*        */ , DO_A_R /* */ , DO_V( MAXWIDTH )} /*                            */ , DO_CL( NODESC ) /*   */ ,
    /*      */ DO_OC( NUM, cli.output.max_width ) /* */ , DO_AT_STAGE( SETUP ) /*                            */ , DO_H(  .... ) /*           */ },


   {.o = {.name = NULL}}
   }
};



/* vi: et ft=c colorcolumn=3,52,67,68,112,113,131,145,146
*/
