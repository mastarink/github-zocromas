#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "tools.h"

char *
mucs_unquote( const char *string, const char *quotes )
{
  char *dststring = NULL, *dstp;
  const char *p;
  int in_q = 0;
  size_t len = strlen( string );

  dststring = mas_malloc( len );
  memset( dststring, 0, len );
  dstp = dststring;
  p = string;
  while ( p && *p )
  {
    if ( strchr( quotes, *p ) )
    {
      if ( in_q )
      {
        if ( in_q == *p )
        {
          in_q = 0;
          p++;
          continue;
        }
      }
      else
      {
        in_q = *p++;
        continue;
      }
    }
    else if ( *p == '\\' && ( ( !in_q && strchr( quotes, *p ) ) || ( ( in_q && p[1] == in_q ) ) ) )
    {
      p++;
    }
    *dstp++ = *p++;
  }
  if ( in_q )
  {
    mas_free( dststring );
    dststring = mas_strdup( string );
  }
  else
  {
    char *t = dststring;

    dststring = mas_strdup( t );
    mas_free( t );
  }
  return dststring;
}
