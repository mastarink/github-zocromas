#ifndef MAS_DUF_SERVICE_H
#  define MAS_DUF_SERVICE_H

#  include "duf_levinfo_types.h"
#  include "duf_fileinfo_types.h"
#  include "duf_pdi_types.h"
#  include "duf_hook_types.h"
/* #  include "duf_format_types.h" */

#  define DUF_FUNN(af) duf_dbg_funname( ( duf_anyhook_t ) af )


int duf_check_field( const char *name, int have );
const char *duf_dbg_funname( duf_anyhook_t p );

#endif
