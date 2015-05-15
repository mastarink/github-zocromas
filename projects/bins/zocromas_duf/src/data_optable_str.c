#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"
const duf_longval_extended_t optable_str[] = {
  {.o = {DO_Q( "db-directory" ) /*      */ , DO_A_R /* */ , DO_V( DB_DIRECTORY )} /* */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_OC( STR, db.dir ) /*                                                                     */ , DO_H( db directory ) /*        */ },
  {.o = {DO_Q( "db-name-adm" ) /*       */ , DO_A_R /* */ , DO_V( DB_NAME_ADM )} /*  */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_OC( STR, db.adm.name ) /*                                                                */ , DO_H( db name adm ) /*         */ },
  {.o = {DO_Q( "db-name" ) /*           */ , DO_A_R /* */ , DO_V( DB_NAME_MAIN )} /* */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_OC( STR, db.main.name ) /*                                                               */ , DO_H( db name main ) /*        */ },

  {.o = {.name = NULL}}
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
