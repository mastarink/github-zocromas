#ifndef MAS_DUF_SCCBH_H
# define MAS_DUF_SCCBH_H

# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */
# include "duf_format_types.h"

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DRX( SCCBH, const char *, sformat, NULL, get_item_format, const duf_sccb_handle_t * sccbh, duf_format_name_ids_t id, int dirs,
     int *puse_format_once );

#endif
