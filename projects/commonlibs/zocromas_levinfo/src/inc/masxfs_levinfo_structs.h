#ifndef MASXFS_LEVINFO_STRUCTS_H
# define MASXFS_LEVINFO_STRUCTS_H

# include <openssl/md5.h>
# include <openssl/sha.h>
# include <magic.h>

//# include <mastar/mysqlpfs/mysqlpfs_types.h>
typedef struct mysqlpfs_mstmt_s mysqlpfs_mstmt_t;

# include "masxfs_levinfo_types.h"
# include "masxfs_levinfo_enums.h"

union masxfs_digest_ctx_u
{
  void *any;
  MD5_CTX *md5;
  SHA_CTX *sha1;
  magic_t magic;
};

struct masxfs_digests_s
{
  int fd;
  off_t from;
  off_t max_blocks;
  masxfs_digest_t *digest;
  ssize_t bytes2update;
  int reads;
  ssize_t read_bytes;
  ssize_t readbufsize;
  char *readbuffer;
};

struct masxfs_digest_s
{
  masxfs_digest_type_t dgtype;
  masxfs_digest_t *next;
  masxfs_digest_ctx_t ctx;
  unsigned char *sum;
};
struct masxfs_entry_filter_s
{
  masxfs_type_flags_t typeflags;
  masxfs_depth_t maxdepth;
};
struct masxfs_entry_callback_s
{
/* unsigned types; */
  masxfs_scan_fun_simple_t fun_simple;
  masxfs_levinfo_flags_t flags;
};
struct masxfs_xstatc_s
{
  unsigned long nsamesize;
  unsigned long nsamesha1;
  const char *hex_sha1;
};
struct masxfs_xstat_s
{
  unsigned long nsamesize;
  unsigned long nsamesha1;
  char *hex_sha1;
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
    masxfs_xstat_t *xstat;
  } db;
  masxfs_digests_t *digests;
  char *path;
  char *prefix;
};
#endif
