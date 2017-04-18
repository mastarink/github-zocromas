#define R_GOOD(_r) ((_r)>=0)
#include <sys/types.h>
#include <unistd.h>

#include "masxfs_levinfo_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include "masxfs_levinfo_structs.h"

/* #include "masxfs_levinfo_db.h" */
/* #include "masxfs_levinfo_fs.h" */

#include "masxfs_levinfo_digest.h"

masxfs_digest_t *
masxfs_digest_create( void )
{
  masxfs_digest_t *digest = mas_calloc( 1, sizeof( masxfs_digest_t ) );

  return digest;
}

masxfs_digest_t *
masxfs_digest_create_setup( masxfs_digest_type_t dgtype )
{
  masxfs_digest_t *digest = masxfs_digest_create(  );

  digest->dgtype = dgtype;
  assert( !digest->ctx.md5 );
  assert( !digest->ctx.sha1 );
  assert( !digest->sum );
  switch ( dgtype )
  {
  case MASXFS_DIGEST_NONE:
    break;
  case MASXFS_DIGEST_MD5:
    digest->ctx.md5 = ( MD5_CTX * ) mas_calloc( 1, sizeof( MD5_CTX ) );
    break;
  case MASXFS_DIGEST_SHA1:
    digest->ctx.sha1 = ( SHA_CTX * ) mas_calloc( 1, sizeof( SHA_CTX ) );
    break;
  }
  return digest;
}

int
masxfs_digest_open( masxfs_digest_t * digest )
{
  rDECLBAD;
  int mr = 0;

  if ( digest )
  {
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      mr = MD5_Init( digest->ctx.md5 );
      break;
    case MASXFS_DIGEST_SHA1:
      mr = SHA1_Init( digest->ctx.sha1 );
      break;
    }
    if ( mr != 1 )
    {
      rSETBAD;
      RGESRM( rCODE, "digest init error" );
    }
    else
      rSETGOOD;
  }
  rRET;
}

int
masxfs_digest_update( masxfs_digest_t * digest, masxfs_digests_t * digests )
{
  rDECLBAD;
  int mr = 0;

  if ( digest )
  {
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      mr = MD5_Update( digest->ctx.md5, digests->readbuffer, digests->readbytes );
      break;
    case MASXFS_DIGEST_SHA1:
      mr = SHA1_Update( digest->ctx.sha1, digests->readbuffer, digests->readbytes );
      break;
    }

    if ( mr != 1 )
    {
      rSETBAD;
      RGESRM( rCODE, "digest update error" );
    }
    else
      rSETGOOD;
  }
  rRET;
}

int
masxfs_digest_close( masxfs_digest_t * digest )
{
  rDECLBAD;
  int mr = 0;

  if ( digest )
  {
    assert( digest->ctx.md5 );
    assert( digest->ctx.sha1 );
    assert( !digest->sum );
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      digest->sum = mas_calloc( 1, MD5_DIGEST_LENGTH );
      mr = MD5_Final( digest->sum, digest->ctx.md5 );
      break;
    case MASXFS_DIGEST_SHA1:
      digest->sum = mas_calloc( 1, SHA_DIGEST_LENGTH );
      mr = SHA1_Final( digest->sum, digest->ctx.sha1 );
      break;
    }
    if ( mr != 1 )
    {
      rSETBAD;
      RGESRM( rCODE, "digest init error" );
    }
    else
      rSETGOOD;
  }
  rRET;
}

void
masxfs_digest_reset( masxfs_digest_t * digest )
{
  if ( digest )
  {
    digest->dgtype = MASXFS_DIGEST_NONE;
    if ( digest->sum )
    {
      mas_free( digest->sum );
      digest->sum = NULL;
    }
    if ( digest->ctx.any )
    {
      mas_free( digest->ctx.any );
      digest->ctx.any = NULL;
      assert( !digest->ctx.md5 );
      assert( !digest->ctx.sha1 );
    }
  }
}

void
masxfs_digest_delete( masxfs_digest_t * digest )
{
  masxfs_digest_reset( digest );
  mas_free( digest );
}

int
masxfs_digest_get( masxfs_digest_t * digest, const unsigned char **pbuf )
{
  int sz = 0;

  if ( digest )
  {
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      sz = MD5_DIGEST_LENGTH;
      break;
    case MASXFS_DIGEST_SHA1:
      sz = SHA_DIGEST_LENGTH;
      break;
    }
    if ( pbuf )
      *pbuf = digest->sum;
  }
  return sz;
}

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

masxfs_digests_t *
masxfs_digests_create( void )
{
  masxfs_digests_t *digests = mas_calloc( 1, sizeof( masxfs_digests_t ) );

  return digests;
}

int
masxfs_digests_open( masxfs_digests_t * digests )
{
  rDECLGOOD;
  masxfs_digest_t *digest = digests->digest;

  while ( rGOOD && digest )
  {
    rC( masxfs_digest_open( digest ) );
    digest = digest->next;
  }
  rRET;
}

int
masxfs_digests_update( masxfs_digests_t * digests )
{
  rDECLGOOD;

  if ( lseek( digests->fd, 0, SEEK_SET ) < 0 )
  {
    rSETBAD;
    RGESRM( rCODE, "digests lseek error" );
  }
  if ( rGOOD )
    do
    {
      masxfs_digest_t *digest = digests->digest;

      digests->readbytes = read( digests->fd, digests->readbuffer, digests->readbufsize );
      if ( digests->readbytes < 0 )
      {
        rSETBAD;
        RGESRM( rCODE, "SHA read error" );
      }

      if ( digests->readbytes > 0 )
        while ( rGOOD && digest )
        {
          rC( masxfs_digest_update( digest, digests ) );
          digest = digest->next;
        }
    } while ( rGOOD && digests->readbytes > 0 );
  rRET;
}

