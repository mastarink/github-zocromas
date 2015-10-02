#ifndef MAS_DUF_OPTION_FS_H
#  define MAS_DUF_OPTION_FS_H


duf_error_code_t duf_option_$_fs_ls( const char *arg );
duf_error_code_t duf_option_$_fs_rm( const char *arg, long v  );
duf_error_code_t duf_option_$_fs_cp( const char *arg, long v  );
duf_error_code_t duf_option_$_fs_mv( const char *arg, long v  );

#endif
