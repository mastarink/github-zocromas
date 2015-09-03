#ifndef SQL_BEGINNING_TYPES_H
#  define SQL_BEGINNING_TYPES_H

typedef struct
{
  unsigned done;
  unsigned beginend;
  unsigned set_selected_db;
  const char *sql[];
} duf_sql_sequence_t;

#  include "duf_base_types.h"
#  include "duf_sql_types.h"
typedef int ( *duf_bind_cb_t ) ( duf_sqlite_stmt_t * pstmt, const duf_argvc_t *arg );

#endif
