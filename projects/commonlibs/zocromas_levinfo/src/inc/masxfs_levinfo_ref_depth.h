#ifndef MASXFS_LEVINFO_REF_DEPTH_H
# define MASXFS_LEVINFO_REF_DEPTH_H

# include "masxfs_levinfo_types.h"

masxfs_depth_t masxfs_levinfo_depth_val( masxfs_levinfo_t * li, masxfs_depth_t offset );
masxfs_depth_t masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

#endif
