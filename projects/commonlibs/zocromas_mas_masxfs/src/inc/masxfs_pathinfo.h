#ifndef MASXFS_PATHINFO_H
# define MASXFS_PATHINFO_H

# include "masxfs_types.h"

char *masxfs_pathinfo_normal_path( masxfs_pathinfo_t * pi, const char *name );
char *masxfs_pathinfo_realpath( masxfs_pathinfo_t * pi );
int masxfs_pathinfo_scan( masxfs_pathinfo_t * pi, masxfs_entry_callback_t * callbacks, int recursive );
int masxfs_pathinfo_scandir( masxfs_pathinfo_t * pi, DIR * dir, masxfs_entry_callback_t * callbacks, int recursive );

#endif
