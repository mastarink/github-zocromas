/* #undef MAS_TRACING */
#include <stdio.h>
#include <magic.h>

int
main( int argc __attribute__(( unused )), char *argv[] __attribute__(( unused )) )
{
  __attribute__ ( ( unused ) ) int rtst = 0;
  magic_t m = NULL;
  magic_t m1 = NULL;

  m = magic_open( MAGIC_NONE | MAGIC_PRESERVE_ATIME );
  m1 = magic_open( MAGIC_MIME | MAGIC_PRESERVE_ATIME );
  if ( m )
  {
    const char *test[] = {
      "test/stealthyzip",
      "test/stealthyjpeg",
      "test/stealthyjpeg1",
      "test/stealthyjpeg2",
      "test/stealthy3gp",
      "test/stealthypls",
      "test/stealthygratxt",
      "test/stealthywmf",
      "test/stealthygif",
      "test/stealthydll",
      "test/stealthyapk",
      "test/stealthysh",
    };
    rtst = magic_load( m, NULL );
    rtst = magic_load( m1, NULL );
    for ( unsigned i = 0; i < sizeof( test ) / sizeof( test[0] ); i++ )
    {
      const char *desc = NULL;
      const char *desc1 = NULL;
      const char *err = NULL;
      const char *err1 = NULL;

      desc = magic_file( m, test[i] );
      desc1 = magic_file( m1, test[i] );
      err = magic_error( m );
      err1 = magic_error( m1 );
      printf( "%18s:  '%s'   %s\n", test[i], desc, err ? err : "" );
      printf( "%18s:  '%s'   %s\n", "", desc1, err1 ? err1 : "" );
    }
    magic_close( m1 );
    magic_close( m );
  }
}
