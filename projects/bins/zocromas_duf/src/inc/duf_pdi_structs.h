#ifndef MAS_DUF_PDI_STRUCTS_H
# define MAS_DUF_PDI_STRUCTS_H
# include <stddef.h>                                                 /* size_t */

# include "duf_fun_types.h"                                          /* duf_void_voidp_func_t etc. etc. ... ✗ */

/* # include "duf_pathinfo_types.h"                                     (* duf_pathinfo_t ✗ *) */
# include "duf_pathinfo_structs.h"                                   /* duf_pathinfo_s; (from duf_pathinfo_types: duf_pathinfo_t ) ✗ */
# include "duf_levinfo_stmt_types.h"                                 /* duf_stmt_ident_t; duf_idstmt_t; ✗ */
# include "duf_item_types.h"                                         /* duf_items_t ✗ */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_seq_structs.h"

/* # include "duf_levinfo_structs.h" */

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

struct duf_modcnts_s
{
  unsigned long long dirent_content2;
};
struct duf_pdi_context_s
{
  void *ptr;
  duf_void_voidp_func_t destructor;
};
struct duf_depthinfo_s
{
  unsigned created_name:1;
  unsigned attached_copy:1;
  unsigned inited:1;
  unsigned sql_beginning_done:1;
  unsigned set_selected_db:1;
  unsigned opendir:1;
  unsigned recursive:1;
  unsigned allow_dirs:1;
  unsigned linear:1;
  unsigned pyp_created:1;

  char *db_attached_selected;
  char *pdi_name;
/* unsigned maxdepth; */
# if 0
  int depth;                                                         /* signed !! */
  int topdepth;                                                      /* signed !! */
/* duf_node_type_t node_type; */
/* char *path; */
  duf_levinfo_t *levinfo;
# else
  duf_pathinfo_t pathinfo;
# endif
  unsigned long long changes;
# if 0
  unsigned long long seq;
  unsigned long long seq_leaf;
  unsigned long long seq_node;
  unsigned long long seq_row;
# else
  union
  {
    unsigned long long aseq[4];
    seq_t seqq;
  };
# endif
  unsigned long long total_bytes;
  unsigned long long total_files;
  duf_items_t items;
  const duf_ufilter_t *pup;
  duf_yfilter_t *pyp;
# if 0
  duf_levinfo_context_t context;
# else
  struct duf_pdi_context_s context;
# endif
  int num_idstatements;
/* duf_stmnt_t **statements; */
  duf_idstmt_t *idstatements;
/* int **xstatements; */
  struct duf_modcnts_s cnts;
  struct duf_depthinfo_s *root_pdi;
  struct duf_depthinfo_s *next;
  duf_stmnt_t *each_pstmt_linear;
};

#endif

/*
vi: ft=c
*/
