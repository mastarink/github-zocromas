#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_common.h"
/* ###################################################################### */


const char *sql_beginning_common[] = {
  "PRAGMA synchronous = OFF" /* */ ,
  "PRAGMA encoding = 'UTF-8'" /* */ ,


  NULL
};
