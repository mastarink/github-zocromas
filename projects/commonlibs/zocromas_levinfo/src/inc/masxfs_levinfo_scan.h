#ifndef MASXFS_LEVINFO_SCAN_H
# define MASXFS_LEVINFO_SCAN_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_scanf_tree_cbs( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cbs, void *userdata,
                                   masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scanf_dirn_cbs( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cbs, void *userdata,
                                   masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scanf_down_cbs( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cbs, void *userdata,
                                   masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

#endif
