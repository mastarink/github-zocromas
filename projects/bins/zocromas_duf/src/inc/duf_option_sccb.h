#ifndef MAS_DUF_OPTION_SCCB_H
#  define MAS_DUF_OPTION_SCCB_H


mas_error_code_t duf_option_O_list_sccbs( void );
mas_error_code_t duf_option_O_evaluate_sccb( const char *name );

mas_error_code_t duf_option_O_call_file( const char *name );

/* mas_error_code_t duf_option_O_list_sccb( int x ); */
mas_error_code_t duf_option_O_db_open( void );

mas_error_code_t duf_option_O_cd( const char *s );
mas_error_code_t duf_option_O_cdid(  long topdirid );

mas_error_code_t duf_option_O_set_dir_priority( long prio );
#endif
