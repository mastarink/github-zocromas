#ifndef MAS_DUF_OPTION_RESTORE_H
#  define MAS_DUF_OPTION_RESTORE_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"


char *duf_restore_some_options( const char *a0 );
void duf_restore_some_option( char *ptr, duf_option_code_t codeval, size_t maxlen );


#endif
