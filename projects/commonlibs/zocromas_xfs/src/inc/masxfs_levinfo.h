#ifndef MASXFS_LEVINFO_H
# define MASXFS_LEVINFO_H

# include "masxfs_types.h"


masxfs_levinfo_t *masxfs_levinfo_root( masxfs_levinfo_t * li );
masxfs_levinfo_t *masxfs_levinfo_li2lia( masxfs_levinfo_t * li );

/* int masxfs_levinfo_scandir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, unsigned long flags ); */
int masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth );
int masxfs_levinfo_scandirn_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth );
int masxfs_levinfo_scanli_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth );

#endif
