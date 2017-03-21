#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
/* #include "masxfs_structs.h" */

/* #include "masxfs_levinfo_base.h" */
/* #include "masxfs_levinfo_tools.h" */
/* #include "masxfs_levinfo_io.h" */
/* #include "masxfs_levinfo_io_dir.h" */
/* #include "masxfs_levinfo_ref.h" */

#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo.h"

masxfs_levinfo_t *
masxfs_levinfo_root( masxfs_levinfo_t * li )
{
  return li ? li - li->lidepth : NULL;
}

masxfs_levinfo_t *masxfs_levinfo_li2lia( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_root" ) ) );
