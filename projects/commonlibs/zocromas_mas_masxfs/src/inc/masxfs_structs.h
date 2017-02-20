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
  MASXFS_ENTRY_UNKNOWN = 1 << MASXFS_ENTRY_UNKNOWN_NUM,
  MASXFS_ENTRY_BLK = 1 << MASXFS_ENTRY_BLK_NUM,
  MASXFS_ENTRY_CHR = 1 << MASXFS_ENTRY_CHR_NUM,
  MASXFS_ENTRY_DIR = 1 << MASXFS_ENTRY_DIR_NUM,
  MASXFS_ENTRY_FIFO = 1 << MASXFS_ENTRY_FIFO_NUM,
  MASXFS_ENTRY_LNK = 1 << MASXFS_ENTRY_LNK_NUM,
  MASXFS_ENTRY_LINK = 1 << MASXFS_ENTRY_LINK_NUM,
  MASXFS_ENTRY_REG = 1 << MASXFS_ENTRY_REG_NUM,
  MASXFS_ENTRY_SOCK = 1 << MASXFS_ENTRY_SOCK_NUM,
};

enum masxfs_cb_flag_e
{
  MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_AT_PARENT_NUM,
  MASXFS_CB_AT_CHILD_NUM,
};
enum masxfs_cb_flag_bit_e
{
  MASXFS_CB_NAME = 1 << MASXFS_CB_NAME_NUM,
  MASXFS_CB_PATH = 1 << MASXFS_CB_PATH_NUM,
  MASXFS_CB_STAT = 1 << MASXFS_CB_STAT_NUM,
  MASXFS_CB_FD = 1 << MASXFS_CB_FD_NUM,
  MASXFS_CB_RECURSIVE = 1 << MASXFS_CB_RECURSIVE_NUM,
  MASXFS_CB_TRAILINGSLASH = 1 << MASXFS_CB_TRAILINGSLASH_NUM,
  MASXFS_CB_AT_PARENT = 1 << MASXFS_CB_AT_PARENT_NUM,
  MASXFS_CB_AT_CHILD = 1 << MASXFS_CB_AT_CHILD_NUM,
};

struct masxfs_entry_callback_s
{
  unsigned types;
  masxfs_scan_fun_simple_t fun_simple;
  unsigned long flags;
};

struct masxfs_pathinfo_s
{
  size_t max_depth;
  size_t pidepth;
  masxfs_levinfo_t *levinfo;
/* char *pathcache; */
/* char *realpathcache; */
  int error;
};

struct masxfs_levinfo_s
{
  char *name;
  masxfs_entry_type_t detype;
  int fd;
  masxfs_dir_t *pdir;
  masxfs_dirent_t *pde;
/* masxfs_dirent_t de; */
  masxfs_stat_t *stat;
/* masxfs_pathinfo_t *pi; */
  size_t lidepth;
  int error;
};
#endif
