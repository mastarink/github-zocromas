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
/* #include <mastar/minierr/minierr.h> */
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
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      QRGP( sqlop );
      if ( sqlop )
      {
        r = mysql_stmt_prepare( mstmt->stmt, sqlop, strlen( sqlop ) );
        QRGS( r );
      }
    }
  }
  return r;
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
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    r = mysql_stmt_reset( mstmt->stmt );
    QRGS( r );
  }
  return r;
}

static int
mas_mysqlpfs_mstmt_prepare_mbind_gen( mysqlpfs_mbind_t * mbind, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

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
            r = 0;
          }
        }
        else
        {
          fprintf( stderr, "ERROR: pos:%d;mbind->nbind:%d\n", pos, mbind->nbind );
          QRG( -1 );
        }
      }
    }
  }
  return r;
}

static int
mas_mysqlpfs_mstmt_prepare_param_gen( mysqlpfs_mstmt_t * mstmt, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_prepare_mbind_gen( &mstmt->binds.param, pos, ft, buffer_length );
  return r;
}

int
mas_mysqlpfs_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  int r = -1;

  r = mas_mysqlpfs_mstmt_prepare_param_gen( mstmt, pos, PFS_LONGLONG, sizeof( mysqlpfs_s_ulonglong_t ) );
  return r;
}

int
mas_mysqlpfs_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

  r = mas_mysqlpfs_mstmt_prepare_param_gen( mstmt, pos, PFS_STRING, buffer_length );
  return r;
}

static int
mas_mysqlpfs_mstmt_prepare_result_gen( mysqlpfs_mstmt_t * mstmt, int pos, enum enum_field_types ft, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_prepare_mbind_gen( &mstmt->binds.result, pos, ft, buffer_length );
  return r;
}

int
mas_mysqlpfs_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  int r = -1;

  r = mas_mysqlpfs_mstmt_prepare_result_gen( mstmt, pos, PFS_LONGLONG, sizeof( mysqlpfs_s_ulonglong_t ) );
  return r;
}

int
mas_mysqlpfs_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

  r = mas_mysqlpfs_mstmt_prepare_result_gen( mstmt, pos, PFS_STRING, buffer_length );
  return r;
}

/* */

int
mas_mysqlpfs_mstmt_set_bind_longlong( mysqlpfs_mbind_t * mbind, int pos, mysqlpfs_s_ulonglong_t num, mysqlpfs_s_bool_t is_null )
{
  int r = -1;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      mysqlpfs_s_length_t *p_length = mbind->length + pos;
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;
      mysqlpfs_s_ulonglong_t *p = mbind->allocated_buffers[pos];

    /* fprintf( stderr, "\t\t\t%d ### IS NULL %p - %p [%d]\n", is_null, mbind->bind[pos].is_null, p_is_null, mbind->bind[pos].is_null == p_is_null ); */

      *p = num;
      *p_is_null = is_null;
      *p_length = 0;
      r = 0;
    }
    else
    {
      fprintf( stderr, "%d / %d %p %p\n", pos, mbind->nbind, mbind->allocated_buffers, mbind->allocated_buffers[pos] );
      QRG( -1 );
    }
  }
  return r;
}

static int
mas_mysqlpfs_mstmt_get_bind_longlong( mysqlpfs_mbind_t * mbind, int pos, mysqlpfs_s_ulonglong_t * pnum, mysqlpfs_s_bool_t * pis_null )
{
  int r = -1;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && mbind->bind[pos].buffer_length )
    {
      mysqlpfs_s_bool_t *p_is_null = mbind->is_null + pos;
      mysqlpfs_s_ulonglong_t *p = mbind->allocated_buffers[pos];

      if ( pnum )
        *pnum = *p;
      if ( pis_null )
        *pis_null = *p_is_null;
      r = 0;
    }
    else
    {
      QRG( -1 );
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_ulonglong_t num, mysqlpfs_s_bool_t is_null )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_set_bind_longlong( &mstmt->binds.param, pos, num, is_null );
  return r;
}

int
mas_mysqlpfs_mstmt_get_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_ulonglong_t * pnum, mysqlpfs_s_bool_t * pis_null )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_get_bind_longlong( &mstmt->binds.param, pos, pnum, pis_null );
  return r;
}

