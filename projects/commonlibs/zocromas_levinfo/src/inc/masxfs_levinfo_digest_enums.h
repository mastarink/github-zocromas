#ifndef MASXFS_LEVINFO_DIGEST_ENUMS_H
# define MASXFS_LEVINFO_DIGEST_ENUMS_H

/* # include "masxfs_levinfo_types.h" */

enum masxfs_digest_type_e
{
  MASXFS_DIGEST_NONE,
  MASXFS_DIGEST_MAGIC,
  MASXFS_DIGEST_MD5,
  MASXFS_DIGEST_SHA1,
};

#endif
