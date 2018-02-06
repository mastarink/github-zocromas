#ifndef MASXFS_LEVINFO_DB_DIR_H
# define MASXFS_LEVINFO_DB_DIR_H

int masxfs_levinfo_db_closedir( masxfs_levinfo_t * li );
int masxfs_levinfo_db_opendir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter );
int masxfs_levinfo_db_opened_dir( const masxfs_levinfo_t * li );
int masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li );
int masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, int *phas_data );

masxfs_levinfo_t *masxfs_levinfo_nameid2lia( unsigned long long nameid );

#endif
