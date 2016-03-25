#ifndef MAS_DUF_LEVINFO_DIRHANDLE_STRUCTS_H
# define MAS_DUF_LEVINFO_DIRHANDLE_STRUCTS_H
# include <sys/stat.h>
/* # include <sys/types.h> */

# include "duf_levinfo_dirhandle_types.h"                            /* duf_dirhandle_t ✗ */

enum duf_dh_source_e
{
  DUF_DH_SOURCE_FS = 1,
  DUF_DH_SOURCE_DB,
  DUF_DH_SOURCE_MAX
};

struct duf_dirhandle_s
{
# if 0
  unsigned long long dirid;
# endif
  unsigned opened_copy:1;
  unsigned long serial;
  int dfd;
  int rs;
  int rdb;
  struct stat st;
  duf_dh_source_t source;
};

#endif
