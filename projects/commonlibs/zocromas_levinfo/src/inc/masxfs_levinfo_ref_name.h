#ifndef MASXFS_LEVINFO_REF_NAME_H
# define MASXFS_LEVINFO_REF_NAME_H

# include "masxfs_levinfo_types.h"

const char *masxfs_levinfo_name_val( masxfs_levinfo_t * li, masxfs_depth_t offset ) __attribute__ ( ( pure ) );
const char *masxfs_levinfo_name_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

const char *masxfs_levinfo_path_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );
const char *masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, masxfs_depth_t top_depth,
                                       masxfs_levinfo_flags_t tflags );

#endif
