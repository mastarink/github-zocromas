#ifndef MASXFS_SCAN_H
# define MASXFS_SCAN_H

# include "masxfs_types.h"

int masxfs_scanpath_real( const char *path, masxfs_entry_callback_t * callbacks, unsigned long flags, int multicb );
int masxfs_scanpath_real2( const char *cpath, masxfs_entry_callback_t * callbacks, unsigned long flags, int multicb );

#endif
