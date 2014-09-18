#include <dirent.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_match.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_dirent.h"
/* ###################################################################### */

int
duf_direntry_filter( const struct dirent *de )
{
  int r = 0;

  DEBUG_START(  );
  switch ( de->d_type )
  {
  case DT_UNKNOWN:
    break;
  case DT_FIFO:
    break;
  case DT_CHR:
    break;
  case DT_DIR:
    r = ( 1
          && ( 0 != strcmp( de->d_name, "." ) )
          && ( 0 != strcmp( de->d_name, ".." ) )
          && ( 0 != strcmp( de->d_name, ".comments" ) )
          && ( 0 != strcmp( de->d_name, ".thumbnails" ) )
          && ( 0 != strcmp( de->d_name, ".xvpics" ) ) && ( 0 != strcmp( de->d_name, ".catalogimages" ) ) );
    break;
  case DT_BLK:
    break;
  case DT_REG:

    r = duf_filename_match( &duf_config->pu->globx, de->d_name );

/* #include <fnmatch.h>                                                   */
/*     int fnmatch( const char *pattern, const char *string, int flags ); */


    break;
  case DT_LNK:
    break;
  case DT_SOCK:
    break;
  case DT_WHT:
    break;
  }
  DEBUG_END( );
  return r;
}
