#ifndef MASXFS_LEVINFO_BASE_H
# define MASXFS_LEVINFO_BASE_H

# include "masxfs_types.h"

masxfs_levinfo_t *masxfs_levinfo_create( void );
masxfs_levinfo_t *masxfs_levinfo_create_array( size_t sz );

void masxfs_levinfo_init( masxfs_levinfo_t * pi, const char *name , const char *path);
void masxfs_levinfo_close_array( masxfs_levinfo_t * pi, size_t sz );
void masxfs_levinfo_delete_array( masxfs_levinfo_t * pi, size_t sz );

#endif
