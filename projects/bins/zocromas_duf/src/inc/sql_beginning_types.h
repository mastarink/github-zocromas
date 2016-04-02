#ifndef MAS_SQL_BEGINNING_TYPES_H
# define MAS_SQL_BEGINNING_TYPES_H

typedef struct
{
  const char *name;
  unsigned done;
  unsigned beginend;
  unsigned set_selected_db;
  const char *sql[];
} duf_sql_sequence_t;

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t; ▤ */

# include "duf_base_types.h"
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */

typedef int ( *duf_bind_cb_t ) ( duf_stmnt_t * pstmt_x, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * arg,
                                 const void *ptr );

#endif
