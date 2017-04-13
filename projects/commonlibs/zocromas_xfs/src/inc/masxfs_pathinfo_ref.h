#ifndef MASXFS_PATHINFO_REF_H
# define MASXFS_PATHINFO_REF_H

masxfs_depth_t masxfs_pathinfo_pidepth( const masxfs_pathinfo_t * pi );
masxfs_levinfo_t *masxfs_pathinfo_li( const masxfs_pathinfo_t * pi, masxfs_depth_t d );

#endif
