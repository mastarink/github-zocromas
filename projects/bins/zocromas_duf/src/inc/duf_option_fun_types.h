#ifndef MAS_DUF_OPTION_FUN_TYPES_H
# define MAS_DUF_OPTION_FUN_TYPES_H

# include "duf_fun_types.h"                                          /* _func_t */

/********************************************************************************************/
typedef struct
{
  duf_void_int_func_t func;
  int arg;
} duf_void_int_arg_funcpair_t;
typedef struct
{
  duf_errc_int_func_t func;
  int arg;
} duf_errc_int_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_int_void_func_t func;
} duf_int_void_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_cargcv_func_t func;
} duf_void_cargcv_funcpair_t;
typedef struct
{
  duf_errc_cargcv_func_t func;
} duf_errc_cargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_carg_func_t func;
} duf_void_carg_funcpair_t;
typedef struct
{
  duf_errc_cargv_func_t func;
} duf_errc_cargv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pargcv_func_t func;
} duf_void_pargcv_funcpair_t;
typedef struct
{
  duf_errc_pargcv_func_t func;
} duf_errc_pargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_n_func_t func;
} duf_void_n_funcpair_t;
typedef struct
{
  duf_errc_n_func_t func;
} duf_errc_n_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pargcvn_func_t func;
} duf_void_pargcvn_funcpair_t;
typedef struct
{
  duf_errc_pargcvn_func_t func;
} duf_errc_pargcvn_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_errc_argvn_func_t func;
} duf_errc_argvn_funcpair_t;

/********************************************************************************************/

typedef struct
{
  duf_void_csv_func_t func;
} duf_void_csv_funcpair_t;
typedef struct
{
  duf_errc_csv_func_t func;
} duf_errc_csv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_csv_func_t func;
  const char *arg;
} duf_void_psv_arg_funcpair_t;
typedef struct
{
  duf_errc_csv_func_t func;
  const char *arg;
} duf_errc_psv_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_csn_func_t func;
  long arg;
} duf_void_csn_arg_funcpair_t;
typedef struct
{
  duf_errc_csn_func_t func;
  long arg;
} duf_errc_csn_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pcargcvcs_func_t func;
} duf_void_pcargcvcs_funcpair_t;
typedef struct
{
  duf_errc_pcargcvcs_func_t func;
} duf_errc_pcargcvcs_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_errc_cargvs_func_t func;
} duf_errc_cargvs_funcpair_t;

/********************************************************************************************/

typedef struct
{
  duf_void_void_func_t func;
} duf_void_void_funcpair_t;
typedef struct
{
  duf_errc_void_func_t func;
} duf_errc_void_funcpair_t;

#endif
