#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_vacuum.h"
/* ###################################################################### */


duf_beginning_t sql_beginning_vacuum = {.done = 0,
  .sql = {
            "VACUUM",

            NULL}
};
