#ifndef MAS_DUF_ERROR_DEFS_CTRL_H
#  define MAS_DUF_ERROR_DEFS_CTRL_H


/* #  define DUF_E1_LOWER(...) duf_enable_mereport_c(1, -1, 0, __VA_ARGS__, 0) */

#  define DUF_E_LOWER(...)  duf_enable_mereport_c(0, -1, 0, __VA_ARGS__, 0)
#  define DUF_E_LOWER_N(_n, ...)  duf_enable_mereport_c(0, -_n, 0, __VA_ARGS__, 0)
#  define DUF_E_UPPER(...) duf_enable_mereport_c(0,  1, 0, __VA_ARGS__, 0)
#  define DUF_E_UPPER_N(_n, ...) duf_enable_mereport_c(0, _n, 0, __VA_ARGS__, 0)

#  define DUF_E_SET(_n, ...) duf_enable_mereport_c(0, _n, 1, __VA_ARGS__, 0)

#  define DUF_E_MAX(cnt, ...) duf_set_memax_count_c(cnt, __VA_ARGS__, 0)

#  define DUF_EREPORT(rc) (duf_enabled_ereport_i(rc)>0)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * -*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#  define DUF_CLEAR_ERROR(_rt, ...)    (_rt=duf_clear_error_i( (_rt), __VA_ARGS__, 0 ))
#  define DUF_CLEARED_ERROR(_rt, ...)  (0==(_rt=duf_clear_error_i( (_rt), __VA_ARGS__, 0 )))
#  define DUF_IS_ERROR_N(_rt, _er) ( duf_error_code_i( _rt ) == _er )
#  define DUF_IS_ERROR(_rt) ( _rt < 0 )
#  define DUF_NOERROR(_rt) ( _rt >= 0 )

#endif

/*
vi: ft=c
*/


