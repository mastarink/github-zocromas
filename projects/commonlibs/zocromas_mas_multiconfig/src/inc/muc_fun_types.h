#ifndef MAS_MUC_FUN_TYPES_H
# define MAS_MUC_FUN_TYPES_H

# include <mastar/tools/mas_argvc_types.h>

typedef void ( *muc_void_void_func_t ) ( void );
typedef void ( *muc_void_voidp_func_t ) ( void * );
typedef void ( *muc_void_int_func_t ) ( int );
typedef void ( *muc_void_cargcv_func_t ) ( int, char *const * );
typedef void ( *muc_void_carg_func_t ) ( mas_cargvc_t );
typedef void ( *muc_void_pargcv_func_t ) ( int *, char *** );
typedef void ( *muc_void_pargcvn_func_t ) ( int *, char ***, long );
typedef void ( *muc_void_n_func_t ) ( long );
typedef void ( *muc_void_csv_func_t ) ( const char * );
typedef void ( *muc_void_csn_func_t ) ( const char *, long );
typedef void ( *muc_void_cscs_func_t ) ( const char *, const char * );
typedef void ( *muc_void_pcargcvcs_func_t ) ( int *, char ***, const char * );
typedef void ( *muc_void_cargvs_func_t ) ( mas_argvc_t *, const char * );

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t */

typedef mas_error_code_t( *muc_errc_void_func_t ) ( void );
typedef mas_error_code_t( *muc_errc_voidp_func_t ) ( void * );
typedef mas_error_code_t( *muc_errc_int_func_t ) ( int );
typedef mas_error_code_t( *muc_errc_cargcv_func_t ) ( int, char *const * );
typedef mas_error_code_t( *muc_errc_cargv_func_t ) ( mas_cargvc_t * );
typedef mas_error_code_t( *muc_errc_pargcv_func_t ) ( int *, char *** );
typedef mas_error_code_t( *muc_errc_pargcvn_func_t ) ( int *, char ***, long );
typedef mas_error_code_t( *muc_errc_argvn_func_t ) ( mas_argvc_t *, long ); /* argvn = argvc, numeric */
typedef mas_error_code_t( *muc_errc_n_func_t ) ( long );             /* n = numeric */
typedef mas_error_code_t( *muc_errc_cs_func_t ) ( const char * );    /* cs = const string */
typedef mas_error_code_t( *muc_errc_csv_func_t ) ( const char * );   /* csv = const string, void */
typedef mas_error_code_t( *muc_errc_psvv_func_t ) ( const char *, const void *pv );
typedef mas_error_code_t( *muc_errc_csn_func_t ) ( const char *, long ); /* csn = const string, numeric */
typedef mas_error_code_t( *muc_errc_cscs_func_t ) ( const char *, const char * ); /* cscs = const string, const string */
typedef mas_error_code_t( *muc_errc_cscsv_func_t ) ( const char *, const char *, const void *pv );
typedef mas_error_code_t( *muc_errc_pcargcvcs_func_t ) ( int *, char ***, const char * );
typedef mas_error_code_t( *muc_errc_cargvs_func_t ) ( mas_argvc_t *, const char * );

typedef int ( *muc_int_cs_func_t ) ( const char * );                 /* cs = const string */

typedef int ( *muc_int_void_func_t ) ( void );
typedef void *( *muc_pvoid_void_func_t ) ( void );
typedef char *( *muc_pchar_void_func_t ) ( void );
typedef const char *( *muc_cpchar_void_func_t ) ( void );

#endif

/*
vi: ft=c
*/
