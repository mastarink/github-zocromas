#ifndef MASXFS_LEVINFO_DB_H
# define MASXFS_LEVINFO_DB_H

int masxfs_levinfo_db_open( masxfs_levinfo_t * li );

int masxfs_levinfo_db_stat( masxfs_levinfo_t * li );
int masxfs_levinfo_db_close( masxfs_levinfo_t * li );

int masxfs_levinfo_db_open_at( masxfs_levinfo_t * li, int fdparent );

unsigned long long masxfs_levinfo_db_store( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

#endif
