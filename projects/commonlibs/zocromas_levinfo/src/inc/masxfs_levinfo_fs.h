#ifndef MASXFS_LEVINFO_FS_H
# define MASXFS_LEVINFO_FS_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_fs_open_at( masxfs_levinfo_t * li, int fdparent );
int masxfs_levinfo_fs_open( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_opened( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_close( masxfs_levinfo_t * li );

int masxfs_levinfo_fs_stat( masxfs_levinfo_t * li, masxfs_stat_t ** pstat );

#endif
