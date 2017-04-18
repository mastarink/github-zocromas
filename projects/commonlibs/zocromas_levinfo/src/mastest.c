#define R_GOOD(_r) ((_r)>=0)
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>
#include <mastar/exam/masexam.h>

#include "masxfs_levinfo_enums.h"
#include "masxfs_levinfo_digest.h"

void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  INFO( "\n\x1b[0;1;44;35m%s %d bytes at %d:%s\x1b[0m", msg, mi.uordblks, line, func );
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( void )
{
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  mallopt( M_CHECK_ACTION, 1 );
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

  INFO( "START MEM" );
#endif
  INFO( "START" );
}

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( void )
{
  mastest_print_allocated( "Still allocated", __LINE__, __func__ );
//malloc_info(0, stderr);
//malloc_stats(  );
}

/* #include "zocromas_levinfo.h" */

int
mastest_digest( void )
{
  rDECLBAD;
  int fd = open( "/home/mastar/.mas/lib/big/misc/media/video/video/fordvd/dvd/pre/Lovely/Other/Kin-dza-dza_1.avi", O_RDONLY );

  if ( fd > 0 )
  {
    for ( int iter = 0; iter < 1; iter++ )
    {
      masxfs_digests_t *digests = masxfs_digests_create_setup( fd, 1024 * 25, 0, 0 );

    /* rC( masxfs_digests_add( digests, MASXFS_DIGEST_MD5 ) ); */
      rC( masxfs_digests_add( digests, MASXFS_DIGEST_SHA1 ) );
      rC( masxfs_digests_add( digests, MASXFS_DIGEST_MAGIC ) );
    /* rC( masxfs_digests_add( digests, MASXFS_DIGEST_SHA1 ) ); */
#if 1
      rC( masxfs_digests_compute( digests ) );
#else
      rC( masxfs_digests_open( digests ) );
      rC( masxfs_digests_update( digests ) );
      rC( masxfs_digests_close( digests ) );
#endif
      if ( rGOOD )
      {
        {
          int sz = 0;
          const unsigned char *dg = NULL;
          char sbuffer[512] = { 0 };
          char *psbuf = sbuffer;

          sz = masxfs_digests_getn( digests, 0, &dg );
          if ( sz )
          {
            for ( ssize_t i = 0; i < sz; i++ )
            {
              snprintf( psbuf, sizeof( sbuffer ) - ( psbuf - sbuffer ), "%02x", dg[i] );
              psbuf += 2;
            }

            WARN( "sz:%d - %s", sz, sbuffer );
          }
        }

        {
          int sz = 0;
          const unsigned char *dg = NULL;
          char sbuffer[512] = { 0 };
          char *psbuf = sbuffer;
          int is_string = masxfs_digests_is_string( digests, 1 );

          sz = masxfs_digests_getn( digests, 1, &dg );
          if ( sz )
          {
            if ( !is_string )
              for ( ssize_t i = 0; i < sz; i++ )
              {
                snprintf( psbuf, sizeof( sbuffer ) - ( psbuf - sbuffer ), "%02x", dg[i] );
                psbuf += 2;
              }
            WARN( "sz:%d - %s - magic:%s", sz, ( !is_string ? sbuffer : "-" ), ( is_string ? ( char * ) dg : "-" ) );
          }
        }
      }
      masxfs_digests_delete( digests );
    }
    close( fd );
  }
  rRET;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* zocromas_levinfo(); */
  mastest_digest(  );
  return 0;
}
