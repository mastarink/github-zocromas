#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_common.h"
/* ###################################################################### */


duf_beginning_t sql_beginning_common = {.done = 0,
  .sql = {
            "PRAGMA synchronous = OFF" /* */ ,
            "PRAGMA encoding = 'UTF-8'" /* */ ,


            NULL}
};
