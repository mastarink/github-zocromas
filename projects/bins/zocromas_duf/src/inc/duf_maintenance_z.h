#ifndef MAS_DUF_MAINTENANCE_Z_H
# define MAS_DUF_MAINTENANCE_Z_H

# include <assert.h>

# include <mastar/wrap/mas_std_def.h>
# include <mastar/wrap/mas_memory.h>

# include "duf_compile_defs.h"
/* #  include "duf_start_end.h" */
# include "duf_se.h"
# include "duf_dodefs.h"

extern int duf_output_level( void ) __attribute__ ( ( weak ) );
extern FILE *duf_output_file( void ) __attribute__ ( ( weak ) );

extern int duf_verbose( void ) __attribute__ ( ( weak ) );
extern int duf_dry_run( void ) __attribute__ ( ( weak ) );

#endif
