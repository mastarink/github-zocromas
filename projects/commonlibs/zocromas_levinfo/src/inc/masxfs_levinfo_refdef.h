#ifndef MASXFS_LEVINFO_REFDEF_H
# define MASXFS_LEVINFO_REFDEF_H

# include "masxfs_levinfo_types.h"

static inline masxfs_levinfo_t * __attribute__ ( ( pure ) ) masxfs_levinfo_offset( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  return li ? li + offset : NULL;
}

static inline const struct stat *
masxfs_levinfo_stat_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  const struct stat *st = NULL;

  li = masxfs_levinfo_offset( li, offset );
  st = li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.stat : ( tflags & MASXFS_CB_MODE_DB ? li->db.stat : NULL ) ) : NULL;
  return st;
}

#endif
