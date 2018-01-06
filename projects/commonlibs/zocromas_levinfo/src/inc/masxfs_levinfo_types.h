#ifndef MASXFS_LEVINFO_TYPES_H
# define MASXFS_LEVINFO_TYPES_H

# include <dirent.h>

typedef struct stat masxfs_stat_t;
typedef struct dirent masxfs_dirent_t;
typedef DIR masxfs_dir_t;
typedef struct masxfs_handler_s masxfs_handler_t;

typedef enum masxfs_entry_type_e masxfs_entry_type_t;
typedef enum masxfs_entry_type_bit_e masxfs_entry_type_bit_t;

typedef signed int masxfs_depth_t;

typedef enum masxfs_cb_flag_e masxfs_cb_flag_t;
typedef enum masxfs_cb_flag_bit_e masxfs_cb_flag_bit_t;
typedef unsigned long masxfs_levinfo_flags_t;
typedef unsigned masxfs_type_flags_t;

typedef struct masxfs_entry_callback_s masxfs_entry_callback_t;
typedef struct masxfs_entry_filter_s masxfs_entry_filter_t;

typedef struct masxfs_xstat_s masxfs_xstat_t;
typedef struct masxfs_xstatc_s masxfs_xstatc_t;

typedef struct masxfs_levinfo_s masxfs_levinfo_t;
typedef struct masxfs_scanner_s masxfs_scanner_t;

typedef int ( *masxfs_scan_fun_simple_t ) ( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *userdata,
                                            unsigned long fun_counter, masxfs_depth_t reldepth );
typedef int ( *masxfs_scan_fun_stat_t ) ( const char *name, struct stat * st, void *userdata, unsigned depth,
                                          unsigned long fun_counter, const char *prefix, masxfs_depth_t reldepth );
// ?? typedef int ( *masxfs_li_scanner_t ) ( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *userdata, masxfs_levinfo_flags_t flags );

typedef int ( *masxfs_li_handler_t ) ( masxfs_levinfo_t * li );

/* typedef int ( *masxfs_li_cb_t ) ( const char *name, masxfs_depth_t depth, masxfs_levinfo_t *li, void *udata ); */

#endif
