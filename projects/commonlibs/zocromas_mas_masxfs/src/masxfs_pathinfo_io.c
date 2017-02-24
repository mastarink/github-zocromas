#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_levinfo_io_dir.h"
#include "masxfs_pathinfo.h"

#include "masxfs_pathinfo_io.h"

int
masxfs_pathinfo_opendir( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_opendir( masxfs_pathinfo_last_li( pi ) );
}

int
masxfs_pathinfo_closedir( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_closedir( masxfs_pathinfo_last_li( pi ) );
}

int
masxfs_pathinfo_closedir_all( masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  return masxfs_levinfo_closedir_all_up( li );
}
