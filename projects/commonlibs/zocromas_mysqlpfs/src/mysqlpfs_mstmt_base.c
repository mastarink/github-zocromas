#define R_GOOD(_r) (!(_r))
#define RGEMSG mysql_stmt_error(mstmt->stmt)
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
/* #include "mysqlpfs.h" */

#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_mstmt_base.h"

void
mas_mysqlpfs_mstmt_init_bind( mysqlpfs_mbind_t * mbind, int nbind )
{
  mbind->nbind = nbind;
  mbind->bind = mas_calloc( nbind, sizeof( mysqlpfs_s_bind_t ) );
  mbind->length = mas_calloc( nbind, sizeof( mysqlpfs_s_length_t ) );
  mbind->is_null = mas_calloc( nbind, sizeof( mysqlpfs_s_bool_t ) );
  mbind->allocated_buffers = mas_calloc( nbind, sizeof( void * ) );
}

void
mas_mysqlpfs_mstmt_init( mysqlpfs_t * pfs, mysqlpfs_mstmt_t * mstmt, int nparams, int nresults )
{
  mstmt->stmt = mysql_stmt_init( &pfs->mysql );
  mstmt->pfs = pfs;
/* fprintf( stderr, "nparams: %d; nresults: %d\n", nparams, nresults ); */
  mas_mysqlpfs_mstmt_init_bind( &mstmt->binds.param, nparams );
  mas_mysqlpfs_mstmt_init_bind( &mstmt->binds.result, nresults );
}

mysqlpfs_mstmt_t *
mas_mysqlpfs_mstmt_create( void )
{
  mysqlpfs_mstmt_t *mstmt = mas_calloc( 1, sizeof( mysqlpfs_mstmt_t ) );

  return mstmt;
}
mysqlpfs_mstmt_t *
mas_mysqlpfs_mstmt_create_setup( mysqlpfs_t * pfs, int nparams, int nresults, const char *sqlop )
{
  rDECLBAD;
  mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create(  );

  QRGP( mstmt );
  if ( mstmt )
  {
    mas_mysqlpfs_mstmt_init( pfs, mstmt, nparams, nresults );
    rC( mas_mysqlpfs_mstmt_prepare( mstmt, sqlop ) );
  }
  mstmt->ret_code = rCODE;
/*if ( rBAD )
    WARN( "ERR sqlop: %s", sqlop );*/
  return mstmt;
}

static int
mas_mysqlpfs_mstmt_reset_bind( mysqlpfs_mbind_t * mbind )
{
  for ( int i = 0; i < mbind->nbind; i++ )
  {
    if ( mbind->allocated_buffers[i] )
      mas_free( mbind->allocated_buffers[i] );
  }
  if ( mbind->allocated_buffers )
    mas_free( mbind->allocated_buffers );
  if ( mbind->is_null )
    mas_free( mbind->is_null );
  if ( mbind->length )
    mas_free( mbind->length );
  if ( mbind->bind )
    mas_free( mbind->bind );
  return 0;
}

int
mas_mysqlpfs_mstmt_reset( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->stmt );
    if ( mstmt->stmt )
    {
      rC( mas_mysqlpfs_mstmt_free_result( mstmt ) );
      rC( mysql_stmt_close( mstmt->stmt ) );
      mstmt->stmt = NULL;
      mstmt->ret_code = rCODE;
      /* QRGS( rCODE ); */
    }
    mas_mysqlpfs_mstmt_reset_bind( &mstmt->binds.param );
    mas_mysqlpfs_mstmt_reset_bind( &mstmt->binds.result );
    mstmt->pfs = NULL;
  }
  rRET;
}

void
mas_mysqlpfs_mstmt_delete( mysqlpfs_mstmt_t * mstmt )
{
/* QRGP( mstmt ); */
  if ( mstmt )
  {
  /* QRGP( mstmt->stmt ); */
    if ( mstmt->stmt )
    {
      mas_mysqlpfs_mstmt_reset( mstmt );
      mas_free( mstmt );
    }
  }
}
