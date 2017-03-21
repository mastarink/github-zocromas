#ifndef MASXFS_LEVINFO_DB_H
# define MASXFS_LEVINFO_DB_H

int masxfs_levinfo_db_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

int masxfs_levinfo_db_closedir( masxfs_levinfo_t * li );

int masxfs_levinfo_db_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

int masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_db_close( masxfs_levinfo_t * li );

int masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int *phas_data );

int masxfs_levinfo_db_open_at( masxfs_levinfo_t * li, int fdparent );

#endif
