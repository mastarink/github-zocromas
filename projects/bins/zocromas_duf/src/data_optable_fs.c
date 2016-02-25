#include <stddef.h>

#include "duf_optable_def.h"
#include "duf_options_enum.h"                                        /* duf_option_code_t ♠ */

#include "duf_optimpl_fs_types.h"
#include "duf_optimpl_fs.h"                                          /* TODO : temp for duf_option_O_fs_...               */

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

const duf_longval_extended_table_t optable_fs = {
  .name = "fs",
/* DO_STG_NOT( LOOP ), */
  .xlist =                                                           /* */
  {
   {.o = {DO_Q( "fs-ls" ) /*            */ , DO_A_R /*  */ , DO_V( FS_LS )} /*             */ , DO_CL( FS ) /*    */ ,
    /*    */ DO_S_CALL( fs_ls ), DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*  */ , DO_H( fs ls ) /*                       */ },

   {.o = {DO_Q( "fs-rm-force" ) /*      */ , DO_A_R /*  */ , DO_V( FS_RM )} /*             */ , DO_CL( FS ) /*    */ ,
  /*    */ DO_SAN_CALL( fs_rm, DUF_OPTION_FS_MODE_FORCE ),
    DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs rm file ) /*                       */ },

   {.o = {DO_Q( "fs-rm-ask" ) /*        */ , DO_A_R /*  */ , DO_V( FS_RM )} /*         */ , DO_CL( FS ) /*    */ , DO_STG_NOT( LOOP ),
  /*    */ DO_SAN_CALL( fs_rm, DUF_OPTION_FS_MODE_ASK ),
    DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs rm file ) /*                       */ },

   {.o = {DO_Q( "fs-cp-ignore" ) /*     */ , DO_A_R /*  */ , DO_V( FS_CP )} /*             */ , DO_CL( FS ) /*    */ , DO_STG_NOT( LOOP ),
    DO_SET_STAGE( FIRST, INTERAC ), DO_SAN_CALL( fs_cp, DUF_OPTION_FS_MODE_IGNORE ) /*      */ , DO_H( fs cp file ) /*                       */ },

   {.o = {DO_Q( "fs-cp-overwrite" ) /*  */ , DO_A_R /*  */ , DO_V( FS_CP )} /*             */ , DO_CL( FS ) /*    */ , DO_STG_NOT( LOOP ),
    DO_SET_STAGE( FIRST, INTERAC ), DO_SAN_CALL( fs_cp, DUF_OPTION_FS_MODE_OVERWRITE ) /*   */ , DO_H( fs cp file ) /*                       */ },

   {.o = {DO_Q( "fs-cp-force" ) /*      */ , DO_A_R /*  */ , DO_V( FS_CP )} /*             */ , DO_CL( FS ) /*    */ , DO_STG_NOT( LOOP ),
    DO_SET_STAGE( FIRST, INTERACTIVE ), DO_SAN_CALL( fs_cp, DUF_OPTION_FS_MODE_FORCE ) /*   */ , DO_H( fs cp file ) /*                       */ },

   {.o = {DO_Q( "fs-cp-ask" ) /*        */ , DO_A_R /*  */ , DO_V( FS_CP )} /*         */ , DO_CL( FS ) /*    */ , DO_STG_NOT( LOOP ),
  /*    */ DO_SAN_CALL( fs_cp, DUF_OPTION_FS_MODE_ASK ),
    DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs cp file ) /*                       */ },

   {.o = {DO_Q( "fs-mv-ignore" ) /*     */ , DO_A_R /*  */ , DO_V( FS_MV )} /*       */ , DO_CL( FS ) /*    */ ,
  /*    */ DO_SAN_CALL( fs_mv, DUF_OPTION_FS_MODE_IGNORE ),
    DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs mv file ) /*                       */ },

   {.o = {DO_Q( "fs-mv-force" ) /*      */ , DO_A_R /*  */ , DO_V( FS_MV )} /*       */ , DO_CL( FS ) /*    */ ,
  /*    */ DO_SAN_CALL( fs_mv, DUF_OPTION_FS_MODE_FORCE ),
    DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs mv file ) /*                       */ },

   {.o = {DO_Q( "fs-mv-ask" ) /*        */ , DO_A_R /*  */ , DO_V( FS_MV )} /*         */ , DO_CL( FS ) /*    */ ,
  /*    */ DO_SAN_CALL( fs_mv, DUF_OPTION_FS_MODE_ASK ),
    DO_STG_NOT( LOOP ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                               */ , DO_H( fs mv file ) /*                       */ },

   {.o = {.name = NULL}}
   }
};

/* vi: et ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
