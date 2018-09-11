#define R_GOOD(_r) ((_r)>=0)
#include <malloc.h>

#include <mastar/minierr/minierr.h>

void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  INFOB(3, "\x1b[0;1;44;37m-=< %s %d bytes at %d:%s >=-\x1b[0m", msg, mi.uordblks, line, func );
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
//INFO( "START MEM" );
#endif
}

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( void )
{
  mastest_print_allocated( "Still allocated", __LINE__, __func__ );
//malloc_info(0, stderr);
//malloc_stats(  );
}
