/* #undef MAS_TRACING */
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */


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

    MAST_TRACE( any, 0, "removing %s ...", path );
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
