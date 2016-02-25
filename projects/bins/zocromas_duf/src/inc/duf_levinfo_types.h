#ifndef MAS_DUF_LEVINFO_TYPES_H
# define MAS_DUF_LEVINFO_TYPES_H

# include <sys/stat.h>                                               /* struct stat */

# include "duf_fun_types.h"                                          /* duf_void_voidp_func_t */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ♠ */
# include "duf_item_types.h"                                         /* duf_items_t */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t */
# include "duf_scan_types.h"                                         /* duf_node_type_t ♠ */
# include "duf_levinfo_dirhandle_types.h"                            /* duf_dirhandle_t ♠ */
# include "duf_levinfo_stmt_types.h"                                 /* duf_idstmt_t */

typedef struct
{
  void *ptr;
  duf_void_voidp_func_t destructor;
} duf_levinfo_context_t;

typedef struct
{
  unsigned long long dirid;
  unsigned long long nameid;
} duf_levinfo_db_t;

typedef struct
{
  unsigned long long nodes;
  unsigned long long leaves;
} duf_levinfo_child_count_t;

# include "duf_defs.h"
typedef struct
{
/* unsigned is__leaf:1; */
  unsigned stat_tested:1;
  unsigned deleted:1;
  unsigned deleted_tested:1;
/* unsigned eod; */
  duf_node_type_t node_type;
  duf_levinfo_db_t db;
/* const char *name; */
# ifndef  MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# elif defined( DUF_DO_NUMS )
  long numdir;
  long numfile;
# else
  unsigned long long childs;                                         /* not used!? */
  long long numchild;
# endif
  duf_levinfo_child_count_t scanned_childs;
  char *fullpath;
  char *itemname;
  duf_levinfo_context_t context;
  duf_dirhandle_t lev_dh;
  int d;
} duf_levinfo_t;

#endif

/*
vi: ft=c
*/
