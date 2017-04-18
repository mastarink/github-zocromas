#define R_GOOD(_r) ((_r)>=0)
#include <string.h>
/* #include <stdio.h> */
#include <sys/types.h>
#include <unistd.h>

#include "masxfs_levinfo_defs.h"

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
  case MASXFS_DIGEST_MAGIC:
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
    case MASXFS_DIGEST_MAGIC:
      mr = 1;
    /* TODO move to module constructor (magic_close respectively) and mag global instead of placing to ctx ?? */
      digest->ctx.magic = magic_open(  /* MAGIC_MIME | */ MAGIC_PRESERVE_ATIME );
      magic_load( digest->ctx.magic, NULL );
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

  if ( digest && digests )
  {
    mr = 1;
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      if ( digests->bytes2update )                                   /*  NOT eof */
        mr = MD5_Update( digest->ctx.md5, digests->readbuffer, digests->bytes2update );
      break;
    case MASXFS_DIGEST_SHA1:
      if ( digests->bytes2update )                                   /*  NOT eof */
        mr = SHA1_Update( digest->ctx.sha1, digests->readbuffer, digests->bytes2update );
      break;
    case MASXFS_DIGEST_MAGIC:
    /* magic_setflags(magic,MAGIC_MIME_TYPE ); */
    /* magic_setflags(magic, MAGIC_MIME ); */
#if 0
      magic_setflags( digest->ctx.magic, MAGIC_PRESERVE_ATIME );
      mime_plus = mas_strdup( magic_descriptor( digest->ctx.magic, duf_levinfo_dfd( H_PDI ) ) );
#else
# if 1
      if ( digests->reads == 1 )
      {
        magic_setflags( digest->ctx.magic, MAGIC_MIME | MAGIC_PRESERVE_ATIME );
        assert( !digest->sum );
        digest->sum = ( unsigned char * ) mas_strdup( magic_buffer( digest->ctx.magic, digests->readbuffer, digests->read_bytes ) );
      }
# else
      if ( digests->bytes2update == 0 )                              /*  eof */
      {
        magic_setflags( digest->ctx.magic, MAGIC_MIME | MAGIC_PRESERVE_ATIME );
        assert( !digest->sum );
        digest->sum = ( unsigned char * ) mas_strdup( magic_descriptor( digest->ctx.magic, digests->fd ) );
      }
# endif
#endif
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
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      assert( !digest->sum );
      digest->sum = mas_calloc( 1, MD5_DIGEST_LENGTH );
      mr = MD5_Final( digest->sum, digest->ctx.md5 );
      break;
    case MASXFS_DIGEST_SHA1:
      assert( !digest->sum );
      digest->sum = mas_calloc( 1, SHA_DIGEST_LENGTH );
      mr = SHA1_Final( digest->sum, digest->ctx.sha1 );
      break;
    case MASXFS_DIGEST_MAGIC:
      mr = 1;
      magic_close( digest->ctx.magic );
      digest->ctx.magic = 0;
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
    case MASXFS_DIGEST_MAGIC:
      sz = strlen( ( char * ) digest->sum );
      break;
    }
    if ( pbuf )
      *pbuf = digest->sum;
  }
  return sz;
}

int
masxfs_digest_is_string( masxfs_digest_t * digest )
{
  int is_string = 0;

  if ( digest )
  {
    switch ( digest->dgtype )
    {
    case MASXFS_DIGEST_NONE:
      break;
    case MASXFS_DIGEST_MD5:
      is_string = 0;
      break;
    case MASXFS_DIGEST_SHA1:
      is_string = 0;
      break;
    case MASXFS_DIGEST_MAGIC:
      is_string = ( digest->sum ? 1 : 0 );
      break;
    }
  }
  return is_string;
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
  if ( digests )
  {
    masxfs_digest_t *digest = digests->digest;

    while ( rGOOD && digest )
    {
      rC( masxfs_digest_open( digest ) );
      digest = digest->next;
    }
  }
  else
  {
    rSETBAD;
    RGESRM( rCODE, "digests null error" );
  }
  rRET;
}

int
masxfs_digests_update( masxfs_digests_t * digests )
{
  rDECLGOOD;

  if ( digests )
  {
    if ( lseek( digests->fd, digests->from, SEEK_SET ) < 0 )
    {
      rSETBAD;
      RGESRM( rCODE, "digests lseek error" );
    }
    if ( rGOOD )
    {
      int ry = 0;

      do
      {
        masxfs_digest_t *digest = digests->digest;

        ry = read( digests->fd, digests->readbuffer, digests->readbufsize );
        if ( ry < 0 )
        {
          rSETBAD;
          RGESRM( rCODE, "digest read error" );
        }
        else if ( ry >= 0 )
        {
          digests->bytes2update = ry;
          digests->read_bytes += ry;
          digests->reads++;
          while ( rGOOD && digest )
          {
            rC( masxfs_digest_update( digest, digests ) );
            digest = digest->next;
          }
          if ( digests->max_blocks && digests->read_bytes >= digests->readbufsize * digests->max_blocks )
            break;
        }
      } while ( rGOOD && ry > 0 );
    }
  }
  else
  {
    rSETBAD;
    RGESRM( rCODE, "digests null error" );
  }
  rRET;
}

int
masxfs_digests_close( masxfs_digests_t * digests )
{
  rDECLGOOD;
  if ( digests )
  {
    masxfs_digest_t *digest = digests->digest;

    while ( rGOOD && digest )
    {
      rC( masxfs_digest_close( digest ) );
      digest = digest->next;
    }
  }
  else
  {
    rSETBAD;
    RGESRM( rCODE, "digests null error" );
  }
  rRET;
}

void
masxfs_digests_reset( masxfs_digests_t * digests )
{
  if ( digests )
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
    digests->bytes2update = 0;
    digests->fd = 0;
  }
  else
  {
  /* rSETBAD;                               */
  /* RGESRM( rCODE, "digests null error" ); */
  }
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
  if ( digests )
  {
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
  }
  else
  {
    rSETBAD;
    RGESRM( rCODE, "digests null error" );
  }
  rRET;
}

masxfs_digests_t *
masxfs_digests_create_setup( int fd, size_t readbufsize, off_t from, off_t max_blocks )
{
  masxfs_digests_t *digests = masxfs_digests_create(  );

  digests->readbuffer = mas_calloc( 1, readbufsize );
  digests->fd = fd;
  digests->from = from;
  digests->max_blocks = max_blocks;
  digests->readbufsize = readbufsize;

  return digests;
}

int
masxfs_digests_compute( masxfs_digests_t * digests )
{
  rDECLBAD;
  rC( masxfs_digests_open( digests ) );
  rC( masxfs_digests_update( digests ) );
  rC( masxfs_digests_close( digests ) );
  rRET;
}

int
masxfs_digests_get( const masxfs_digests_t * digests, masxfs_digest_type_t dgtype, const unsigned char **pbuf )
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
masxfs_digests_getn( const masxfs_digests_t * digests, int npos, const unsigned char **pbuf )
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

int
masxfs_digests_is_string( masxfs_digests_t * digests, int npos )
{
  int is_string = 0;
  int n = 0;
  masxfs_digest_t *digest = digests->digest;

  while ( digest && npos != n )
  {
    digest = digest->next;
    n++;
  }
  if ( digest )
    is_string = masxfs_digest_is_string( digest );
  return is_string;
}
