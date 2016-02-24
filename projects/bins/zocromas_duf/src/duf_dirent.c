/* #undef MAS_TRACING */
#include <dirent.h>
#include <string.h>

#include "duf_maintenance.h"

#include "duf_config.h"


#include "duf_match.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

/* ###################################################################### */
#include "duf_dirent.h"
/* ###################################################################### */

int
duf_direntry_filter( const struct dirent *de )
{
  int rx = 0;

  DUF_START(  );
  switch ( de->d_type )
  {
  case DT_UNKNOWN:
    break;
  case DT_FIFO:
    break;
  case DT_CHR:
    break;
  case DT_DIR:
    rx = ( 1                    /* */
#if 0
           && !( de->d_name[0] == '.' && !de->d_name[1] ) /* */
           && !( de->d_name[0] == '.' && de->d_name[1] == '.' && !de->d_name[2] ) /* */
#else
           && ( 0 != strcmp( de->d_name, "." ) ) /* */
           && ( 0 != strcmp( de->d_name, ".." ) ) /* */
#endif
#if 0
           && ( 0 != strcmp( de->d_name, ".comments" ) ) /* */
           && ( 0 != strcmp( de->d_name, ".thumbnails" ) ) /* */
           && ( 0 != strcmp( de->d_name, ".xvpics" ) ) /* */
           && ( 0 != strcmp( de->d_name, ".catalogimages" ) ) /* */
#endif
           );
    break;
  case DT_BLK:
    break;
  case DT_REG:

    rx = duf_filename_match( DUF_CONFIGA( vars.puz->globx ), de->d_name );

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
  DUF_END(  );
  return rx;
}
