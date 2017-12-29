#ifndef MASXFS_LEVINFO_FS_DIR_H
# define MASXFS_LEVINFO_FS_DIR_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li );
int masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t *entry_pfilter, int *phas_data );

#endif
