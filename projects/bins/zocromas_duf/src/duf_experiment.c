
#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_experiment.h"
/* ###################################################################### */

extern const duf_config_t *duf_config4trace;
#if 0
int
duf_experiment( void )
{
  void *han;

  han = dlopen( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/admin/install/default/lib/dufmod/crc32.so", RTLD_NOLOAD | RTLD_LAZY );
  DUF_TRACE( temp, 0, "(so) han:%p", han );
  if ( han )
  {
    dlclose( han );
  }
  else
  {
    han = dlopen( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/admin/install/default/lib/dufmod/crc32.so", RTLD_GLOBAL | RTLD_LAZY );
    DUF_TRACE( temp, 0, "(so) han:%p", han );
    if ( han )
    {
      duf_scan_callbacks_t *ps;
      extern duf_scan_callbacks_t duf_collect_openat_crc32_callbacks;

      ps = ( duf_scan_callbacks_t * ) dlsym( han, "scancbs" );
      if ( ps )
      {
        ps->dlhan = han;
        DUF_TRACE( temp, 0, "[%p:%p] %s :: %s", han, ps, ( ps )->name, duf_collect_openat_crc32_callbacks.name );
      }
      dlclose( han );
    }
    else
    {
      DUF_TRACE( temp, 0, "%s", dlerror(  ) );
    }
  }


  return 0;
}
#endif
