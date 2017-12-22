#ifndef MASXFS_LEVINFO_SCAN_H
# define MASXFS_LEVINFO_SCAN_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_scanf_tree_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, void *userdata, masxfs_depth_t reldepth );

int masxfs_levinfo_scanf_dirn_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                       masxfs_depth_t reldepth );

#endif
