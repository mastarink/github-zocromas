#ifndef MASXFS_STRUCTS_H
# define MASXFS_STRUCTS_H

# include <mastar/levinfo/masxfs_levinfo_types.h>
# include "masxfs_types.h"
# include "masxfs_enums.h"

struct masxfs_pathinfo_s
{
  masxfs_depth_t depth_limit;
  masxfs_depth_t pidepth;
  masxfs_levinfo_t *levinfo;
/* char *pathcache; */
/* char *realpathcache; */
  int error;
};

#endif
