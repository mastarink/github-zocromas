#ifndef MAS_OPTS_COMMON_H
#  define MAS_OPTS_COMMON_H

typedef int ( *mas_new_section_func_t ) ( mas_options_t * popts, const char *section );
typedef int ( *mas_at_section_func_t ) ( mas_options_t * popts, const char *section, const char *s );
typedef int ( *mas_unknown_opt_func_t ) ( mas_options_t * popts, const char *s );


int mas_opts_restore_nosection( mas_options_t * popts, void *ptopts, const char *section, const char *sectvalue, const char *s,
                                mas_option_parse_t * opt_table, size_t opt_table_size, const void *arg );
int _mas_opts_restore_path( mas_options_t * popts, const char *fpath, void *ptopts, mas_option_parse_t * opt_table, size_t opt_table_size,
                            const void *arg, mas_new_section_func_t ns_func, mas_at_section_func_t ats_func,
                            mas_unknown_opt_func_t uo_func );
int _mas_opts_restore_relative( mas_options_t * popts, const char *filename, void *ptopts, mas_option_parse_t * opt_table,
                                size_t opt_table_size, const void *arg, mas_new_section_func_t new_section_func,
                                mas_at_section_func_t at_section_func, mas_unknown_opt_func_t unknown_opt_func );

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
