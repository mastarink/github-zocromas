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

#if 1
int
masxfs_levinfo_fd_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->fd : 0;
}

#endif
#if 1
int
masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  int fd = 0;

  if ( li && !( tflags & MASXFS_CB_NO_FD ) )
  {
    int r = 0;

    r = masxfs_levinfo_open( li, tflags );
    if ( r >= 0 )
      fd = li->fd /*masxfs_levinfo_fd_val( li, 0 ) */ ;
  }
  return fd;
}
#endif
#if 0
masxfs_dirent_t * __attribute__ ( ( pure ) ) masxfs_levinfo_pde_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.scan.pde : li->db.pde ) : 0;
}

int masxfs_levinfo_is_open( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_fd_val" ) ) );

#endif
