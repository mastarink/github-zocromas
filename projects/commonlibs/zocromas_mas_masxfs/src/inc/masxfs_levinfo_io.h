#ifndef MASXFS_LEVINFO_IO_H
# define MASXFS_LEVINFO_IO_H

# include "masxfs_types.h"

int masxfs_levinfo_opendirfd( masxfs_levinfo_t * li );
int masxfs_levinfo_opendir( masxfs_levinfo_t * li );
int masxfs_levinfo_closedirfd( masxfs_levinfo_t * li );
int masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li );


masxfs_dir_t *masxfs_levinfo_opendir_up( masxfs_levinfo_t * li );
int masxfs_levinfo_closedir( masxfs_levinfo_t * li );
int masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li );

masxfs_dirent_t *masxfs_levinfo_readdir( masxfs_levinfo_t * li );
int masxfs_levinfo_rewinddir( masxfs_levinfo_t * li );

#endif
