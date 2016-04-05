#ifndef MAS_DUF_FORMAT_TYPES_H
# define MAS_DUF_FORMAT_TYPES_H
# include "duf_defs.h"

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t */

# ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# endif


typedef struct
{
  char *dirs_tree;
  char *files_tree;

  char *prefix_gen_tree;
  char *prefix_files_tree;
  char *prefix_dirs_tree;

  char *dirs_list;
  char *files_list;

  char *dirs_gen;
  char *files_gen;
} duf_sformats_t;

typedef struct
{
  mas_argvc_t files;
  mas_argvc_t dirs;
} duf_filedirformat_t;
typedef struct
{
  duf_filedirformat_t tree;
  duf_filedirformat_t list;
  duf_filedirformat_t gen;
} duf_asformats_t;

#endif

/*
vi: ft=c
*/
