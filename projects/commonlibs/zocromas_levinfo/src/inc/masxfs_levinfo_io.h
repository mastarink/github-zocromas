#ifndef MASXFS_LEVINFO_IO_H
# define MASXFS_LEVINFO_IO_H

# include "masxfs_levinfo_types.h"
int masxfs_levinfo_fs_open( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_close( masxfs_levinfo_t * li );
int masxfs_levinfo_close_all_up( masxfs_levinfo_t * li );

int masxfs_levinfo_fs_stat( masxfs_levinfo_t * li );

#endif
