#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
const duf_longval_extended_t optable_redo_and_obsolete[] = {
  {.o = {DO_Q( "min-dirfiles" ) /*       */, DO_A_R /* */, DO_V( MINDIRFILES )} /*    */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "max-dirfiles" ) /*       */, DO_A_R /* */, DO_V( MAXDIRFILES )} /*    */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "max-dirs" ) /*           */, DO_A_R /* */, DO_V( MAXITEMS_DIRS )} /*  */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "max-files" ) /*          */, DO_A_R /* */, DO_V( MAXITEMS_FILES )} /* */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "max-items" ) /*          */, DO_A_R /* */, DO_V( MAXITEMS )} /*       */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },




  /* ????????? */





  {.o = {DO_Q( "tree2db" ) /*            */, DO_A_N /* */, DO_V( TREE_TO_DB )} /*     */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "tree-to-db" ) /*         */, DO_A_N /* */, DO_V( TREE_TO_DB )} /*     */ , DO_CL( NODESC ) /*  */ , DO_H(  .... ) /*         */ },
  {.o = {DO_Q( "zero-db" ) /*            */, DO_A_N /* */, DO_VF( ZERO_DB )} /*       */ , DO_CL( SYSTEM ) /*  */ , DO_H( zero db ) /*       */ },


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

  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
