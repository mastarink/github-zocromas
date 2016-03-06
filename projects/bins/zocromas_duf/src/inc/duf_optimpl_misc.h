#ifndef MAS_DUF_OPTIMPL_MISC_H
#  define MAS_DUF_OPTIMPL_MISC_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR(SNIPPET_OPTION,optimpl_O_list_targ1, mas_argvc_t * targ, long n );
DR(SNIPPET_OPTION,optimpl_O_list_targ2, int *ptargc, char ***ptargv, long n );

DR(SNIPPET_OPTION,optimpl_O_clear_targ1, mas_argvc_t * targ, long n );
DR(SNIPPET_OPTION,optimpl_O_clear_targ2, int *ptargc, char ***ptargv, long n );

DR(SNIPPET_OPTION,optimpl_O_add_targ1, mas_argvc_t * targ, const char *s );
DR(SNIPPET_OPTION,optimpl_O_add_targ2, int *ptargc, char ***ptargv, const char *s );

DR(SNIPPET_OPTION,optimpl_O_echo, const char *arg );

DR(SNIPPET_OPTION,optimpl_O_history, void );

DR(SNIPPET_OPTION,optimpl_O_error_level, long lev );


#endif
