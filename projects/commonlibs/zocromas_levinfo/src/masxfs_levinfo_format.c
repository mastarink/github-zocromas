#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
/* #include "masxfs_structs.h" */

/* #include "masxfs_levinfo_base.h" */
/* #include "masxfs_levinfo_io_dir.h" */

/* #include "masxfs_levinfo_path.h" */

#include "masxfs_levinfo.h"

#include "masxfs_levinfo_format.h"

char *
masxfs_levinfo_prefix( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, int test )
{
  char *prefix = NULL;

  if ( li )
  {
    size_t len1 = strlen( p1 );
    size_t len2 = strlen( p2 );
    size_t len3 = strlen( p3 );
    size_t len = len1;

    if ( len2 > len )
      len = len2;
    if ( len3 > len )
      len = len3;
    char *pw = NULL;
    masxfs_levinfo_t *lia = masxfs_levinfo_li2lia( li );

    if ( test > 1 )
      len = 17;
    else if ( test )
      len = 9;
    pw = prefix = mas_calloc( li->lidepth + 2, len );
    for ( masxfs_depth_t d = 0; d < li->lidepth; d++ )
    {
      size_t child_count = lia[d].child_count_pair[1];
      size_t child_count_z = lia[d].child_count_pair[0];

      unsigned delta = ( child_count_z - child_count ) > 0;
      unsigned deep = ( d == li->lidepth - 1 );
      unsigned cas = ( delta << 1 ) + deep;

      if ( !test && child_count_z )
      {
        if ( test )
        {
          if ( test > 1 )
            sprintf( pw, "[%3ld %3ld %3ld %3ld]", child_count_z, child_count, ( long ) li->lidepth, ( long ) d );
          else
            sprintf( pw, "[%15d]", cas );
          pw += len;
        }
        else
        {
          switch ( cas )
          {
          case 0:
            pw = p1;
            break;
          case 1:
            pw = p2;
            break;
          case 2:
            pw = p3;
            break;
          case 3:
            pw = p4;
            break;
          }
          strncat( prefix, pw, len );
        }
      }
    }
  }
  return prefix;
}
