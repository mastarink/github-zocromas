#ifndef MAS_DUF_OPTIMPL_FS_H
# define MAS_DUF_OPTIMPL_FS_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */

mas_error_code_t duf_option_O_fs_ls( const char *arg );
mas_error_code_t duf_option_O_fs_rm( const char *arg, long v );
mas_error_code_t duf_option_O_fs_cp( const char *arg, long v );
mas_error_code_t duf_option_O_fs_mv( const char *arg, long v );

#endif
