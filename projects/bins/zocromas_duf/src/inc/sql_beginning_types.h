#ifndef SQL_BEGINNING_TYPES_H
#  define SQL_BEGINNING_TYPES_H


typedef struct
{
  const char *name;
  unsigned done;
  unsigned beginend;
  unsigned set_selected_db;
  const char *sql[];
} duf_sql_sequence_t;

#include <mastar/tools/mas_argvc_types.h>

#  include "duf_base_types.h"
/* #  include "duf_sql_types.h" */
#  include "duf_sql_types.h"
#  include "duf_ufilter_types.h"
typedef int ( *duf_bind_cb_t ) ( duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * arg );

#endif
