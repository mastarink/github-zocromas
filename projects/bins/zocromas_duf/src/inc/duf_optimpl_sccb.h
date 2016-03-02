#ifndef MAS_DUF_OPTIMPL_SCCB_H
# define MAS_DUF_OPTIMPL_SCCB_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

mas_error_code_t duf_optimpl_O_list_sccbs( void );
mas_error_code_t duf_optimpl_O_evaluate_sccb( const char *name );

/* mas_error_index_t duf_optimpl_O_call_file( const char *name ); */
DR( OPTION_SNIPPET, optimpl_O_call_file, const char *name );

/* mas_error_code_t duf_optimpl_O_list_sccb( int x ); */
mas_error_code_t duf_optimpl_O_db_open( void );

mas_error_code_t duf_optimpl_O_cd( const char *s );
mas_error_code_t duf_optimpl_O_cdid( long topdirid );

mas_error_code_t duf_optimpl_O_set_dir_priority( long prio );
#endif
