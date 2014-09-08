#ifndef MAS_DUF_LEVINFO_TYPES_H
#  define MAS_DUF_LEVINFO_TYPES_H

#  include <sys/types.h>
#  include <sys/stat.h>

#  include "duf_fun_types.h"
#  include "duf_sql_types.h"
#  include "duf_item_types.h"
#  include "duf_ufilter_types.h"

typedef struct duf_dirhandle_s
{
  unsigned long long dirid;

  int dfd;
  int rs;
  struct stat st;
} duf_dirhandle_t;

typedef struct
{
  void *ptr;
  duf_void_voidp_t destructor;
} duf_context_t;


typedef struct
{
  unsigned is_leaf:1;
  unsigned deleted:1;
  /* unsigned eod; */
  unsigned long long dirid;
  /* const char *name; */
  duf_items_t items;
  long numdir;
  long numfile;
  char *fullpath;
  char *itemname;
  duf_context_t context;
  duf_dirhandle_t lev_dh;
} duf_levinfo_t;


typedef struct
{
  unsigned long long dirent_content2;
} duf_modcnts_t;

typedef struct
{
  int *id;
  duf_sqlite_stmt_t *pstmt;
} duf_idstmt_t;

typedef struct duf_depthinfo_s
{
  unsigned inited:1;
  unsigned opendir:1;
  unsigned maxdepth;
  int depth;                    /* signed !! */
  int topdepth;                 /* signed !! */
  /* duf_node_type_t node_type; */
  /* char *path; */
  duf_levinfo_t *levinfo;
  unsigned long long changes;
  unsigned long long seq;
  unsigned long long seq_leaf;
  unsigned long long seq_node;
  unsigned long long total_files;
  duf_items_t items;
  duf_ufilter_t u;
  duf_context_t context;
  int num_idstatements;
  /* duf_sqlite_stmt_t **statements; */
  duf_idstmt_t *idstatements;
  /* int **xstatements; */
  duf_modcnts_t cnts;
} duf_depthinfo_t;

#endif
