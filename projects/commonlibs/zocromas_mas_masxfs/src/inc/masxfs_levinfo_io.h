#ifndef MASXFS_LEVINFO_IO_H
# define MASXFS_LEVINFO_IO_H

# include "masxfs_types.h"

masxfs_dir_t *masxfs_levinfo_openpath( masxfs_levinfo_t * li, const char *real_path );
masxfs_dir_t *masxfs_levinfo_openpath_free( masxfs_levinfo_t * li, char *real_path );

int masxfs_levinfo_opendirfd( masxfs_levinfo_t * li );
int masxfs_levinfo_closedirfd( masxfs_levinfo_t * li );
int masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li );

masxfs_dir_t *masxfs_levinfo_opendir_up( masxfs_levinfo_t * li );
void masxfs_levinfo_closedir( masxfs_levinfo_t * li );

#endif
