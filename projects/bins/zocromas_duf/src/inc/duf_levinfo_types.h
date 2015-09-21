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
#  if 0
  unsigned long long dirid;
#  endif
  unsigned opened_copy:1;
  unsigned long serial;
  int dfd;
  int rs;
  int rdb;
  struct stat st;
  duf_dh_source_t source;
} duf_dirhandle_t;

typedef struct
{
  void *ptr;
  duf_void_voidp_func_t destructor;
} duf_levinfo_context_t;

typedef struct
{
  unsigned long long dirent_content2;
} duf_modcnts_t;

typedef enum
{
  DUF_SQL_STMD_ID_select_filedata = 0x10000000,
  DUF_SQL_STMD_ID_insert_filedata,
  DUF_SQL_STMD_ID_insert_path_table,
  DUF_SQL_STMD_ID_select_path,
  DUF_SQL_STMD_ID_dirid2name_existed,
  DUF_SQL_STMD_ID_insert_tagname,
  DUF_SQL_STMD_ID_select_tagname,
  DUF_SQL_STMD_ID_insert_tag,
  DUF_SQL_STMD_ID_select_tag,
  DUF_SQL_STMD_ID_insert_filename,
  DUF_SQL_STMD_ID_select_md5,
  DUF_SQL_STMD_ID_insert_md5,
  DUF_SQL_STMD_ID_update_md5id,
  DUF_SQL_STMD_ID_select_sd5,
  DUF_SQL_STMD_ID_insert_sd5,
  DUF_SQL_STMD_ID_update_sd5id,
  DUF_SQL_STMD_ID_select_sha1,
  DUF_SQL_STMD_ID_insert_sha1,
  DUF_SQL_STMD_ID_update_sha1id,
  DUF_SQL_STMD_ID_select_crc32,
  DUF_SQL_STMD_ID_insert_crc32,
  DUF_SQL_STMD_ID_update_crc32id,
  DUF_SQL_STMD_ID_select_mime,
  DUF_SQL_STMD_ID_insert_mime,
  DUF_SQL_STMD_ID_update_mime,
  DUF_SQL_STMD_ID_select_model,
  DUF_SQL_STMD_ID_insert_model,
  DUF_SQL_STMD_ID_update_model,
  DUF_SQL_STMD_ID_select_exif,
  DUF_SQL_STMD_ID_insert_exif,
  DUF_SQL_STMD_ID_update_exif,
} duf_stmt_ident_t;

typedef struct
{
#if 0
  const int *id; /* just as id */
#else
  duf_stmt_ident_t id;
#endif
  duf_stmnt_t *pstmt;
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

typedef struct
{
  unsigned maxdepth;
  int depth;                    /* signed !! */
  int topdepth;                 /* signed !! */
  /* duf_node_type_t node_type; */
  /* char *path; */
  size_t levinfo_count;
  duf_levinfo_t *levinfo;
} duf_pathinfo_t;

typedef struct duf_depthinfo_s
{
  unsigned created_name:1;
  unsigned attached_copy:1;
  unsigned inited:1;
  unsigned sql_beginning_done:1;
  unsigned opendir:1;
  unsigned recursive:1;

  char *db_attached_selected;
  char *pdi_name;
  /* unsigned maxdepth; */
#  if 0
  int depth;                    /* signed !! */
  int topdepth;                 /* signed !! */
  /* duf_node_type_t node_type; */
  /* char *path; */
  duf_levinfo_t *levinfo;
#  else
  duf_pathinfo_t pathinfo;
#  endif
  unsigned long long changes;
  unsigned long long seq;
  unsigned long long seq_leaf;
  unsigned long long seq_node;
  duf_items_t items;
  const duf_ufilter_t *pup;
  duf_levinfo_context_t context;
  int num_idstatements;
  /* duf_stmnt_t **statements; */
  duf_idstmt_t *idstatements;
  /* int **xstatements; */
  duf_modcnts_t cnts;
  struct duf_depthinfo_s *root_pdi;
} duf_depthinfo_t;

#endif

/*
vi: ft=c
*/
