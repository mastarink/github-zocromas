#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_pid.h"

#include "mas_runonce_config.h"
#include "mas_runonce_config_section.h"


#include "mas_runonce_window.h"


#include "mas_runonce_base.h"

void
mas_atexit( void )
{
  if ( configuration.flags.verbose > 2 )
    printf( "at exit\n" );
  runonce_delete(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
}

int
runonce_create( void )
{
  /*
     cap_t caps;
     cap_value_t cap_list[1];
     char buf[256];


     if ( !CAP_IS_SUPPORTED( CAP_SETGID ) )
     {
     printf( "1 CAP error %s\n", strerror_r( errno, buf, sizeof( buf ) ) );
     }
     caps = cap_get_proc(  );
     if ( caps == NULL )
     {
     printf( "2 CAP error %s\n", strerror_r( errno, buf, sizeof( buf ) ) );
     }
     else
     {
     cap_list[0] = CAP_SETGID;
     if ( cap_set_flag( caps, CAP_EFFECTIVE, 1, cap_list, CAP_SET ) == -1 )
     printf( "3 CAP error %s\n", strerror_r( errno, buf, sizeof( buf ) ) );

     if ( cap_set_proc( caps ) == -1 )
     printf( "4 CAP error %s\n", strerror_r( errno, buf, sizeof( buf ) ) );

     if ( cap_free( caps ) == -1 )
     printf( "5 CAP error %s\n", strerror_r( errno, buf, sizeof( buf ) ) );
     }
   */
  atexit( mas_atexit );

  runonce_config_create(  );
  runonce_pids_create(  );
  /* mas_runonce_display_create(  ); */
  return 0;
}

int
runonce_delete( void )
{
  mas_runonce_display_delete(  );
  runonce_pids_delete(  );
  runonce_config_delete(  );
  return 0;
}
