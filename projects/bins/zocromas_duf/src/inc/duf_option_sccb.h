#ifndef MAS_DUF_OPTION_SCCB_H
#  define MAS_DUF_OPTION_SCCB_H


mas_error_code_t duf_option_$_list_sccbs( void );
mas_error_code_t duf_option_$_evaluate_sccb( const char *name );

mas_error_code_t duf_option_$_call_file( const char *name );

/* mas_error_code_t duf_option_$_list_sccb( int x ); */
mas_error_code_t duf_option_$_db_open( void );

mas_error_code_t duf_option_$_cd( const char *s );
mas_error_code_t duf_option_$_cdid(  long topdirid );

mas_error_code_t duf_option_$_set_dir_priority( long prio );
#endif
