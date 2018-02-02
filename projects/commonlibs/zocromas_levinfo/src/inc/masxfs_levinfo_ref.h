#ifndef MASXFS_LEVINFO_REF_H
# define MASXFS_LEVINFO_REF_H

# include "masxfs_levinfo_types.h"

const struct stat *masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

/* int masxfs_levinfo_is_open( masxfs_levinfo_t * li ); */

masxfs_dir_t *masxfs_levinfo_pdir_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

/* masxfs_dirent_t *masxfs_levinfo_pde_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) ); */

const char *masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, masxfs_depth_t top_depth,
                                       masxfs_levinfo_flags_t tflags );

masxfs_entry_type_t masxfs_levinfo_detype( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

int masxfs_levinfo_has_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

#endif
