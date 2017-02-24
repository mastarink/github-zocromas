#ifndef MASXFS_STRUCTS_H
# define MASXFS_STRUCTS_H

# include "masxfs_types.h"

enum masxfs_entry_type_e
{
  MASXFS_ENTRY_UNKNOWN_NUM,
  MASXFS_ENTRY_BLK_NUM,
  MASXFS_ENTRY_CHR_NUM,
  MASXFS_ENTRY_DIR_NUM,
  MASXFS_ENTRY_FIFO_NUM,
  MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_LINK_NUM = MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_REG_NUM,
  MASXFS_ENTRY_SOCK_NUM,
};

enum masxfs_entry_type_bit_e
{
  MASXFS_ENTRY_UNKNOWN = 1L << MASXFS_ENTRY_UNKNOWN_NUM,
  MASXFS_ENTRY_BLK = 1L << MASXFS_ENTRY_BLK_NUM,
  MASXFS_ENTRY_CHR = 1L << MASXFS_ENTRY_CHR_NUM,
  MASXFS_ENTRY_DIR = 1L << MASXFS_ENTRY_DIR_NUM,
  MASXFS_ENTRY_FIFO = 1L << MASXFS_ENTRY_FIFO_NUM,
  MASXFS_ENTRY_LNK = 1L << MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_LINK = 1L << MASXFS_ENTRY_LINK_NUM,
  MASXFS_ENTRY_REG = 1L << MASXFS_ENTRY_REG_NUM,
  MASXFS_ENTRY_SOCK = 1L << MASXFS_ENTRY_SOCK_NUM,
};

enum masxfs_cb_flag_e
{
  MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_MULTIPLE_CBS_NUM,
  MASXFS_CB_SKIP_NUM,
};
enum masxfs_cb_flag_bit_e
{
  MASXFS_CB_NAME = 1L << MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH = 1L << MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT = 1L << MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD = 1L << MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE = 1L << MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_PREFIX = 1L << MASXFS_CB_PREFIX_NUM,
  MASXFS_CB_TRAILINGSLASH = 1L << MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_MULTIPLE_CBS = 1L << MASXFS_CB_MULTIPLE_CBS_NUM,
  MASXFS_CB_SKIP = 1L << MASXFS_CB_SKIP_NUM,
};

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
  masxfs_entry_type_t detype;
  ino_t deinode;
  int fd;
  masxfs_dir_t *pdir;
  masxfs_dirent_t *pde;
/* masxfs_dirent_t de; */
  masxfs_stat_t *stat;
/* masxfs_pathinfo_t *pi; */
  masxfs_depth_t lidepth;
  int error;
# if 0
  size_t child_count_z;
  size_t child_count;
# else
  size_t child_count_pair[2];
# endif

  char *path;
  char *prefix;
};
#endif
