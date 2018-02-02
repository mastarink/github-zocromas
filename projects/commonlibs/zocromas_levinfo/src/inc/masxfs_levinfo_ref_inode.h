#ifndef MASXFS_LEVINFO_REF_INODE_H
# define MASXFS_LEVINFO_REF_INODE_H

# include "masxfs_levinfo_types.h"

/* ino_t masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) ); */

ino_t masxfs_levinfo_inode_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags );
ino_t masxfs_levinfo_inode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

#endif
