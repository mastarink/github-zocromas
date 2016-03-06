#ifndef MAS_DUF_UFILTER_BIND_H
# define MAS_DUF_UFILTER_BIND_H

# include <mastar/tools/mas_argvc_types.h>

# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( OTHER, bind_ufilter_uni, duf_stmnt_t * pstmt, const duf_ufilter_t * pu, const duf_yfilter_t * py, const mas_argvc_t * ttarg_unused,
                                 const void *ptr );


#endif
