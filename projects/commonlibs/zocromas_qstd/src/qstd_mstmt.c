#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_mysqlpfs_mstmt_error(mstmt)
#include "qstd_defs.h"
#include <string.h>

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
#include <mastar/mysqlpfs/mysqlpfs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"

#include "qstd_mstmt.h"

int
mas_qstd_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, pos ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  
  rRET;
}

int
mas_qstd_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, pos ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_prepare_result_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 ));
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, pos, num, is_null ) );

  rRET;
}

int
mas_qstd_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, pos, string ) );

  rRET;
}

int
mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_data_seek( mstmt, offset ) );

  rRET;
}

int
mas_qstd_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_execute_store( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_fetch( mstmt, phas_data ) );
  rRET;
}

int
mas_qstd_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  return mas_mysqlpfs_mstmt_get_result_longlong( mstmt, pos, pnum, pis_null );
}

int
mas_qstd_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  return mas_mysqlpfs_mstmt_get_result_string_na( mstmt, pos, pstring );
}

int
mas_qstd_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
  if ( !rGOOD )
    WARN( "%s", mas_mysqlpfs_mstmt_error( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_free_result( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_free_result( mstmt );
}
