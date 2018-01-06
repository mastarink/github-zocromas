#ifndef MASXFS_LEVINFO_DIGEST_STRUCTS_H
# define MASXFS_LEVINFO_DIGEST_STRUCTS_H

# include <openssl/md5.h>
# include <openssl/sha.h>
# include <magic.h>

//# include <mastar/mysqlpfs/mysqlpfs_types.h>
typedef struct mysqlpfs_mstmt_s mysqlpfs_mstmt_t;

# include "masxfs_levinfo_digest_types.h"
# include "masxfs_levinfo_digest_enums.h"
/* # include "masxfs_levinfo_types.h" */
/* # include "masxfs_levinfo_enums.h" */

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
#endif

