#ifndef MASXFS_LEVINFO_STRUCTS_H
# define MASXFS_LEVINFO_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>
//typedef struct mysqlpfs_mstmt_s mysqlpfs_mstmt_t;

# include "masxfs_levinfo_digest_types.h"
# include "masxfs_levinfo_types.h"
# include "masxfs_levinfo_enums.h"

struct masxfs_entry_filter_s
{
  masxfs_type_flags_t typeflags;
  masxfs_depth_t maxdepth;
  off_t min_size;
  off_t max_size;
  unsigned long min_nsame_digest;
  unsigned long max_nsame_digest;
  unsigned long min_nsamesize;
  unsigned long max_nsamesize;
  unsigned long min_inode;
  unsigned long max_inode;
  unsigned long min_digid;
  unsigned long max_digid;
  char *glob;
  char *regexp;
};
struct masxfs_entry_callback_s
{
  unsigned long fun_counter;
  union
  {
    masxfs_cb_type_t cb_type;
    masxfs_cb_type_t cbt;
  };
/* unsigned types; */
  union
  {
    masxfs_levinfo_flags_t flags;
    masxfs_levinfo_flags_t f;
  };
/* masxfs_entry_filter_t entry_filter; */
  masxfs_depth_t fun_top_depth;
/* union        */
/* {            */
/*   void *fun; */
  union
  {
    void *fun;
    masxfs_scan_fun_simple_t fun_simple;
    masxfs_scan_fun_stat_t fun_stat;
  };
/* }; */
};
struct masxfs_scanner_s
{                                                                    /* TODO */
  masxfs_entry_filter_t *entry_pfilter;
  masxfs_entry_callback_t *cbs;
  masxfs_levinfo_flags_t flags;
  masxfs_depth_t maxdepth;
};
struct masxfs_xstatd_s
{
  unsigned long nsamesize;
  unsigned long nsamedigest;
  unsigned long long digestid;
};

struct masxfs_xstati_s
{
  unsigned long long parentid;
  unsigned long long nameid;
  unsigned long long dataid;
};

struct masxfs_xstat_s
{
  masxfs_xstatd_t dg;
/* unsigned long nsamesize;   */
/* unsigned long nsamedigest; */
/* unsigned long digestid;    */
  masxfs_xstati_t id;
  char *hex_digest;
};

struct masxfs_xstatc_s
{
  masxfs_xstatd_t dg;
/* unsigned long nsamesize;   */
/* unsigned long nsamedigest; */
/* unsigned long digestid;    */
  const char *chex_digest;
  masxfs_xstati_t id;
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
  size_t leaf_count;

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
    masxfs_xstat_t *xstat;
  } db;
  masxfs_digests_t *digests;
  char *path;
  char *prefix;
};
#endif
