/* #undef MAS_TRACING */
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"


/* ###################################################################### */
#include "duf_sys_wrap.h"
/* ###################################################################### */


int
duf_unlink( const char *path )
{
  DUF_STARTR( r );
  if ( path )
  {
    int ry = 0;

    DUF_TRACE( any, 0, "removing %s ...", path );
    ry = unlink( path );
    if ( ry < 0 )
    {
      char *s;
      char serr[1024];

      s = strerror_r( errno, serr, sizeof( serr ) );

      /* DUF_SHOW_ERRORO( "unlink %s: [%s]", path, s ); */
      if ( errno != ENOENT )
        DUF_MAKE_ERRORM( r, DUF_ERROR_UNLINK, "unlink %s: [%s]", path, s );
    }
  }
  DUF_ENDR( r );
}
