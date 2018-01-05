#ifndef MASXFS_PATHINFO_H
# define MASXFS_PATHINFO_H

# include "masxfs_types.h"

/* int masxfs_pathinfo_open( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags ); */

char *masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi );
int masxfs_pathinfo_scan_cbs( masxfs_pathinfo_t * pi, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * callbacks, void *udata,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth );
int masxfs_pathinfo_scanf_cbs( masxfs_pathinfo_t * pi, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cbs, void *userdata,
                               masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth );
int masxfs_pathinfo_scanf_scanner( masxfs_pathinfo_t * pi, masxfs_scanner_t * scanner, void *userdata );

masxfs_levinfo_t *masxfs_pathinfo_tail( masxfs_pathinfo_t * pi, masxfs_depth_t offset );
masxfs_levinfo_t *masxfs_pathinfo_last_li( masxfs_pathinfo_t * pi );

/* int masxfs_pathinfo_scan_depth_cbf( masxfs_pathinfo_t * pi, masxfs_scan_fun_simple_t cb, void *udata, masxfs_levinfo_flags_t flags ); */

#endif
