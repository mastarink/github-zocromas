#define R_GOOD(_r) (!(_r))
#define RGEMSG mysql_stmt_error(mstmt->stmt)
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
/* #include "mysqlpfs.h" */

#include "mysqlpfs_mstmt.h"

  /* mysql_stmt_bind_result
   * mysql_stmt_fetch
   *
   *  mysql_stmt_errno (stmt)
   *  mysql_stmt_sqlstate(stmt)
   *  mysql_stmt_error (stmt)
   * */

/*
MYSQL_TYPE_TINY
MYSQL_TYPE_SHORT
PFS_LONGLONG
MYSQL_TYPE_FLOAT
MYSQL_TYPE_DOUBLE
MYSQL_TYPE_TIME
MYSQL_TYPE_DATE
MYSQL_TYPE_DATETIME
MYSQL_TYPE_TIMESTAMP
PFS_STRING
MYSQL_TYPE_BLOB
MYSQL_TYPE_NULL
*/

/* https://dev.mysql.com/doc/refman/5.7/en/c-api-prepared-statement-functions.html */

int
mas_mysqlpfs_mstmt_prepare( mysqlpfs_mstmt_t * mstmt, const char *sqlop )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      QRGP( sqlop );
      if ( sqlop )
      {
        rC( mysql_stmt_prepare( mstmt->stmt, sqlop, strlen( sqlop ) ) );
        QRGS( rCODE );
      }
    }
  }
  rRET;
}

/*
from https://dev.mysql.com/doc/refman/5.7/en/mysql-stmt-reset.html
---
 Resets a prepared statement on client and server to state after prepare. 
 It resets the statement on the server, data sent using mysql_stmt_send_long_data(), unbuffered result sets and current errors.
 It does not clear bindings or stored result sets. Stored result sets will be cleared when executing the prepared statement (or closing it).

 To re-prepare the statement with another query, use mysql_stmt_prepare().
*/
int
mas_mysqlpfs_mstmt_unprepare( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    rC( mysql_stmt_reset( mstmt->stmt ) );
    QRGS( rCODE );
  }
  return rCODE;
}

static int
mas_mysqlpfs_mstmt_prepare_mbind_gen( mysqlpfs_mbind_t * mbind, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    QRGP( mbind->length );
    if ( mbind->length )
    {
      QRGP( mbind->is_null );
      if ( mbind->is_null )
      {
        if ( pos < mbind->nbind )
        {
          mysqlpfs_s_length_t *p_length = mbind->length + pos;
          mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;

          mbind->bind[pos].buffer_type = ft;
          mbind->bind[pos].is_null = p_is_null;
          mbind->bind[pos].length = p_length;
          QRGP( mbind->allocated_buffers );
          if ( mbind->allocated_buffers )
          {
            if ( mbind->allocated_buffers[pos] )
              mas_free( mbind->allocated_buffers[pos] );
            mbind->allocated_buffers[pos] = mbind->bind[pos].buffer = mas_calloc( 1, buffer_length );
            mbind->bind[pos].buffer_length = buffer_length;
/* mbind->bind[pos].is_unsigned = 0; */
            rCODE = 0;
          }
        }
        else
        {
          QRGM( rCODE, "ERROR: pos:%d;mbind->nbind:%d", pos, mbind->nbind );
        /* ADIE( "ERROR: pos:%d;mbind->nbind:%d", pos, mbind->nbind ); */
        }
      }
    }
  }
  return rCODE;
}

