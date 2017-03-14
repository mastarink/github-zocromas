#ifndef MASXFS_LEVINFO_IO_DIR_H
# define MASXFS_LEVINFO_IO_DIR_H

# include "masxfs_levinfo_types.h"
int masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

/* int masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li ); */

int masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

/* int masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li ); */
/* int masxfs_levinfo_db_closedir( masxfs_levinfo_t * li ); */

int masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

masxfs_dirent_t *masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

/* masxfs_dirent_t *masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li ); */
/* masxfs_dirent_t *masxfs_levinfo_db_readdir( masxfs_levinfo_t * li ); */

int masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

/* int masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li ); */

const char *masxfs_levinfo_scanned_name( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_scanned_type( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
ino_t masxfs_levinfo_scanned_inode( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

#endif
