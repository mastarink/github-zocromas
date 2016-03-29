/* #undef MAS_TRACING */
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

/* ###################################################################### */
#include "duf_sys_wrap.h"                                            /* duf_unlink() ✗ */
/* ###################################################################### */

SR(OTHER,unlink, const char *path )
{
  /* DUF_STARTR( r ); */
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
        ERRMAKE_M( UNLINK, "unlink %s: [%s]", path, s );
    }
  }
  /* DUF_ENDR( r ); */
ER(OTHER,unlink, const char *path );
}
