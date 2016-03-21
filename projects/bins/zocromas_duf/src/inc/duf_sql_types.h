#ifndef MAS_DUF_SQL_TYPES_H
# define MAS_DUF_SQL_TYPES_H

# include <mastar/sqlite/mas_sqlite_types.h>
enum duf_sqltype_e {
   DUF_SQLTYPE_NONE,
   DUF_SQLTYPE_INTEGER,
   DUF_SQLTYPE_FLOAT,
   DUF_SQLTYPE_TEXT,
   DUF_SQLTYPE_BLOB,
   DUF_SQLTYPE_NULL,
};
typedef enum duf_sqltype_e duf_sqltype_t;
typedef mas_sqlite_stmt_t duf_stmnt_t;
typedef mas_sqexe_cb_t duf_sqexe_cb_t;

#endif
