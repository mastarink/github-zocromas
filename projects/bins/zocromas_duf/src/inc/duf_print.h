#ifndef MAS_DUF_PRINT_H
#  define MAS_DUF_PRINT_H

#  include "duf_levinfo_types.h"
#  include "duf_fileinfo_types.h"
#  include "duf_pdi_types.h"
#  include "duf_hook_types.h"
#  include "duf_format_types.h"



int duf_print_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb );

#endif
