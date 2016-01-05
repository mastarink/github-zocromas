#ifndef MAS_DUF_OPTIONS_FILE_H
#  define MAS_DUF_OPTIONS_FILE_H

#  include <stdio.h>

int duf_source_stdin_options( duf_option_stage_t istage );
int duf_source_indirect_options( duf_option_stage_t istage );
int duf_source_incfg_last_options( duf_option_stage_t istage );
int duf_source_incfg_stg_options( duf_option_stage_t istage );
int duf_source_incfg_options( duf_option_stage_t istage );


const char *duf_options_infilepath( void );
int duf_infile_options_at_filepath( duf_option_stage_t istage, const char *filepath );


#endif
