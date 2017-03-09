#ifndef MASXFS_LEVINFO_SCAN_H
# define MASXFS_LEVINFO_SCAN_H

# include "masxfs_levinfo_types.h"


int masxfs_levinfo_scan_dir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, size_t maxdepth
                              /* , masxfs_scan_mode_t mode */  );
int masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, size_t maxdepth
                               /* , masxfs_scan_mode_t mode */  );
int masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, size_t maxdepth
                             /* , masxfs_scan_mode_t mode */  );
int masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth
                                /* , masxfs_scan_mode_t mode */  );

int masxfs_levinfo_scan_depth_cbf( masxfs_levinfo_t * lia, masxfs_depth_t depth, masxfs_scan_fun_simple_t cbf, void *udata _uUu_, masxfs_levinfo_flags_t flags
                             /* , masxfs_scan_mode_t mode */  );

#endif
