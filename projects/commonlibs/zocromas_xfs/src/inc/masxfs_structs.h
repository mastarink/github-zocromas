#ifndef MASXFS_STRUCTS_H
# define MASXFS_STRUCTS_H

# include "masxfs_types.h"
# include "masxfs_enums.h"

struct masxfs_entry_callback_s
{
  unsigned types;
  masxfs_scan_fun_simple_t fun_simple;
  unsigned long flags;
};

struct masxfs_pathinfo_s
{
  masxfs_depth_t depth_limit;
  masxfs_depth_t pidepth;
  masxfs_levinfo_t *levinfo;
/* char *pathcache; */
/* char *realpathcache; */
  int error;
};

struct masxfs_levinfo_s
{
  char *name;
  unsigned long id;
  masxfs_entry_type_t detype;
  ino_t deinode;
  int fd;
  masxfs_dir_t *pdir;
  masxfs_dirent_t *pde;
  masxfs_stat_t *stat;
  masxfs_depth_t lidepth;
  int error;
  size_t child_count_pair[2];

  char *path;
  char *prefix;
};
#endif
