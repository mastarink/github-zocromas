#ifndef MAS_OPTS_COMMON_H
#  define MAS_OPTS_COMMON_H


int mas_opts_restore_nosection( void *popts, const char *s, mas_option_parse_t * opt_table, size_t opt_table_size );
int _mas_opts_restore_path( const char *fpath, void *popts, mas_option_parse_t * opt_table, size_t opt_table_size,
                            mas_new_section_func_t ns_func, mas_at_section_func_t ats_func, mas_unknown_opt_func_t uo_func );

#endif
