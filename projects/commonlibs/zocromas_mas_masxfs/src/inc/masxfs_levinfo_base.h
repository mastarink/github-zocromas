#ifndef MASXFS_LEVINFO_BASE_H
# define MASXFS_LEVINFO_BASE_H

# include "masxfs_types.h"

masxfs_levinfo_t *masxfs_levinfo_create( void );
masxfs_levinfo_t *masxfs_levinfo_create_array( size_t sz );
masxfs_levinfo_t *masxfs_levinfo_create_array_setup( size_t sz );

void masxfs_levinfo_init( masxfs_levinfo_t * pi, const char *name, masxfs_entry_type_t d_type );
void masxfs_levinfo_n_init( masxfs_levinfo_t * li, const char *name, size_t len, masxfs_entry_type_t d_type );
void masxfs_levinfo_reset_lia( masxfs_levinfo_t * pi, size_t sz );
void masxfs_levinfo_reset( masxfs_levinfo_t * li );
void masxfs_levinfo_delete_lia( masxfs_levinfo_t * pi, size_t sz );

#endif
