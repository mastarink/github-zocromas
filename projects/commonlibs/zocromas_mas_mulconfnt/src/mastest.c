#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"

int test_popt( int argc, const char *argv[] );
int test_0( int argc, const char *argv[] );
int test_1( int argc, const char *argv[] );

int do_fprintf = 0;
static int tests_count = 0, tests_count_good = 0, tests_count_bad = 0;

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ), char **envp __attribute__ ( ( unused ) ) )
{
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

static void destructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ), char **envp __attribute__ ( ( unused ) ) )
{
  if ( tests_count )
    fprintf( stderr, "TESTS DONE: %d (%d/%d)\n\n\n\n\n\n\n", tests_count, tests_count_good, tests_count_bad );
}

int
mastest_vexam( const char *name, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args )
{
  if ( !tests_count )
    fprintf( stderr, "\n\nTESTS:\n" );
  if ( cond )
    tests_count_good++;
  else
    tests_count_bad++;
  tests_count++;
  fprintf( stderr, "%4d\t**** %s : %-30s --\t", tests_count, name, cond ? goodmsg : badmsg );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  return cond;
}

int
mastest_exam( const char *name, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  cond = mastest_vexam( name, cond, goodmsg, badmsg, fmt, args );
  va_end( args );

  return cond;
}

/* getopt_long */
int
main( int argc, const char *argv[] )
{
//mas_strdup( "abrakadabra" );

  if ( do_fprintf )
    fprintf( stderr, "START\n" );
  if ( 0 )
    test_popt( argc, argv );

  if ( 0 )
    test_0( argc, argv );

  if ( 1 )
    test_1( argc, argv );

  return 0;
}
