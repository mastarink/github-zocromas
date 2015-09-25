#ifndef MAS_DUF_FUN_TYPES_H
#  define MAS_DUF_FUN_TYPES_H

typedef void ( *duf_void_void_t ) ( void );
typedef void ( *duf_void_voidp_t ) ( void * );
typedef void ( *duf_void_int_t ) ( int );
typedef void ( *duf_void_argcv_t ) ( int, char *const * );
typedef void ( *duf_void_arg_t ) ( duf_cargvc_t );
typedef void ( *duf_void_pargcv_t ) ( int *, char *** );
typedef void ( *duf_void_pargcnv_t ) ( int *, char ***, long );
typedef void ( *duf_void_pnv_t ) ( long );
typedef void ( *duf_void_psv_t ) ( const char * );
typedef void ( *duf_void_pargcsv_t ) ( int *, char ***, const char * );
typedef int ( *duf_int_void_t ) ( void );

#endif

/*
vi: ft=c
*/
