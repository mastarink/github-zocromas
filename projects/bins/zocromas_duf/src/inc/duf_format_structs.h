#ifndef MAS_DUF_FORMAT_STRUCTS_H
# define MAS_DUF_FORMAT_STRUCTS_H
# include "duf_defs.h"

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t; ▤ */

# include "duf_format_types.h"

struct duf_sformats_s
{
  char *files_list;
  char *files_gen;
  char *files_tree;
  char *prefix_gen_tree; 
  char *prefix_files_tree;

  char *dirs_gen;
  char *dirs_tree;
  char *dirs_list;
  char *prefix_dirs_tree;
};

enum duf_format_name_ids_e
{
  DUF_FORMAT_NAME_ID_MIN,
  DUF_FORMAT_NAME_ID_GEN = DUF_FORMAT_NAME_ID_MIN,
  DUF_FORMAT_NAME_ID_TREE,
  DUF_FORMAT_NAME_ID_LIST,
  DUF_FORMAT_NAME_ID_PACK,
  DUF_FORMAT_NAME_ID_GEN_TREE_PREFIX,
  DUF_FORMAT_NAME_ID_TREE_PREFIX,
  DUF_FORMAT_NAME_ID_DIRS,
  DUF_FORMAT_NAME_ID_DIRS_TREE_PREFIX,
  DUF_FORMAT_NAME_ID_MAX = DUF_FORMAT_NAME_ID_DIRS_TREE_PREFIX
};

struct duf_filedirformat_s
{
  mas_argvc_t files;
  mas_argvc_t dirs;
};

struct duf_asformats_s
{
  duf_filedirformat_t gen;
  duf_filedirformat_t tree;
  duf_filedirformat_t list;
  duf_filedirformat_t pack;
  duf_filedirformat_t gen_tree_prefix;
  duf_filedirformat_t tree_prefix;
  duf_filedirformat_t dirs;
  duf_filedirformat_t dirs_tree_prefix;
};

struct duf_format_block_s
{
  char *gen;
  char *tree;
  char *list;
  char *pack;
  char *gen_tree_prefix;
  char *tree_prefix;
  char *dirs;
  char *dirs_tree_prefix;
};

#endif

/*
vi: ft=c
*/
