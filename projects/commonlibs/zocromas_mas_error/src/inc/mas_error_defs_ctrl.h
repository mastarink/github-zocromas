#ifndef MAS_ERROR_DEFS_CTRL_H
#  define MAS_ERROR_DEFS_CTRL_H


/* #  define MASE_E1_LOWER(...) mas_enable_mereport_c(1, -1, 0, __VA_ARGS__, 0) */

#  define MASE_E_LOWER(...)  mas_enable_mereport_c(0, -1, 0, __VA_ARGS__, 0)
#  define MASE_E_LOWER_N(_n, ...)  mas_enable_mereport_c(0, -_n, 0, __VA_ARGS__, 0)
#  define MASE_E_UPPER(...) mas_enable_mereport_c(0,  1, 0, __VA_ARGS__, 0)
#  define MASE_E_UPPER_N(_n, ...) mas_enable_mereport_c(0, _n, 0, __VA_ARGS__, 0)

#  define MASE_E_SET(_n, ...) mas_enable_mereport_c(0, _n, 1, __VA_ARGS__, 0)

#  define MASE_E_MAX(cnt, ...) mas_set_memax_count_c(cnt, __VA_ARGS__, 0)

#  define MASE_EREPORT(rc) (mas_enabled_ereport_i(rc)>0)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * -*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#  define MASE_CLEAR_ERROR(_rt, ...)    (_rt=mas_clear_error_i( (_rt), __VA_ARGS__, 0 ))
#  define MASE_CLEARED_ERROR(_rt, ...)  (0==(_rt=mas_clear_error_i( (_rt), __VA_ARGS__, 0 )))
#  define MASE_IS_ERROR_N(_rt, _er) ( mas_error_code_i( _rt ) == _er )
#  define MASE_IS_ERROR(_rt) ( _rt < 0 )
#  define MASE_NOERROR(_rt) ( _rt >= 0 )

#endif

/*
vi: ft=c
*/


