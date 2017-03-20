#ifndef MASXFS_PATHINFO_BASE_H
# define MASXFS_PATHINFO_BASE_H

# include <mastar/levinfo/masxfs_levinfo_types.h>
# include "masxfs_types.h"

void masxfs_pathinfo_set_flags( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags );
void masxfs_pathinfo_clear_flags( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags );

void masxfs_pathinfo_init( masxfs_pathinfo_t * pi, const char *path, masxfs_depth_t depth_limit, masxfs_levinfo_flags_t flags );

masxfs_pathinfo_t *masxfs_pathinfo_create( void );
masxfs_pathinfo_t *masxfs_pathinfo_create_setup( const char *real_path, masxfs_depth_t depth_limit, masxfs_levinfo_flags_t flags );

void masxfs_pathinfo_reset( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags );
void masxfs_pathinfo_delete( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags );

#endif
