#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_autoobject_types.h"
#include "mas_autoobject.h"

int
main( void )
{
  mas_autoobject_t *ao;

  ao = mas_autoobject_create(  );
  printf( "=== CHAR ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_CHAR );
      mas_autoobject_set_data( ao, "Hello world\n" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao );
  }
  printf( "=== FCHAR ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_FCHAR );
      mas_autoobject_set_data( ao, "hwif.txt" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao );
  }
  printf( "=== SENDFILE ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_SENDFILE );
      mas_autoobject_set_data( ao, "hwif.txt" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao );
  }
  printf( "=== SENDFILE ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_SENDFILE );
      mas_autoobject_set_data( ao, "hwif.txt" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao );
      mas_autoobject_set_data( ao, "hwif2.txt" );
    mas_autoobject_cat( STDOUT_FILENO, ao );
    mas_autoobject_cat( STDOUT_FILENO, ao );
      mas_autoobject_set_data( ao, "hwif1.txt" );
    mas_autoobject_cat( STDOUT_FILENO, ao );
  }







  mas_autoobject_delete( ao );
#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif

  return 0;
}