int
mas_mysqlpfs_mstmt_set_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_ulonglong_t num, mysqlpfs_s_bool_t is_null )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_set_bind_longlong( &mstmt->binds.result, pos, num, is_null );
  return r;
}

int
mas_mysqlpfs_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_ulonglong_t * pnum, mysqlpfs_s_bool_t * pis_null )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_get_bind_longlong( &mstmt->binds.result, pos, pnum, pis_null );
  return r;
}

int
mas_mysqlpfs_mstmt_set_bind_string( mysqlpfs_mbind_t * mbind, int pos, const char *string )
{
  int r = -1;

  QRGP( mbind->bind );
  if ( mbind->bind )
  {
    if ( pos < mbind->nbind && mbind->allocated_buffers[pos] && string && mbind->bind[pos].buffer_length )
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
      r = 0;
    }
    else
    {
      fprintf( stderr, "%d / %d %p %p\n", pos, mbind->nbind, mbind->allocated_buffers, mbind->allocated_buffers[pos] );
      QRG( -1 );
    }
  }
  return r;
}

static int
mas_mysqlpfs_mstmt_get_bind_string( mysqlpfs_mbind_t * mbind, int pos, const char **pstring )
{
  int r = -1;

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
          *pstring = mas_strndup( s, *p_length );
        }
      }
      r = 0;
    }
    else
    {
      QRG( -1 );
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_set_bind_string( &mstmt->binds.param, pos, string );
  return r;
}

int
mas_mysqlpfs_mstmt_get_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_get_bind_string( &mstmt->binds.param, pos, pstring );
  return r;
}

int
mas_mysqlpfs_mstmt_set_result_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_set_bind_string( &mstmt->binds.result, pos, string );
  return r;
}

int
mas_mysqlpfs_mstmt_get_result_string( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
    r = mas_mysqlpfs_mstmt_get_bind_string( &mstmt->binds.result, pos, pstring );
  return r;
}

int
mas_mysqlpfs_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->binds.param.bind );
    if ( mstmt->binds.param.bind )
    {
      r = mysql_stmt_bind_param( mstmt->stmt, mstmt->binds.param.bind );
      QRGS( r );
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->binds.result.bind );
    if ( mstmt->binds.result.bind )
    {
      r = mysql_stmt_bind_result( mstmt->stmt, mstmt->binds.result.bind );
      QRGS( r );
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_store_result( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    r = mysql_stmt_store_result( mstmt->stmt );
    QRGS( r );
  }
  return r;
}

int
mas_mysqlpfs_mstmt_free_result( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    r = mysql_stmt_free_result( mstmt->stmt );
    QRGS( r );
  }
  return r;
}

int
mas_mysqlpfs_mstmt_fetch( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    r = mysql_stmt_fetch( mstmt->stmt );
    if ( r != MYSQL_NO_DATA )
      QRGS( r );
  }
  return r;
}

int
mas_mysqlpfs_mstmt_execute( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt && mstmt->stmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
    /* fprintf( stderr, "STRING: '%s'(%lu),%lld - IS NULL:%p:%d\n", ( char * ) mstmt->binds.param.bind[0].buffer, *mstmt->binds.param.bind[0].length, */
    /*          *( ( mysqlpfs_s_ulonglong_t * ) mstmt->binds.param.bind[1].buffer ), mstmt->binds.param.bind[1].is_null,                              */
    /*          *mstmt->binds.param.bind[1].is_null );                                                                                                */
      r = mysql_stmt_execute( mstmt->stmt );
      QRGS( r );
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  r = mas_mysqlpfs_mstmt_execute( mstmt );
  if ( !r )
    r = mas_mysqlpfs_mstmt_store_result( mstmt );
  return r;
}

mysqlpfs_s_ulonglong_t
mas_mysqlpfs_mstmt_num_rows( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_num_rows( mstmt->stmt );
  }
  return nr;
}

mysqlpfs_s_ulonglong_t
mas_mysqlpfs_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_affected_rows( mstmt->stmt );
  }
  return nr;
}

mysqlpfs_s_ulonglong_t
mas_mysqlpfs_mstmt_insert_id( mysqlpfs_mstmt_t * mstmt )
{
  mysqlpfs_s_ulonglong_t nr = 0;

  QRGP( mstmt );
  if ( mstmt && mstmt->stmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_insert_id( mstmt->stmt );
  }
  return nr;
}
