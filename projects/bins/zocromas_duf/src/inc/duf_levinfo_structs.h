#ifndef MAS_DUF_LEVINFO_STRUCTS_H
# define MAS_DUF_LEVINFO_STRUCTS_H

# include <sys/stat.h>                                               /* struct stat */

# include "duf_defs.h"

# include "duf_fun_types.h"                                          /* duf_void_voidp_func_t etc. etc. ... ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */
# include "duf_item_types.h"                                         /* duf_items_t */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_levinfo_dirhandle_structs.h"
# include "duf_levinfo_stmt_types.h"                                 /* duf_stmt_ident_t; duf_idstmt_t; ✗ */
# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */

# if 0
struct duf_levinfo_context_s
{
  void *ptr;
  duf_void_voidp_func_t destructor;
};
# endif

struct duf_levinfo_db_s
{
  unsigned long long dirid;
  unsigned long long nameid;
};

struct duf_levinfo_child_count_s
{
  unsigned long long nodes;
  unsigned long long leaves;
};

struct duf_levinfo_s
{
/* unsigned is__leaf:1; */
  unsigned stat_tested:1;
  unsigned deleted:1;
  unsigned deleted_tested:1;
/* unsigned eod; */
  duf_node_type_t node_type;
  duf_levinfo_db_t db;
/* const char *name; */
  unsigned long long childs;                                         /* not used!? */
  long long numchild;
  duf_levinfo_child_count_t scanned_childs;
  char *fullpath;
  char *itemname;
/* duf_levinfo_context_t context; */
  duf_dirhandle_t lev_dh;
  int d;
  duf_stmnt_t *each_pstmt;
};

#endif

/*
vi: ft=c
*/
