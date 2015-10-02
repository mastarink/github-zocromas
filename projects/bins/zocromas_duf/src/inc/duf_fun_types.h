#ifndef MAS_DUF_FUN_TYPES_H
#  define MAS_DUF_FUN_TYPES_H

typedef void ( *duf_void_void_func_t ) ( void );
typedef void ( *duf_void_voidp_func_t ) ( void * );
typedef void ( *duf_void_int_func_t ) ( int );
typedef void ( *duf_void_argcv_func_t ) ( int, char *const * );
typedef void ( *duf_void_arg_func_t ) ( duf_cargvc_t );
typedef void ( *duf_void_pargcv_func_t ) ( int *, char *** );
typedef void ( *duf_void_pargcnv_func_t ) ( int *, char ***, long );
typedef void ( *duf_void_pnv_func_t ) ( long );
typedef void ( *duf_void_psv_func_t ) ( const char * );
typedef void ( *duf_void_psnv_func_t ) ( const char *, long );
typedef void ( *duf_void_ps2v_func_t ) ( const char *, const char * );
typedef void ( *duf_void_pargcsv_func_t ) ( int *, char ***, const char * );

#include "duf_error_types.h"

typedef duf_error_code_t ( *duf_errc_void_func_t ) ( void );
typedef duf_error_code_t ( *duf_errc_voidp_func_t ) ( void * );
typedef duf_error_code_t ( *duf_errc_int_func_t ) ( int );
typedef duf_error_code_t ( *duf_errc_argcv_func_t ) ( int, char *const * );
typedef duf_error_code_t ( *duf_errc_arg_func_t ) ( duf_cargvc_t );
typedef duf_error_code_t ( *duf_errc_pargcv_func_t ) ( int *, char *** );
typedef duf_error_code_t ( *duf_errc_pargcnv_func_t ) ( int *, char ***, long );
typedef duf_error_code_t ( *duf_errc_pnv_func_t ) ( long );
typedef duf_error_code_t ( *duf_errc_psv_func_t ) ( const char * );
typedef duf_error_code_t ( *duf_errc_psvv_func_t ) ( const char *, const void *pv );
typedef duf_error_code_t ( *duf_errc_psnv_func_t ) ( const char *, long );
typedef duf_error_code_t ( *duf_errc_ps2v_func_t ) ( const char *, const char * );
typedef duf_error_code_t ( *duf_errc_ps2vv_func_t ) ( const char *, const char *, const void *pv );
typedef duf_error_code_t ( *duf_errc_pargcsv_func_t ) ( int *, char ***, const char * );



typedef int ( *duf_int_void_func_t ) ( void );

#endif

/*
vi: ft=c
*/