int
masxfs_digests_close( masxfs_digests_t * digests )
{
  rDECLGOOD;
  masxfs_digest_t *digest = digests->digest;

  while ( rGOOD && digest )
  {
    rC( masxfs_digest_close( digest ) );
    digest = digest->next;
  }
  rRET;
}

void
masxfs_digests_reset( masxfs_digests_t * digests )
{
  masxfs_digest_t *digest = digests->digest;

  digests->digest = NULL;
  while ( digest )
  {
    masxfs_digest_t *next = digest->next;

    masxfs_digest_delete( digest );
    digest = next;
  }
  mas_free( digests->readbuffer );
  digests->readbuffer = NULL;
  digests->readbufsize = 0;
  digests->readbytes = 0;
  digests->fd = 0;
}

void
masxfs_digests_delete( masxfs_digests_t * digests )
{
  masxfs_digests_reset( digests );
  mas_free( digests );
}

int
masxfs_digests_add( masxfs_digests_t * digests, masxfs_digest_type_t dgtype )
{
  rDECLBAD;
  masxfs_digest_t *digest = NULL, *prev_digest = NULL;

  prev_digest = digests->digest;
  if ( prev_digest )
  {
    while ( prev_digest->next )
      prev_digest = prev_digest->next;
  }
  digest = masxfs_digest_create_setup( dgtype );
  if ( digest )
  {
    if ( prev_digest )
      prev_digest->next = digest;
    else
      digests->digest = digest;
    rSETGOOD;
  }
  else
    rSETBAD;
  rRET;
}

masxfs_digests_t *
masxfs_digests_create_setup( int fd, size_t readbufsize )
{
  masxfs_digests_t *digests = masxfs_digests_create(  );

  digests->readbuffer = mas_calloc( 1, readbufsize );
  digests->fd = fd;
  digests->readbufsize = readbufsize;

  return digests;
}

int
masxfs_digests_get( masxfs_digests_t * digests, masxfs_digest_type_t dgtype, const unsigned char **pbuf )
{
  int sz = 0;
  masxfs_digest_t *digest = digests->digest;

  while ( digest && digest->dgtype != dgtype )
  {
    digest = digest->next;
  }
  if ( digest )
    sz = masxfs_digest_get( digest, pbuf );
  return sz;
}

int
masxfs_digests_getn( masxfs_digests_t * digests, int npos, const unsigned char **pbuf )
{
  int sz = 0;
  int n = 0;
  masxfs_digest_t *digest = digests->digest;

  while ( digest && npos != n )
  {
    digest = digest->next;
    n++;
  }
  if ( digest )
    sz = masxfs_digest_get( digest, pbuf );

  return sz;
}

#if 0
static int _uUu_
md5( masxfs_digests_t * digests, masxfs_digest_t * digest, unsigned char *pdgst )
{
  rDECLGOOD;
  unsigned long bytes = 0;
  int mr = 0;

  if ( lseek( digests->fd, 0, SEEK_SET ) < 0 )
  {
    rSETBAD;
    RGESRM( rCODE, "MD5 lseek error" );
  }

  if ( rGOOD )
  {
    char buffer[1024 * 10];

    if ( !digest->inited )
    {
      mr = MD5_Init( &digest->ctx->md5 );
      if ( mr != 1 )
      {
        rSETBAD;
        RGESRM( rCODE, "MD5 init error" );
      }
    }
    {
      int ry = 0;

      do
      {
        ry = read( digests->fd, buffer, sizeof( buffer ) );

        if ( ry < 0 )
        {
          rSETBAD;
          RGESRM( rCODE, "MD5 read error" );
        }
        else if ( ry > 0 )
        {
          bytes += ry;
          mr = MD5_Update( &digest->ctx->md5, digests->readbuffer, ry );
          if ( mr != 1 )
          {
            rSETBAD;
            RGESRM( rCODE, "MD5 update error" );
          }
        }
      } while ( ry > 0 );
    }
  }
  mr = MD5_Final( pdgst, &digest->ctx->md5 );
  if ( mr != 1 )
  {
    rSETBAD;
    RGESRM( rCODE, "MD5 final error" );
  }
  rRET;
}

static int
sha1( int fd, unsigned char *pdgst )
{
  rDECLGOOD;
  SHA_CTX ctx = { 0 };
  unsigned long bytes = 0;
  int mr = 0;

  if ( lseek( fd, 0, SEEK_SET ) < 0 )
  {
    rSETBAD;
    RGESRM( rCODE, "SHA lseek error" );
  }

  if ( rGOOD )
  {
    char buffer[1024 * 10];

    mr = SHA1_Init( &ctx );
    if ( mr != 1 )
    {
      rSETBAD;
      RGESRM( rCODE, "SHA init error" );
    }
    {
      int ry = 0;

      do
      {
        ry = read( fd, buffer, sizeof( buffer ) );

        if ( ry < 0 )
        {
          rSETBAD;
          RGESRM( rCODE, "SHA read error" );
        }
        else if ( ry > 0 )
        {
          bytes += ry;
          mr = SHA1_Update( &ctx, buffer, ry );
          if ( mr != 1 )
          {
            rSETBAD;
            RGESRM( rCODE, "SHA update error" );
          }
        }
      } while ( ry > 0 );
    }
  }
  mr = SHA1_Final( pdgst, &ctx );
  if ( mr != 1 )
  {
    rSETBAD;
    RGESRM( rCODE, "SHA final error" );
  }
  rRET;
}
#endif
