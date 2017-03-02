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
MYSQL_TYPE_LONGLONG
MYSQL_TYPE_FLOAT
MYSQL_TYPE_DOUBLE
MYSQL_TYPE_TIME
MYSQL_TYPE_DATE
MYSQL_TYPE_DATETIME
MYSQL_TYPE_TIMESTAMP
MYSQL_TYPE_STRING
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

int
mas_mysqlpfs_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->bind );
    if ( mstmt->bind && buffer_length )
    {
      mysqlpfs_s_length_t *plength = mstmt->length + pos;

      mstmt->bind[pos].buffer_type = MYSQL_TYPE_STRING;
      if ( mstmt->allocated_buffers[pos] )
        mas_free( mstmt->allocated_buffers[pos] );
      mstmt->allocated_buffers[pos] = mstmt->bind[pos].buffer = mas_calloc( 1, buffer_length );
      mstmt->bind[pos].buffer_length = buffer_length;
/* mstmt->bind[pos].is_unsigned = 0; */
      mstmt->bind[pos].is_null = 0;
      mstmt->bind[pos].length = plength;
      r = 0;
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
  {
    QRGP( mstmt->bind );
    if ( mstmt->bind )
    {
      if ( pos < mstmt->nbind && mstmt->allocated_buffers[pos] && string && mstmt->bind[pos].buffer_length )
      {
        char *s = mstmt->allocated_buffers[pos];
        mysqlpfs_s_length_t *plength = mstmt->length + pos;

        if ( string )
        {
          strncpy( s, string, mstmt->bind[pos].buffer_length - 1 );
          s[mstmt->bind[pos].buffer_length - 1] = 0;
          *plength = strlen( s );
        }
        else
        {
          *s = 0;
          *plength = 0;
        }
        r = 0;
      }
      else
      {
        fprintf( stderr, "%d / %d %p %p\n", pos, mstmt->nbind, mstmt->allocated_buffers, mstmt->allocated_buffers[pos] );
        QRG( -1 );
      }
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_set_direct_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *str )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->bind );
    QRGP( mstmt->length );
    if ( mstmt->bind && mstmt->length && pos < mstmt->nbind )
    {
      void **albs = mstmt->allocated_buffers;

      fprintf( stderr, "(%d) BIND '%s'\n", pos, str );
      mstmt->bind[pos].buffer_type = MYSQL_TYPE_STRING;
      QRGP( albs );
      if ( albs )
      {
        QRGP( albs[pos] );
        if ( albs[pos] )
        {
          mas_free( albs[pos] );
          albs[pos] = NULL;
          mstmt->bind[pos].buffer_length = 0;
        }
      }
      {
        mysqlpfs_s_length_t *plength = mstmt->length + pos;

        *plength = strlen( str );
        mstmt->bind[pos].buffer = ( char * ) str;
      /* mstmt->bind[pos].buffer_length = 255; */
/* mstmt->bind[pos].is_unsigned = 0; */
        mstmt->bind[pos].is_null = 0;
        mstmt->bind[pos].length = plength;
      }
      r = 0;
    }
  }
  return r;
}

int
mas_mysqlpfs_mstmt_bind( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->bind );
    if ( mstmt->bind )
    {
      r = mysql_stmt_bind_param( mstmt->stmt, mstmt->bind );
      QRGS( r );
    }
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
      r = mysql_stmt_execute( mstmt->stmt );
      QRGS( r );
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

my_ulonglong
mas_mysqlpfs_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt )
{
  my_ulonglong nr = 0;

  QRGP( mstmt );
  if ( mstmt && mstmt->stmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
      nr = mysql_stmt_affected_rows( mstmt->stmt );
  }
  return nr;
}
