#ifndef MASXFS_LEVINFO_IO_H
# define MASXFS_LEVINFO_IO_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_close( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

int masxfs_levinfo_close_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

int masxfs_levinfo_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, masxfs_entry_filter_t * entry_pfilter, masxfs_stat_t ** pstat );

#endif
