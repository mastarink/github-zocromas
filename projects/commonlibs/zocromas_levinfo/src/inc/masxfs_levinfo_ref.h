#ifndef MASXFS_LEVINFO_REF_H
# define MASXFS_LEVINFO_REF_H

# include "masxfs_levinfo_types.h"

masxfs_levinfo_t *masxfs_levinfo_offset( masxfs_levinfo_t * li, masxfs_depth_t offset ) __attribute__ ( ( pure ) );

const struct stat *masxfs_levinfo_stat_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );
const struct stat *masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

size_t masxfs_levinfo_size_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );
size_t masxfs_levinfo_size_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

int masxfs_levinfo_is_open( masxfs_levinfo_t * li );

int masxfs_levinfo_fd_val( masxfs_levinfo_t * li, masxfs_depth_t offset ) __attribute__ ( ( pure ) );
int masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

masxfs_dir_t *masxfs_levinfo_pdir_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );
/* masxfs_dirent_t *masxfs_levinfo_pde_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) ); */
masxfs_depth_t masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

ino_t masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

const char *masxfs_levinfo_name_val( masxfs_levinfo_t * li, masxfs_depth_t offset ) __attribute__ ( ( pure ) );
const char *masxfs_levinfo_name_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags ) __attribute__ ( ( pure ) );

const char *masxfs_levinfo_path_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );
const char *masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, masxfs_levinfo_flags_t tflags );

masxfs_entry_type_t masxfs_levinfo_detype( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

void masxfs_levinfo_set_node_id( masxfs_levinfo_t * li, unsigned long node_id );
unsigned long masxfs_levinfo_node_id( masxfs_levinfo_t * li, masxfs_depth_t offset );
int masxfs_levinfo_parent_id( masxfs_levinfo_t * li );

#endif
