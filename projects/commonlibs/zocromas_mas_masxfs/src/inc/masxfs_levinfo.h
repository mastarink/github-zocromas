#ifndef MASXFS_LEVINFO_H
# define MASXFS_LEVINFO_H

# include "masxfs_types.h"

masxfs_levinfo_t *masxfs_levinfo_path2levnfo( const char *path, size_t max_depth, masxfs_pathinfo_t * pi );
char *masxfs_levinfo_realpath( masxfs_levinfo_t * lia, size_t depth );
char *masxfs_levinfo_right_realpath( masxfs_levinfo_t * li );

#endif
