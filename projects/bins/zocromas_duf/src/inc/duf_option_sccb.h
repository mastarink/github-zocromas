#ifndef MAS_DUF_OPTION_SCCB_H
#  define MAS_DUF_OPTION_SCCB_H


duf_error_code_t duf_option_$_list_sccbs( void );
duf_error_code_t duf_option_$_evaluate_sccb( const char *name );

duf_error_code_t duf_option_$_call_file( const char *name );

duf_error_code_t duf_option_$_list_sccb( int x );
duf_error_code_t duf_option_$_db_open( void );

duf_error_code_t duf_option_$_cd( const char *s );

duf_error_code_t duf_option_$_set_dir_priority( long prio );
#endif
