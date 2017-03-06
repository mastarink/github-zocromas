#ifndef MASXFS_LEVINFO_TYPES_H
# define MASXFS_LEVINFO_TYPES_H

# include <dirent.h>

typedef struct stat masxfs_stat_t;
typedef struct dirent masxfs_dirent_t;
typedef DIR masxfs_dir_t;

typedef enum masxfs_entry_type_e masxfs_entry_type_t;
typedef enum masxfs_entry_type_bit_e masxfs_entry_type_bit_t;

typedef enum masxfs_cb_flag_e masxfs_cb_flag_t;
typedef enum masxfs_cb_flag_bit_e masxfs_cb_flag_bit_t;
typedef struct masxfs_entry_callback_s masxfs_entry_callback_t;

typedef struct masxfs_levinfo_s masxfs_levinfo_t;

typedef int ( *masxfs_scan_fun_simple_t ) ( masxfs_levinfo_t * li, unsigned long flags, void *data );
typedef int ( *masxfs_li_scanner_t ) ( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags );
typedef int ( *masxfs_li_filter_t ) ( masxfs_levinfo_t * li );
typedef int ( *masxfs_li_stopper_t ) ( masxfs_levinfo_t * li );

/* typedef int ( *masxfs_li_cb_t ) ( const char *name, size_t depth, masxfs_levinfo_t *li, void *udata ); */

typedef size_t masxfs_depth_t;

#endif
