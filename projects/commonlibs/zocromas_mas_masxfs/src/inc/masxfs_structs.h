#ifndef MASXFS_STRUCTS_H
# define MASXFS_STRUCTS_H

# include "masxfs_types.h"

enum masxfs_entry_type_e
{
  MASXFS_ENTRY_UNKNOWN,
  MASXFS_ENTRY_BLK,
  MASXFS_ENTRY_CHR,
  MASXFS_ENTRY_DIR,
  MASXFS_ENTRY_FIFO,
  MASXFS_ENTRY_LNK,
  MASXFS_ENTRY_LINK = MASXFS_ENTRY_LNK,
  MASXFS_ENTRY_REG,
  MASXFS_ENTRY_SOCK,
};

enum masxfs_entry_type_bit_e
{
  MASXFS_ENTRY_BIT_UNKNOWN = 1 << MASXFS_ENTRY_UNKNOWN,
  MASXFS_ENTRY_BIT_BLK = 1 << MASXFS_ENTRY_BLK,
  MASXFS_ENTRY_BIT_CHR = 1 << MASXFS_ENTRY_CHR,
  MASXFS_ENTRY_BIT_DIR = 1 << MASXFS_ENTRY_DIR,
  MASXFS_ENTRY_BIT_FIFO = 1 << MASXFS_ENTRY_FIFO,
  MASXFS_ENTRY_BIT_LNK = 1 << MASXFS_ENTRY_LNK,
  MASXFS_ENTRY_BIT_LINK = 1 << MASXFS_ENTRY_LINK,
  MASXFS_ENTRY_BIT_REG = 1 << MASXFS_ENTRY_REG,
  MASXFS_ENTRY_BIT_SOCK = 1 << MASXFS_ENTRY_SOCK,
};

struct masxfs_entry_callback_s
{
  unsigned types;
  masxfs_scan_fun_simple_t fun_simple;
};

struct masxfs_pathinfo_s
{
  size_t max_depth;
  size_t depth;
  masxfs_levinfo_t *levinfo;
  char *pathcache;
  char *realpathcache;
};

struct masxfs_levinfo_s
{
  char *name;
  masxfs_dir_t *dir;
  masxfs_dirent_t *de;
  masxfs_stat_t st;
  masxfs_pathinfo_t *pi;
  size_t depth;
};
#endif
