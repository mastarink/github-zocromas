#ifndef MASXFS_LEVINFO_SCAN_H
# define MASXFS_LEVINFO_SCAN_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_scan_dir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth,
                                masxfs_scan_mode_t mode );
int masxfs_levinfo_scan_dirn_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth,
                                 masxfs_scan_mode_t mode );
int masxfs_levinfo_scan_li_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, size_t maxdepth,
                               masxfs_scan_mode_t mode );
int masxfs_levinfo_scan_entry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth,
                                  masxfs_scan_mode_t mode );

int masxfs_levinfo_scan_depth( masxfs_levinfo_t * lia, masxfs_depth_t depth, masxfs_scan_fun_simple_t cb, void *udata _uUu_, unsigned long flags,
                               masxfs_scan_mode_t mode );

#endif
