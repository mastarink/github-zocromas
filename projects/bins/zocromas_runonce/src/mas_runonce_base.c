#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/capability.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_pid.h"

#include "mas_runonce_config.h"
#include "mas_runonce_config_group.h"
#include "mas_runonce_config_section.h"

#include "mas_runonce_base.h"

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

  runonce_pids_create(  );
  runonce_config_create(  );
  return 0;
}

int
runonce_delete( void )
{
  runonce_pids_delete(  );
  runonce_config_delete(  );
  return 0;
}
