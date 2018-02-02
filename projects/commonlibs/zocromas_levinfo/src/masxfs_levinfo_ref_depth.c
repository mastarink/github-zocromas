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

masxfs_depth_t
masxfs_levinfo_depth_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->lidepth : 0;
}

masxfs_depth_t __attribute__ ( ( pure ) ) masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags _uUu_ )
{
  return masxfs_levinfo_depth_val( li, 0 );
}
