#ifndef MASXFS_LEVINFO_STRUCTS_H
# define MASXFS_LEVINFO_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>
# include "masxfs_levinfo_types.h"
# include "masxfs_levinfo_enums.h"

struct masxfs_entry_callback_s
{
  unsigned types;
  masxfs_scan_fun_simple_t fun_simple;
  unsigned long flags;
};

struct masxfs_levinfo_s
{
  char *name;
  masxfs_entry_type_t detype;
  /* ino_t deinode; */
  int fd;
  unsigned fixed:1;
  unsigned no_more:1;
  masxfs_depth_t lidepth;
  int error;
  size_t child_count_pair[2];

  struct
  {
    struct
    {
      masxfs_dir_t *pdir;
      /* masxfs_dirent_t *pde; */
    } scan;
    masxfs_stat_t *stat;
  } fs;

  struct
  {
    struct
    {
      mysqlpfs_mstmt_t *mstmt;
      unsigned long long node_id;
      /* masxfs_entry_type_t type; */
      /* ino_t inode; */
      /* masxfs_stat_t *stat; */
    } scan;
    unsigned long long node_id;
  /* masxfs_dirent_t de; */
  /* masxfs_dirent_t *pde; */
    masxfs_stat_t *stat;
  } db;

  char *path;
  char *prefix;
};
#endif
