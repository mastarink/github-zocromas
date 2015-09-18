#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_common.h"
/* ###################################################################### */


duf_sql_sequence_t sql_beginning_common = {.done = 0,
  .name = "common",
  .sql = {
          "PRAGMA synchronous = OFF" /* */ ,
          "PRAGMA encoding = 'UTF-8'" /* */ ,
          "PRAGMA temp_store = 2" /* */ ,
          "PRAGMA locking_mode = EXCLUSIVE" /**/,
          "PRAGMA automatic_index = 1" /* */ ,
          /* "PRAGMA foreign_keys = 1" (* *) , */
          NULL}
};
