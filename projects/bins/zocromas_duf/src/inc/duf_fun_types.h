#ifndef MAS_DUF_FUN_TYPES_H
#  define MAS_DUF_FUN_TYPES_H

typedef void ( *duf_void_void_t ) ( void );
typedef void ( *duf_void_voidp_t ) ( void * );
typedef void ( *duf_void_int_t ) ( int );
typedef void ( *duf_void_argcv_t ) ( int, char *const * );
typedef int ( *duf_int_void_t ) ( void );
#endif
