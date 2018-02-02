#ifndef MASXFS_LEVINFO_REF_SIZE_H
# define MASXFS_LEVINFO_REF_SIZE_H

# include "masxfs_levinfo_types.h"


off_t masxfs_levinfo_size_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags );
off_t masxfs_levinfo_size_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );


#endif

