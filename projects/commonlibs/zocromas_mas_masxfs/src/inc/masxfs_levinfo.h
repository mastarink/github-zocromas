#ifndef MASXFS_LEVINFO_H
# define MASXFS_LEVINFO_H

# include "masxfs_types.h"

masxfs_entry_type_t masxfs_levinfo_de2entry( int d_type );

masxfs_levinfo_t *masxfs_levinfo_root( masxfs_levinfo_t * li );
masxfs_levinfo_t *masxfs_levinfo_li2lia( masxfs_levinfo_t * li );

/* int masxfs_levinfo_scandir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, unsigned long flags ); */
int masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags, size_t maxdepth );
int masxfs_levinfo_scandirn_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags, size_t maxdepth );
int masxfs_levinfo_scanli_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags, size_t maxdepth );

char *masxfs_levinfo_prefix( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, int test );

#endif
