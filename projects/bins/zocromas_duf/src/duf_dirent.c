#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <mastar/wrap/mas_memory.h>
#include "duf_def.h"
#include "duf_utils.h"


#include "duf_def.h"
#include "duf_dirent.h"


int
direntry_filter( const struct dirent *de )
{
  int r = 0;

  switch ( de->d_type )
  {
  case DT_UNKNOWN:
    break;
  case DT_FIFO:
    break;
  case DT_CHR:
    break;
  case DT_DIR:
    if ( 0 != strcmp( de->d_name, ".comments" ) && 0 != strcmp( de->d_name, ".thumbnails" )
         && 0 != strcmp( de->d_name, ".xvpics" ) && 0 != strcmp( de->d_name, ".catalogimages" ) )
      r = 1;
    break;
  case DT_BLK:
    break;
  case DT_REG:
    /* fprintf( stderr, ">>>> %u : %lu : %s\n", de->d_type, de->d_ino, de->d_name ); */
    r = 1;
    break;
  case DT_LNK:
    break;
  case DT_SOCK:
    break;
  case DT_WHT:
    break;
  }
  return r;
}
