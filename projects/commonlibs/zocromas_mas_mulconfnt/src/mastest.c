#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>

#include "parse.h"

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ), char **envp __attribute__ ( ( unused ) ) )
{
  fprintf( stderr, "%s.%d : %d\n", __FILE__, __LINE__, argc );
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

#endif
}

/* getopt_long */
int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
//mas_strdup( "abrakadabra" );
  mulconfnt_parse( argc, argv );
  return 0;
}
