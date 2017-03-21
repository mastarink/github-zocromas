#ifndef MASXFS_LEVINFO_SCAN_H
# define MASXFS_LEVINFO_SCAN_H

# include "masxfs_levinfo_types.h"

int
masxfs_levinfo_scan_tree_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                  masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                   masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scan_down_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                  masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

int masxfs_levinfo_scan_entry_single_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                          masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth );

int masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth );

#endif
