#ifndef MASXFS_LEVINFO_ENUMS_H
# define MASXFS_LEVINFO_ENUMS_H

/* # include "masxfs_levinfo_types.h" */

enum masxfs_entry_type_e
{
  MASXFS_ENTRY_NONE_NUM,
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
  MASXFS_ENTRY_NONE = 1L << MASXFS_ENTRY_NONE_NUM,
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

enum masxfs_cb_type_e
{
  MASXFS_CBTYPE_NONE,
  MASXFS_CBTYPE_SIMPLE,
  MASXFS_CBTYPE_STAT,
};

enum masxfs_cb_flag_e
{
  MASXFS_CB_OFF_NAME_NUM,                                            // 0 // 1
  MASXFS_CB_PATH_NUM,                                                // 1 // 2
  MASXFS_CB_STAT_NUM,                                                // 2 // 4
  MASXFS_CB_NO_FD_NUM,                                               // 3 // 8
  MASXFS_CB_RECURSIVE_NUM,                                           // 4 // 10
  MASXFS_CB_PREFIX_NUM,                                              // 5 // 20
  MASXFS_CB_TRAILINGSLASH_NUM,                                       // 6 // 40
  MASXFS_CB_SINGLE_CB_NUM,                                           // 7 // 80
  MASXFS_CB_SKIP_NUM,                                                // 8 // 100
  MASXFS_CB_SKIP_EMPTY_NUM,                                          // 9 // 200
  MASXFS_CB_MODE_FS_NUM,                                             // 10 // 400
  MASXFS_CB_MODE_DB_NUM,                                             // 11 // 800
  MASXFS_CB_UP_ROOT_NUM,                                             // 12 // 1000
  MASXFS_CB_SELF_NUM,                                                // 13 // 2000
  MASXFS_CB_UP_NUM,                                                  // 14 // 4000
  MASXFS_CB_COUNT_NUM,                                               // 15 // 8000
  MASXFS_CB_FROM_ROOT_NUM,                                           // 16 // 10000
/* MASXFS_CB_CAN_UPDATE_DB_NUM, */
  MASXFS_CB_DIGESTS_NUM,                                             // 17 // 20000
  MASXFS_CB_USE_SIMPLE_CB_NUM,                                       // 18 // 40000
  MASXFS_CB_USE_STAT_CB_NUM,                                         // 19 // 80000
};
enum masxfs_cb_flag_bit_e
{
  MASXFS_CB_OFF_NAME = 1UL << MASXFS_CB_OFF_NAME_NUM,                // 1
  MASXFS_CB_PATH = 1UL << MASXFS_CB_PATH_NUM,                        // 2
  MASXFS_CB_STAT = 1UL << MASXFS_CB_STAT_NUM,                        // 4
  MASXFS_CB_NO_FD = 1UL << MASXFS_CB_NO_FD_NUM,                      // 8
  MASXFS_CB_RECURSIVE = 1UL << MASXFS_CB_RECURSIVE_NUM,              // 10
  MASXFS_CB_PREFIX = 1UL << MASXFS_CB_PREFIX_NUM,                    // 20
  MASXFS_CB_TRAILINGSLASH = 1UL << MASXFS_CB_TRAILINGSLASH_NUM,      // 40
  MASXFS_CB_SINGLE_CB = 1UL << MASXFS_CB_SINGLE_CB_NUM,              // 80
  MASXFS_CB_SKIP = 1UL << MASXFS_CB_SKIP_NUM,                        // 100
  MASXFS_CB_SKIP_EMPTY = 1UL << MASXFS_CB_SKIP_EMPTY_NUM,            // 200
  MASXFS_CB_MODE_FS = 1UL << MASXFS_CB_MODE_FS_NUM,                  // 400
  MASXFS_CB_MODE_DB = 1UL << MASXFS_CB_MODE_DB_NUM,                  // 800
  MASXFS_CB_UP_ROOT = 1UL << MASXFS_CB_UP_ROOT_NUM,                  // 1000
  MASXFS_CB_SELF = 1UL << MASXFS_CB_SELF_NUM,                        // 2000
  MASXFS_CB_UP = 1UL << MASXFS_CB_UP_NUM,                            // 4000
  MASXFS_CB_COUNT = 1UL << MASXFS_CB_COUNT_NUM,                      // 8000
  MASXFS_CB_FROM_ROOT = 1UL << MASXFS_CB_FROM_ROOT_NUM,              // 10000
  MASXFS_CB_SELF_N_UP = MASXFS_CB_SELF | MASXFS_CB_UP,
/* MASXFS_CB_CAN_UPDATE_DB = 1UL << MASXFS_CB_CAN_UPDATE_DB_NUM, */
  MASXFS_CB_DIGESTS = 1UL << MASXFS_CB_DIGESTS_NUM,                  // 20000
  MASXFS_CB_USE_SIMPLE_CB = 1UL << MASXFS_CB_USE_SIMPLE_CB_NUM,      // 40000
  MASXFS_CB_USE_STAT_CB = 1UL << MASXFS_CB_USE_STAT_CB_NUM,          // 80000
  MASXFS_CB_MODE_ALL = MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB,
};

#endif
