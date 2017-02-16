#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_realpath( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_realpath( pi->levinfo, pi->depth );
}
