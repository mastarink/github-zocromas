#ifndef MAS_MUC_OPTION_FUN_TYPES_H
# define MAS_MUC_OPTION_FUN_TYPES_H

# include "muc_fun_types.h"                                          /* _func_t */

/********************************************************************************************/
typedef struct
{
  muc_void_int_func_t func;
  int arg;
} muc_void_int_arg_funcpair_t;
typedef struct
{
  muc_errc_int_func_t func;
  int arg;
} muc_errc_int_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_int_void_func_t func;
} muc_int_void_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_cargcv_func_t func;
} muc_void_cargcv_funcpair_t;
typedef struct
{
  muc_errc_cargcv_func_t func;
} muc_errc_cargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_carg_func_t func;
} muc_void_carg_funcpair_t;
typedef struct
{
  muc_errc_cargv_func_t func;
} muc_errc_cargv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_pargcv_func_t func;
} muc_void_pargcv_funcpair_t;
typedef struct
{
  muc_errc_pargcv_func_t func;
} muc_errc_pargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_n_func_t func;
} muc_void_n_funcpair_t;
typedef struct
{
  muc_errc_n_func_t func;
} muc_errc_n_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_pargcvn_func_t func;
} muc_void_pargcvn_funcpair_t;
typedef struct
{
  muc_errc_pargcvn_func_t func;
} muc_errc_pargcvn_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_errc_argvn_func_t func;
} muc_errc_argvn_funcpair_t;

/********************************************************************************************/

typedef struct
{
  muc_void_csv_func_t func;
} muc_void_csv_funcpair_t;
typedef struct
{
  muc_errc_csv_func_t func;
} muc_errc_csv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_csv_func_t func;
  const char *arg;
} muc_void_psv_arg_funcpair_t;
typedef struct
{
  muc_errc_csv_func_t func;
  const char *arg;
} muc_errc_psv_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_csn_func_t func;
  long arg;
} muc_void_csn_arg_funcpair_t;
typedef struct
{
  muc_errc_csn_func_t func;
  long arg;
} muc_errc_csn_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_void_pcargcvcs_func_t func;
} muc_void_pcargcvcs_funcpair_t;
typedef struct
{
  muc_errc_pcargcvcs_func_t func;
} muc_errc_pcargcvcs_funcpair_t;

/********************************************************************************************/
typedef struct
{
  muc_errc_cargvs_func_t func;
} muc_errc_cargvs_funcpair_t;

/********************************************************************************************/

typedef struct
{
  muc_void_void_func_t func;
} muc_void_void_funcpair_t;
typedef struct
{
  muc_errc_void_func_t func;
} muc_errc_void_funcpair_t;

#endif
