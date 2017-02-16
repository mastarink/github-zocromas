#ifndef MASXFS_TYPES_H
# define MASXFS_TYPES_H

# include <dirent.h>

typedef struct stat masxfs_stat_t;
typedef struct dirent masxfs_dirent_t;

typedef enum masxfs_entry_type_e masxfs_entry_type_t;
typedef enum masxfs_entry_type_bit_e masxfs_entry_type_bit_t;
typedef struct masxfs_entry_callback_s masxfs_entry_callback_t;

typedef struct masxfs_pathinfo_s masxfs_pathinfo_t;
typedef struct masxfs_levinfo_s masxfs_levinfo_t;

typedef int ( *masxfs_scan_fun_simple_t ) ( const char *path, const char *name, char **pnpath );
#endif
