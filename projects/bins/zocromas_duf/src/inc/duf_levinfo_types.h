#ifndef MAS_DUF_LEVINFO_TYPES_H
#  define MAS_DUF_LEVINFO_TYPES_H

#  include <sys/types.h>
#  include <sys/stat.h>

#  include "duf_fun_types.h"
#  include "duf_sql_types.h"
#  include "duf_item_types.h"
#  include "duf_ufilter_types.h"

typedef enum
{
  DUF_DH_SOURCE_FS = 1,
  DUF_DH_SOURCE_DB,
  DUF_DH_SOURCE_MAX
} duf_dh_source_t;

typedef struct duf_dirhandle_s
{
#if 0
  unsigned long long dirid;
#endif
  int dfd;
  int rs;
  int rdb;
  struct stat st;
  duf_dh_source_t source;
} duf_dirhandle_t;

typedef struct
{
  void *ptr;
  duf_void_voidp_t destructor;
} duf_levinfo_context_t;

typedef struct
{
  unsigned long long dirent_content2;
} duf_modcnts_t;

typedef struct
{
  int *id;
  duf_sqlite_stmt_t *pstmt;
} duf_idstmt_t;

/********************************************************************************************/

typedef struct
{
  unsigned is_leaf:1;
  unsigned deleted:1;
  /* unsigned eod; */
  unsigned long long dirid;
  unsigned long long nameid;
  /* const char *name; */
  duf_items_t items;
  long numdir;
  long numfile;
  char *fullpath;
  char *itemname;
  duf_levinfo_context_t context;
  duf_dirhandle_t lev_dh;
} duf_levinfo_t;


typedef struct duf_depthinfo_s
{
  unsigned inited:1;
  unsigned opendir:1;
  unsigned recursive:1;
  unsigned attached_selected:1;
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
  duf_items_t items;
  const duf_ufilter_t *pu;
  duf_levinfo_context_t context;
  int num_idstatements;
  /* duf_sqlite_stmt_t **statements; */
  duf_idstmt_t *idstatements;
  char *selected_db;
  /* int **xstatements; */
  duf_modcnts_t cnts;
} duf_depthinfo_t;

#endif

/*
vi: ft=c
*/