static int
mas_mysqlpfs_mstmt_prepare_param_gen( mysqlpfs_mstmt_t * mstmt, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_prepare_mbind_gen( &mstmt->binds.param, pos, ft, buffer_length ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_prepare_param_gen( mstmt, pos, PFS_LONGLONG, sizeof( mysqlpfs_s_ulonglong_t ) ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_prepare_param_gen( mstmt, pos, PFS_STRING, buffer_length ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_prepare_param_binary( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_prepare_param_gen( mstmt, pos, PFS_BINARY, buffer_length ) );
  rRET;
}

static int
mas_mysqlpfs_mstmt_prepare_result_gen( mysqlpfs_mstmt_t * mstmt, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_prepare_mbind_gen( &mstmt->binds.result, pos, ft, buffer_length ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_prepare_result_gen( mstmt, pos, PFS_LONGLONG, sizeof( mysqlpfs_s_ulonglong_t ) ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_prepare_result_gen( mstmt, pos, PFS_STRING, buffer_length ) );
  rRET;
}

/* */

int
mas_mysqlpfs_mstmt_set_bind_longlong( mysqlpfs_mbind_t * mbind, int pos, unsigned long long num, unsigned is_null )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;
      mysqlpfs_s_ulonglong_t *p = mbind->allocated_buffers[pos];

      *p = ( mysqlpfs_s_ulonglong_t ) num;
      *p_is_null = ( mysqlpfs_s_bool_t ) is_null;
      *p_length = 0;
      rSETGOOD;
    }
    else
    {
      WARN( "Unmet: (pos:(%d) < nbind:(%d)) and %p and %p", pos, mbind->nbind, mbind->allocated_buffers, mbind->allocated_buffers[pos] );
      rSETBAD;
      QRG( rCODE );
    }
  }
  rRET;
}

static int
mas_mysqlpfs_mstmt_get_bind_longlong( mysqlpfs_mbind_t * mbind, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;
      mysqlpfs_s_ulonglong_t *p = mbind->allocated_buffers[pos];

      if ( pis_null )
        *pis_null = ( unsigned ) *p_is_null;
      if ( pnum )
        *pnum = ( *p_is_null ) ? 0L : ( unsigned long long ) *p;
      rSETGOOD;
    }
    else
    {
      QRG( -1 );
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_longlong( &mstmt->binds.param, pos, num, is_null ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_longlong( &mstmt->binds.param, pos, pnum, pis_null ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_set_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_longlong( &mstmt->binds.result, pos, num, is_null ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_longlong( &mstmt->binds.result, pos, pnum, pis_null ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_set_bind_string( mysqlpfs_mbind_t * mbind, int pos, const char *string )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      char *s = mbind->allocated_buffers[pos];
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;

      if ( string )
      {
        strncpy( s, string, mbind->bind[pos].buffer_length - 1 );
        s[mbind->bind[pos].buffer_length - 1] = 0;
        *p_length = strlen( s );
        *p_is_null = ( mysqlpfs_s_bool_t ) 0;
      }
      else
      {
        *s = 0;
        *p_length = 0;
        *p_is_null = ( mysqlpfs_s_bool_t ) 1;
      }
      rSETGOOD;
    }
    else
    {
      WARN( "pos:%d / nbind:%d ab:%p abp:%p\n", pos, mbind->nbind, mbind->allocated_buffers,
            mbind->allocated_buffers ? mbind->allocated_buffers[pos] : NULL );
      rSETBAD;
      QRG( rCODE );
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_set_bind_binary( mysqlpfs_mbind_t * mbind, int pos, const unsigned char *binary, size_t length )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && binary && mbind->bind[pos].buffer_length )
    {
      unsigned char *s = mbind->allocated_buffers[pos];
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;

      if ( binary )
      {
        if ( length > mbind->bind[pos].buffer_length )
          length = mbind->bind[pos].buffer_length;
        memcpy( s, binary, length );
        *p_length = length;
        *p_is_null = ( mysqlpfs_s_bool_t ) 0;
      }
      else
      {
        *s = 0;
        *p_length = 0;
        *p_is_null = ( mysqlpfs_s_bool_t ) 1;
      }
      rSETGOOD;
    }
    else
    {
      WARN( "pos:%d / nbind:%d ab:%p abp:%p\n", pos, mbind->nbind, mbind->allocated_buffers,
            mbind->allocated_buffers ? mbind->allocated_buffers[pos] : NULL );
      rSETBAD;
      QRG( rCODE );
    }
  }
  rRET;
}

static int
mas_mysqlpfs_mstmt_get_bind_string_na( mysqlpfs_mbind_t * mbind, int pos, const char **pstring )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      char *s = mbind->allocated_buffers[pos];
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;

      if ( pstring )
      {
        *pstring = NULL;
        if ( !*p_is_null && s )
        {
          s[*p_length] = 0;
          *pstring = s;
        }
      }
      rSETGOOD;
    }
    else
    {
      rSETBAD;
      QRG( rCODE );
    }
  }
  rRET;
}

