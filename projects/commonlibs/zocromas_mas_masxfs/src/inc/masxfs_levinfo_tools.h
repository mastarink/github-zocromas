#ifndef MASXFS_LEVINFO_TOOLS_H
# define MASXFS_LEVINFO_TOOLS_H

# include "masxfs_types.h"

const char *masxfs_levinfo_detype_name( masxfs_levinfo_t * li );
masxfs_entry_type_t masxfs_levinfo_de2entry( int d_type );
masxfs_entry_type_t masxfs_levinfo_stat2entry( masxfs_stat_t * stat );

#endif
