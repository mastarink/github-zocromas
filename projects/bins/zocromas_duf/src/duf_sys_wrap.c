#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"



/* ###################################################################### */
#include "duf_sys_wrap.h"
/* ###################################################################### */


int
duf_unlink( const char *path )
{
  DEBUG_STARTR( r );
  if ( path )
  {
    DUF_TRACE( any, 0, "removing %s ...", path );
    r = unlink( path );
    if ( r < 0 )
    {
      char *s;
      char serr[1024];

      s = strerror_r( errno, serr, sizeof( serr ) );

      DUF_SHOW_ERROR( "unlink %s: [%s]", path, s );
      if ( errno == ENOENT )
        r = 0;
      else
        DOR( r, DUF_ERROR_UNLINK );
    }
  }
  DEBUG_ENDR( r );
}
