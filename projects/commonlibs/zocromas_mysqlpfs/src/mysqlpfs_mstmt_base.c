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

#include "mysqlpfs_mstmt_base.h"

void
mas_mysqlpfs_mstmt_init( mysqlpfs_t * pfs, mysqlpfs_mstmt_t * mstmt, int nbind )
{
  mstmt->stmt = mysql_stmt_init( &pfs->mysql );
  mstmt->nbind = nbind;
  mstmt->bind = mas_calloc( nbind, sizeof( mysqlpfs_s_bind_t ) );
  mstmt->length = mas_calloc( nbind, sizeof( mysqlpfs_s_length_t ) );
  mstmt->is_null = mas_calloc( nbind, sizeof( mysqlpfs_s_bool_t ) );
  mstmt->allocated_buffers = mas_calloc( nbind, sizeof( void * ) );
}

mysqlpfs_mstmt_t *
mas_mysqlpfs_mstmt_create( void )
{
  mysqlpfs_mstmt_t *mstmt = mas_calloc( 1, sizeof( mysqlpfs_mstmt_t ) );

  return mstmt;
}

mysqlpfs_mstmt_t *
mas_mysqlpfs_mstmt_create_setup( mysqlpfs_t * pfs, int nparams, const char *sqlop _uUu_ )
{
  mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create(  );

  QRGP( mstmt );
  if ( mstmt )
  {
    mas_mysqlpfs_mstmt_init( pfs, mstmt, nparams );
    mas_mysqlpfs_mstmt_prepare( mstmt, sqlop );
  }
  return mstmt;
}

int
mas_mysqlpfs_mstmt_reset( mysqlpfs_mstmt_t * mstmt )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      r = mysql_stmt_close( mstmt->stmt );
      QRGS( r );
    }
    for ( int i = 0; i < mstmt->nbind; i++ )
    {
      if ( mstmt->allocated_buffers[i] )
        mas_free( mstmt->allocated_buffers[i] );
    }
    if ( mstmt->allocated_buffers )
      mas_free( mstmt->allocated_buffers );
    if ( mstmt->is_null )
      mas_free( mstmt->is_null );
    if ( mstmt->length )
      mas_free( mstmt->length );
    if ( mstmt->bind )
      mas_free( mstmt->bind );
  }
  return r;
}

void
mas_mysqlpfs_mstmt_delete( mysqlpfs_mstmt_t * mstmt )
{
  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      mas_mysqlpfs_mstmt_reset( mstmt );
      mas_free( mstmt );
    }
  }
}
