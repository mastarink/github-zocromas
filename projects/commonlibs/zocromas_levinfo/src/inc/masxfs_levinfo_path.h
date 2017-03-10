#ifndef MASXFS_LEVINFO_PATH_H
# define MASXFS_LEVINFO_PATH_H

# include "masxfs_levinfo_types.h"

masxfs_levinfo_t *masxfs_levinfo_path2lia( const char *path, masxfs_depth_t depth_limit, masxfs_depth_t * psz );
char *masxfs_levinfo_lia2path( masxfs_levinfo_t * lia, masxfs_depth_t pidepth, char tail );
char *masxfs_levinfo_li2path_up( masxfs_levinfo_t * li, char tail );
char *masxfs_levinfo_li2path( masxfs_levinfo_t * li );

#endif
