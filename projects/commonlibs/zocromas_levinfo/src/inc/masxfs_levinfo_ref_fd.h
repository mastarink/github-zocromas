#ifndef MASXFS_LEVINFO_REF_FD_H
# define MASXFS_LEVINFO_REF_FD_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_fd_val( masxfs_levinfo_t * li, masxfs_depth_t offset ) __attribute__ ( ( pure ) );
int masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

#endif
