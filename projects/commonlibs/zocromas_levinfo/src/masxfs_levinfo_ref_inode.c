#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_refdef.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_ref_inode.h"

#if 1
ino_t
masxfs_levinfo_inode_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  ino_t inode = 0;

  li = masxfs_levinfo_offset( li, offset );
  const struct stat *stat = masxfs_levinfo_stat_val( li, 0, tflags );

  if ( stat )
    inode = stat->st_ino;
  return inode;
}

ino_t
masxfs_levinfo_inode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  ino_t inode = 0;

  if ( masxfs_levinfo_stat_ref( li, tflags ) )
    inode = masxfs_levinfo_inode_val( li, 0, tflags );
  return inode;
}
#else
ino_t __attribute__ ( ( pure ) ) masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags _uUu_ )
{
  return li ? li->deinode : 0;
}
#endif
