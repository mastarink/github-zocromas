#ifndef MAS_DUF_SERVICE_H
#  define MAS_DUF_SERVICE_H

#  include "duf_levinfo_types.h"
#  include "duf_fileinfo_types.h"
#  include "duf_pdi_types.h"
#  include "duf_hook_types.h"
#  include "duf_format_types.h"


int duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format, duf_pdi_cb_t prefix_cb,
                         duf_pdi_cb_t suffix_cb );
int duf_check_field( const char *name, int have );
const char *duf_dbg_funname( duf_anyhook_t p );

#endif
