#ifndef MASXFS_PATHINFO_BASE_H
# define MASXFS_PATHINFO_BASE_H

# include "masxfs_types.h"

masxfs_pathinfo_t *masxfs_pathinfo_create( void );
masxfs_pathinfo_t *masxfs_pathinfo_create_setup( const char *real_path, size_t max_depth );

void masxfs_pathinfo_reset( masxfs_pathinfo_t * pi );
void masxfs_pathinfo_delete( masxfs_pathinfo_t * pi );

#endif
