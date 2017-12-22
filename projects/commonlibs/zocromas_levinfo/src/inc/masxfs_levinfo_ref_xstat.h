#ifndef MASXFS_LEVINFO_REF_XSTAT_H
# define MASXFS_LEVINFO_REF_XSTAT_H

# include "masxfs_levinfo_types.h"

unsigned long masxfs_levinfo_nsamesize_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags );
unsigned long masxfs_levinfo_nsamesize_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );
unsigned long masxfs_levinfo_nsamesha1_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags );
unsigned long masxfs_levinfo_nsamesha1_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );
const char *masxfs_levinfo_hexsha1_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags );
const char *masxfs_levinfo_hexsha1_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

#endif
