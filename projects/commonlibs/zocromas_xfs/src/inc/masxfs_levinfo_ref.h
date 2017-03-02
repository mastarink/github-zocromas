#ifndef MASXFS_LEVINFO_REF_H
# define MASXFS_LEVINFO_REF_H

# include "masxfs_types.h"

const struct stat *masxfs_levinfo_stat_val( masxfs_levinfo_t * li );
const struct stat *masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, unsigned long tflags );

size_t masxfs_levinfo_size_val( masxfs_levinfo_t * li );
size_t masxfs_levinfo_size_ref( masxfs_levinfo_t * li, unsigned long tflags );

int masxfs_levinfo_is_open( masxfs_levinfo_t * li );

int masxfs_levinfo_fd_val( masxfs_levinfo_t * li );
int masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, unsigned long tflags );

masxfs_depth_t masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, unsigned long tflags );

ino_t masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, unsigned long tflags );

const char *masxfs_levinfo_name_val( masxfs_levinfo_t * li );
const char *masxfs_levinfo_name_ref( masxfs_levinfo_t * li, unsigned long tflags );

const char *masxfs_levinfo_path_ref( masxfs_levinfo_t * li, unsigned long tflags );
const char *masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, unsigned long tflags );

masxfs_entry_type_t masxfs_levinfo_detype( masxfs_levinfo_t * li );

void masxfs_levinfo_set_id( masxfs_levinfo_t * li, unsigned long id );
unsigned long masxfs_levinfo_id( masxfs_levinfo_t * li, masxfs_depth_t offset );
int masxfs_levinfo_parent_id( masxfs_levinfo_t * li );

#endif
