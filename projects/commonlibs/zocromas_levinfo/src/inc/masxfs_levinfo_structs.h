#ifndef MASXFS_LEVINFO_STRUCTS_H
# define MASXFS_LEVINFO_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>
# include "masxfs_levinfo_types.h"
# include "masxfs_levinfo_enums.h"

struct masxfs_entry_callback_s
{
  /* unsigned types; */
  masxfs_scan_fun_simple_t fun_simple;
  unsigned long flags;
};

struct masxfs_levinfo_s
{
  char *name;
  masxfs_entry_type_t detype;
  int fd;
  /* unsigned fixed:1; */
  /* unsigned no_more:1; */
  masxfs_depth_t lidepth;
  int error;
  size_t child_count_pair[2];

  struct
  {
    struct
    {
      masxfs_dir_t *pdir;
    } scan;
    masxfs_stat_t *stat;
  } fs;

  struct
  {
    struct
    {
      mysqlpfs_mstmt_t *mstmt;
      unsigned long long node_id;
    } scan;
    unsigned long long node_id;
    masxfs_stat_t *stat;
  } db;

  char *path;
  char *prefix;
};
#endif
