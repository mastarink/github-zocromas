#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo.h"

#include "masxfs_levinfo_format.h"

char *
masxfs_levinfo_prefix( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, masxfs_depth_t top_depth, int test )
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
    /* char *pw = NULL; */
    masxfs_levinfo_t *lia = masxfs_levinfo_li2lia( li );

    if ( test > 1 )
      len = 17;
    else if ( test )
      len = 17 /*9*/;
/*    pw = */prefix = mas_calloc( li->lidepth + 2, len );
    /* WARN( "%d", top_depth ); */
    for ( masxfs_depth_t d = top_depth; d < li->lidepth; d++ )
    {
      size_t child_count = lia[d].child_count_pair[1];
      size_t child_count_z = lia[d].child_count_pair[0];

/* WARN("%d - %ld : %ld", d, child_count, child_count_z); */
      unsigned delta = ( child_count_z - child_count ) > 0;
      unsigned deep = ( d == li->lidepth - 1 );
      unsigned cas = ( delta << 1 ) + deep;

      if ( /* !test && */child_count_z )
      {
//      if ( test )
        {
	  char buf[32];
          if ( test > 1 )
            sprintf( buf, "[%3ld %3ld %3ld %3ld]", child_count_z, child_count, ( long ) li->lidepth, ( long ) d );
          else if (test)
            sprintf( buf, "[%15d]", cas );
	  strcat(prefix, buf);
        }
//      else
        {
	  char *pp=NULL;
          switch ( cas )
          {	    
          case 0:
            pp = p1;
            break;
          case 1:
            pp = p2;
            break;
          case 2:
            pp = p3;
            break;
          case 3:
            pp = p4;
            break;
          }
          strcat( prefix, pp );
        }
      }
    }
  }
  return prefix;
}
