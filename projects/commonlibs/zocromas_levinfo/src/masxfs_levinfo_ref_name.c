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

const char * __attribute__ ( ( pure ) ) masxfs_levinfo_name_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->name : NULL;
}

const char * __attribute__ ( ( pure ) ) masxfs_levinfo_name_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  const char *name = NULL;

  if ( li )
  {
    if ( !( tflags & MASXFS_CB_OFF_NAME ) )
    {
      name = masxfs_levinfo_name_val( li, 0 );
    }
  }
  return name;
}

const char *
masxfs_levinfo_path_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  const char *path = NULL;

  if ( li )
  {
    if ( ( tflags & MASXFS_CB_PATH ) )
    {
      if ( li->path )
        mas_free( li->path );
      path = li->path = masxfs_levinfo_li2path_up( li, ( li->detype == MASXFS_ENTRY_DIR_NUM && ( tflags & MASXFS_CB_TRAILINGSLASH ) ) ? '/' : 0 );
    }
  }
  return path;
}
