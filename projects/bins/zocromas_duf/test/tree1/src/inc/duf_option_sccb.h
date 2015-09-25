#ifndef MAS_DUF_OPTION_SCCB_H
#  define MAS_DUF_OPTION_SCCB_H


void duf_option_$_list_sccbs( void );
void duf_option_$_evaluate_sccb( const char *name );

void duf_option_$_call_file( const char *name );

void duf_option_$_list_sccb( int x );
void duf_option_$_db_open( void );

void duf_option_$_cd( const char *s );
#endif
