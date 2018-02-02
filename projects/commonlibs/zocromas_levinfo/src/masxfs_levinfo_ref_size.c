#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_format.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_refdef.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_ref_size.h"

#if 1
off_t
masxfs_levinfo_size_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  off_t size = 0;

  li = masxfs_levinfo_offset( li, offset );
  const struct stat *stat = masxfs_levinfo_stat_val( li, 0, tflags );

  if ( stat )
    size = stat->st_size;
  return size;
}

off_t
masxfs_levinfo_size_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  off_t size = 0;

  if ( masxfs_levinfo_stat_ref( li, tflags ) )
    size = masxfs_levinfo_size_val( li, 0, tflags );
  return size;
}
#endif
