#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


int
main( void )
{


#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif

  return 0;
}
