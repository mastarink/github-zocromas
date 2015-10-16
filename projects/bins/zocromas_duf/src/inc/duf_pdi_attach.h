#ifndef MAS_DUF_PDI_ATTACH_H
#  define MAS_DUF_PDI_ATTACH_H

#  include "sql_tables_defs.h"

#  ifdef DUF_ATTACH_SELECTED_PATTERN
#    ifdef DUF_SQL_SELECTED_TEMPORARY
#      error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#    endif

int duf_pdi_attach_selected( duf_depthinfo_t * pdi );
int duf_pdi_detach_selected( duf_depthinfo_t * pdi );
#  else
#    ifndef DUF_SQL_SELECTED_TEMPORARY
#      error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#    endif
#  endif

#endif
