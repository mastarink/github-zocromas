#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_base.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo_ref.h"

masxfs_depth_t
masxfs_pathinfo_pidepth( const masxfs_pathinfo_t * pi )
{
  return pi ? pi->pidepth : 0;
}

masxfs_levinfo_t *
masxfs_pathinfo_li( const masxfs_pathinfo_t * pi, masxfs_depth_t d )
{
  return pi && pi->levinfo && d >= 0 && d < pi->pidepth ? pi->levinfo + d : NULL;
}
