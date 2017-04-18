#ifndef MASXFS_LEVINFO_DIGEST_H
# define MASXFS_LEVINFO_DIGEST_H

# include <openssl/md5.h>
# include <openssl/sha.h>
# include <magic.h>

typedef struct masxfs_digests_s masxfs_digests_t;
typedef struct masxfs_digest_s masxfs_digest_t;
typedef union masxfs_digest_ctx_u masxfs_digest_ctx_t;
typedef enum masxfs_digest_type_e masxfs_digest_type_t;

enum masxfs_digest_type_e
{
  MASXFS_DIGEST_NONE,
  MASXFS_DIGEST_MAGIC,
  MASXFS_DIGEST_MD5,
  MASXFS_DIGEST_SHA1,
};

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

masxfs_digests_t *masxfs_digests_create( void );
masxfs_digests_t *masxfs_digests_create_setup( int fd, size_t readbufsize, off_t from, off_t max_blocks );
void masxfs_digests_delete( masxfs_digests_t * digests );

int masxfs_digests_add( masxfs_digests_t * digests, masxfs_digest_type_t dgtype );

int masxfs_digests_open( masxfs_digests_t * digests );
int masxfs_digests_update( masxfs_digests_t * digests );
int masxfs_digests_close( masxfs_digests_t * digests );

int masxfs_digests_compute( masxfs_digests_t * digests );

int masxfs_digests_get( masxfs_digests_t * digests, masxfs_digest_type_t dgtype, const unsigned char **pbuf );
int masxfs_digests_getn( masxfs_digests_t * digests, int npos, const unsigned char **pbuf );

int
masxfs_digests_is_string( masxfs_digests_t * digests, int npos );

#endif
