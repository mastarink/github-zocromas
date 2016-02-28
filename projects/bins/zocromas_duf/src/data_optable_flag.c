#include <stddef.h>

#include "duf_optable_def.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_flag[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_flag,
	 ...

	NULL
      };
*/

const duf_longval_extended_table_t optable_flag = {
  .name = "flag",
  .xlist =                                                           /* */
  {
#if 0
 /* SEE --disable-memusage */
   {.o = {DO_Q( "memusage" ) /*          */ , DO_A_N /* */ , DO_V( MEMUSAGE )} /*   */ , DO_CL( DEBUG ) /*   */ , DO_H( debug ) /*        */ },
#endif
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

   {.o = {DO_Q( "quit" ) /*             */ , DO_A_N /* */ , DO_VF( QUIT )} /*              */ , DO_CL( CONTROL ) /*    */ ,
    /*      */ DO_OC( NOBFLAG, opt.act.v ), DO_BFL( act, interactive ) /*                    */ , DO_H( quit ) /*                              */ },
   {.o = {DO_Q( "add-path" ) /*         */ , DO_A_N /* */ , DO_VF( ADD_PATH )} /*          */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, add_path ) /*                          */ , DO_H( reg.init path ) /*                    */ },
   {.o = {DO_Q( "interactive" ) /*      */ , DO_A_N /* */ , DO_VF( INTERACTIVE )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, interactive ) /*                       */ , DO_H(  ... ) /*                             */ },

   {.o = {DO_Q( "force-color" ) /*      */ , DO_A_N /* */ , DO_VF( FORCE_COLOR )} /*       */ , DO_CL( CONTROL ) /* */ ,
    /* */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, force_color ), DO_STAGE_ALL /*        */ , DO_H(  ... ) /*                      */ },

/* DO_OC(NOBBFLAG, ...) + DO_BFL eq. to DO_OC(FLAG, ...) + DO_BFN */
   {.o = {DO_Q( "color" ) /*            */ , DO_A_N /* */ , DO_VF( NOCOLOR )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( NOBFLAG, opt.output.stream.v ), DO_BFL( output, nocolor ) /*                   */ , DO_H(  ... ) /*                      */ },
   {.o = {DO_Q( "escapecoloring" ) /*   */ , DO_A_N /* */ , DO_VF( NOCOLOR )} /*           */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFN( output, nocolor ) /*                     */ , DO_H(  ... ) /*                      */ },

 /* {.o = {DO_Q( "use-binformat" ) (*        *) , DO_A_N (* *) , DO_VF( USE_BINFORMAT )} (* *) , DO_CL( PRINT ) (*   *) ,                    */
 /*  (*      *) DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, use_binformat ) (*               *) , DO_H( use bin format ) (*     *) }, */
   {.o = {DO_Q( "progress" ) /*         */ , DO_A_N /* */ , DO_VF( PROGRESS )} /*          */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.output.stream.v ), DO_BFL( output, progress ) /*             */ , DO_H( progress ) /*                         */ },

   {.o = {DO_Q( "fast" ) /*             */ , DO_A_N /* */ , DO_VF( FAST )} /*              */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, fast ) /*                              */ , DO_H(  ... ) /*                             */ },
   {.o = {DO_Q( "fresh" ) /*            */ , DO_A_N /* */ , DO_VF( FRESH )} /*             */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, fresh ) /*                             */ , DO_HQ( "only process for unset data" ) /*   */ },
   {.o = {DO_Q( "recursive" ) /*        */ , DO_A_N /* */ , DO_VF( RECURSIVE )} /*         */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( rec_etc, recursive ), DO_STAGE_SPLS /*             */ , DO_H( recursive ) /*                        */ },

#if 0
   {.o = {DO_Q( "linear" ) /*           */ , DO_A_N /* */ , DO_VF( LINEAR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( rec_etc, linear ), DO_STAGE_SPLS /*                */ , DO_H( linear ) /*                           */ },
#else
   {.o = {DO_Q( "linear" ) /*           */ , DO_A_N /* */ , DO_VF( LINEAR )} /*            */ , DO_CL( CONTROL ) /* */ ,
    /*      */ DO_OU( BSFLAG, v ), DO_BFL( rec_etc, linear ), DO_STAGE_SPLS /*              */ , DO_H( linear ) /*                           */ },
#endif

   {.o = {DO_Q( "dirent" ) /*           */ , DO_A_N /* */ , DO_VF( DIRENT )} /*            */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, dirent ) /*                            */ , DO_H( dirent ) /*                           */ },

   {.o = {DO_Q( "allow-directories" ), DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /*       */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_dirs ), DO_STAGE_SPLS /*         */ , DO_H( collect dir info ) /*                 */ },
   {.o = {DO_Q( "allow-dirs" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_DIRECTORIES )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_dirs ) /*                        */ , DO_H( get dir info ) /*                     */ },
   {.o = {DO_Q( "allow-sub" ) /*       */ , DO_A_N /* */ , DO_VF( ALLOW_SUB )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_sub ) /*                        */ , DO_H( get sub info ) /*                      */ },
 /* renames files --> allow-files */
   {.o = {DO_Q( "allow-files" ) /*            */ , DO_A_N /* */ , DO_VF( ALLOW_FILES )} /* */ , DO_CL( COLLECT ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, allow_files ), DO_STAGE_SPLS /*        */ , DO_H( get file info ) /*                   */ },

   {.o = {DO_Q( "info" ) /*             */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "information" ) /*      */ , DO_A_N /* */ , DO_VF( INFO )} /*              */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, info ) /*                              */ , DO_H( db info ) /*                          */ },
   {.o = {DO_Q( "dry-run" ) /*          */ , DO_A_N /* */ , DO_VF( DRY_RUN )} /*           */ , DO_CL( DEBUG ) /*   */ ,
    /*      */ DO_OC( BFLAG, opt.flow ), DO_BFL( flow, dry_run ) /*                               */ , DO_H( dry ) /*                              */ },

   {.o = {DO_Q( "summary" ) /*          */ , DO_A_N /* */ , DO_VF( SUMMARY )} /*           */ , DO_CL( REFERENCE ) /* */ ,
    /*      */ DO_OC( BFLAG, opt.act.v ), DO_BFL( act, summary ) /*                           */ , DO_H( summary ) /*                          */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
