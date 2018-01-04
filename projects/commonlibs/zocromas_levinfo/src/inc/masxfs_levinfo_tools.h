#ifndef MASXFS_LEVINFO_TOOLS_H
# define MASXFS_LEVINFO_TOOLS_H

# include "masxfs_levinfo_types.h"

const char *masxfs_levinfo_detype_name( masxfs_levinfo_t * li );
masxfs_entry_type_t masxfs_levinfo_s2detype( const char *sdetype );
const char *masxfs_levinfo_detype2s( masxfs_entry_type_t detype );

masxfs_entry_type_t masxfs_levinfo_de2entry( unsigned char d_type );
unsigned char masxfs_levinfo_entry2de( masxfs_entry_type_t detype );

masxfs_entry_type_t masxfs_levinfo_stat2entry( masxfs_stat_t * stat );
masxfs_entry_type_t masxfs_levinfo_statmode2entry( mode_t m );

int masxfs_levinfo_name_valid( const char *name, masxfs_entry_type_t detype, masxfs_entry_filter_t * entry_pfilter );
int masxfs_levinfo_stat_valid( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_xstat_valid( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_levinfo_flags_t flags );

#endif
