#ifndef MASXFS_LEVINFO_H
# define MASXFS_LEVINFO_H

# include "masxfs_types.h"

masxfs_levinfo_t *masxfs_levinfo_root( masxfs_levinfo_t * li );
masxfs_levinfo_t *masxfs_levinfo_li2lia( masxfs_levinfo_t * li );


int masxfs_levinfo_scandir( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive );
int masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive );

#endif
