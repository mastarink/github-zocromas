#ifndef MASXFS_PATHINFO_IO_H
# define MASXFS_PATHINFO_IO_H

# include "masxfs_types.h"

int masxfs_pathinfo_opendir( masxfs_pathinfo_t * pi );
int masxfs_pathinfo_closedir( masxfs_pathinfo_t * pi );
int masxfs_pathinfo_closedir_all( masxfs_pathinfo_t * pi );

#endif
