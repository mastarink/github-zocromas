#ifndef MASXFS_LEVINFO_BASE_H
# define MASXFS_LEVINFO_BASE_H

# include "masxfs_levinfo_types.h"

masxfs_levinfo_t *masxfs_levinfo_create( void );
masxfs_levinfo_t *masxfs_levinfo_create_array( masxfs_depth_t sz );
masxfs_levinfo_t *masxfs_levinfo_create_array_setup( masxfs_depth_t sz );

void masxfs_levinfo_init( masxfs_levinfo_t * pi, masxfs_depth_t lidepth, const char *name, masxfs_entry_type_t d_type /*, ino_t d_inode */ ,
                          unsigned long long node_id, masxfs_stat_t * stat );
void masxfs_levinfo_n_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, size_t len,
                            masxfs_entry_type_t d_type /*, ino_t d_inode */ ,
                            unsigned long long node_id, masxfs_stat_t * stat );
void masxfs_levinfo_reset_lia( masxfs_levinfo_t * pi, masxfs_depth_t sz, masxfs_levinfo_flags_t flags );
void masxfs_levinfo_reset( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
void masxfs_levinfo_delete_lia( masxfs_levinfo_t * pi, masxfs_depth_t sz, masxfs_levinfo_flags_t flags );

#endif
