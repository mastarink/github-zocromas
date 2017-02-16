#ifndef MASXFS_SCAN_H
# define MASXFS_SCAN_H

# include "masxfs_types.h"

char *masxfs_get_normalized_path( const char *path, const char *name, char **pnpath );
char *masxfs_normalize_path( const char *path, const char *name );

int masxfs_scanentry_cb( masxfs_dirent_t * de, const char *path, masxfs_entry_callback_t * cb, int recursive );
int masxfs_scanentry( masxfs_dirent_t * de, const char *path, masxfs_entry_callback_t * callbacks, int recursive );

int masxfs_scandir_cb( DIR * dir, const char *path, masxfs_entry_callback_t * cb, int recursive );
int masxfs_scandir( DIR * dir, const char *path, masxfs_entry_callback_t * callbacks, int recursive );
int masxfs_scandir_r( DIR * dir, const char *path, masxfs_entry_callback_t * callbacks, int recursive );

int masxfs_scanpath_cb( const char *path, masxfs_entry_callback_t * callbacks, int recursive );
int masxfs_scanpath( const char *path, masxfs_entry_callback_t * callbacks, int recursive );
int masxfs_scanpath_real( const char *path, masxfs_entry_callback_t * callbacks, int recursive );

#endif
