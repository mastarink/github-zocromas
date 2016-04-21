#ifndef MAS_DUF_CONFIG_OUTPUT_UTIL_H
# define MAS_DUF_CONFIG_OUTPUT_UTIL_H

/* int mas_verbose( void ); */
/* int mas_dry_run( void ); */

/* int mas_output_level( void ); */
/* FILE *mas_output_file( void ); */

/* int mas_output_force_color( void ); */
/* int mas_output_nocolor( void ); */
int duf_output_progress( void );

/* int duf_output_use_binformat( void ); */

const duf_sformats_t *mas_get_config_output_sformat( void );
const char *mas_get_config_output_sformat_list( void );
const char *mas_get_config_output_sformat_tree( void );
const char *mas_get_config_output_sformat_gen( void );

const duf_asformats_t *mas_get_config_output_asformats( void );
const duf_filedirformat_t *mas_get_config_output_asformat_list( void );
const duf_filedirformat_t *mas_get_config_output_asformat_tree( void );
const duf_filedirformat_t *mas_get_config_output_asformat_gen( void );

unsigned mas_get_config_output_max_width( void );

#endif
