#ifndef MAS_DUF_OPTIMPL_SCCB_H
# define MAS_DUF_OPTIMPL_SCCB_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ▤ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SNIPPET_OPTION, optimpl_O_list_sccbs, void );
DR( SNIPPET_OPTION, optimpl_O_evaluate_sccb, const char *name );

/* mas_error_index_t duf_optimpl_O_call_file( const char *name ); */
DR( SNIPPET_OPTION, optimpl_O_call_file, const char *name );

/* DR(SNIPPET_OPTION,optimpl_O_list_sccb, int x ); */
DR( SNIPPET_OPTION, optimpl_O_db_open, void );

DR( SNIPPET_OPTION, optimpl_O_cd, const char *s );
DR( SNIPPET_OPTION, optimpl_O_cdid, long topdirid );

DR( SNIPPET_OPTION, optimpl_O_set_dir_priority, long prio );
#endif
