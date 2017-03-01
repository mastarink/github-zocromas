#include "masxfs_defs.h"
#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_structs.h"

#include "masxfs_levinfo_io_dir.h"
#include "masxfs_levinfo_tools.h"

#include "masxfs_pathinfo.h"

#include "masxfs_pathinfo_io.h"

#if 0
int
masxfs_pathinfo_opendir( masxfs_pathinfo_t * pi )
{
  int r = 0;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  r = masxfs_levinfo_opendir( li );
  QRPI( pi, r );
  return r;
}

int
masxfs_pathinfo_closedir( masxfs_pathinfo_t * pi )
{
  int r = 0;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  r = masxfs_levinfo_closedir( li );
  QRPI( pi, r );
  return r;
}

int
masxfs_pathinfo_closedir_all( masxfs_pathinfo_t * pi )
{
  int r = 0;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  r = masxfs_levinfo_closedir_all_up( li );
  QRPI( pi, r );
  return r;
}
#endif
