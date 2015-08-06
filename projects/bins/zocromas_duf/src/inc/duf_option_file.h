#ifndef MAS_DUF_OPTION_FILE_H
#  define MAS_DUF_OPTION_FILE_H


int duf_infile_options_at_file( duf_option_stage_t istage, const char *filename );
int duf_infile_options( duf_option_stage_t istage );

int duf_infile_options_at_stream( duf_option_stage_t istage, FILE * f );
int duf_stdin_options( duf_option_stage_t istage );


#endif