static int
mas_mysqlpfs_mstmt_get_bind_binary_na( mysqlpfs_mbind_t * mbind, int pos, const unsigned char **pstring )
{
  rDECLBAD;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      unsigned char *s = mbind->allocated_buffers[pos];
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;

      if ( pstring )
      {
        *pstring = NULL;
        if ( !*p_is_null && s )
        {
          s[*p_length] = 0;
          *pstring = s;
        }
      }
      rSETGOOD;
    }
    else
    {
      rSETBAD;
      QRG( rCODE );
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_string( &mstmt->binds.param, pos, string ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_param_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_string_na( &mstmt->binds.param, pos, pstring ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_set_result_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_string( &mstmt->binds.result, pos, string ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_string_na( &mstmt->binds.result, pos, pstring ) );
  rRET;
}

 /**/ int
mas_mysqlpfs_mstmt_set_param_binary( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char *binary, size_t length )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_binary( &mstmt->binds.param, pos, binary, length ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_param_binary_na( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char **pbinary )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_binary_na( &mstmt->binds.param, pos, pbinary ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_set_result_binary( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char *binary, size_t length )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_set_bind_binary( &mstmt->binds.result, pos, binary, length ) );
  rRET;
}

int
mas_mysqlpfs_mstmt_get_result_binary_na( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char **pbinary )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
    rC( mas_mysqlpfs_mstmt_get_bind_binary_na( &mstmt->binds.result, pos, pbinary ) );
  rRET;
}

 /**/ int
mas_mysqlpfs_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->binds.param.bind );
    if ( mstmt->binds.param.bind )
    {
      rC( mysql_stmt_bind_param( mstmt->stmt, mstmt->binds.param.bind ) );
      QRGS( rCODE );
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->binds.result.bind );
    if ( mstmt->binds.result.bind )
    {
      rC( mysql_stmt_bind_result( mstmt->stmt, mstmt->binds.result.bind ) );
      QRGS( rCODE );
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_store_result( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    rC( mysql_stmt_store_result( mstmt->stmt ) );
    QRGS( rCODE );
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_free_result( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    rC( mysql_stmt_free_result( mstmt->stmt ) );
    QRGS( rCODE );
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data )
{
  rDECLBAD;
  int has_data = 0;

  QRGP( mstmt );
  if ( mstmt )
  {
    rC( mysql_stmt_fetch( mstmt->stmt ) );
    if ( rCODE == MYSQL_NO_DATA )
    {
      rSETGOOD;
    }
    else
    {
      has_data = 1;
      QRGS( rCODE );
    }
  }
  if ( phas_data )
    *phas_data = has_data;
  rRET;
}

int
mas_mysqlpfs_mstmt_execute( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      rC( mysql_stmt_execute( mstmt->stmt ) );
      QRGS( rCODE );
    /* if ( rCODE )                                       */
    /*   WARN( "[%s]", mysql_stmt_error( mstmt->stmt ) ); */
    }
  }
  rRET;
}

int
mas_mysqlpfs_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
  rC( mas_mysqlpfs_mstmt_store_result( mstmt ) );
  rRET;
}

unsigned long long
mas_mysqlpfs_mstmt_num_rows( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_num_rows( mstmt->stmt );
  }
  return ( unsigned long long ) nr;
}

unsigned long long
mas_mysqlpfs_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_affected_rows( mstmt->stmt );
  }
  return ( unsigned long long ) nr;
}

unsigned long long
mas_mysqlpfs_mstmt_insert_id( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_insert_id( mstmt->stmt );
  }
  return ( unsigned long long ) nr;
}

int
mas_mysqlpfs_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset )
{
  rDECLBAD;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGPM( mstmt->stmt, "no stmt" );
    if ( mstmt->stmt )
    {
      mysql_stmt_data_seek( mstmt->stmt, ( my_ulonglong ) offset );
      rSETGOOD;
    }
  }
  rRET;
}

unsigned int
mas_mysqlpfs_mstmt_errno( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      rC( mysql_stmt_errno( mstmt->stmt ) );
  }
  rRET;
}

const char *
mas_mysqlpfs_mstmt_error( mysqlpfs_mstmt_t * mstmt )
{
  const char *s = NULL;

  QRGPM( mstmt, "no mstmt" );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      s = mysql_stmt_error( mstmt->stmt );
  }
  return s;
}
