#ifndef MAS_DUF_OPTIONS_FILE_H
#  define MAS_DUF_OPTIONS_FILE_H


int duf_infile_options_at_file( duf_option_stage_t istage, const char *filename, int optional );
int duf_incfg_options( duf_option_stage_t istage );

int duf_incfgf_options( duf_option_stage_t istage, const char *bfilename, int optional );
int duf_incfg_last_options( duf_option_stage_t istage );
int duf_incfg_stg_options( duf_option_stage_t istage );

int duf_infile_options_at_stream( duf_option_stage_t istage, FILE * f, duf_option_source_t source );
int duf_stdin_options( duf_option_stage_t istage );

int duf_infile_options_at_filepath( duf_option_stage_t istage, const char *filepath );

int duf_indirect_options( duf_option_stage_t istage );

#endif
