#include <stdlib.h>


/* ########################################################################################## */
#include "mas_convert.h"
/* ########################################################################################## */

long
mas_strtol_suff( const char *s, int *pr )
{
  int rpr = 0;
  long l = 0;
  char *pe = NULL;

  if ( s )
  {
    l = strtol( s, &pe, 10 );
    if ( pe )
    {
      switch ( *pe )
      {
      case 'G':
        l *= 1024 * 1024 * 1024;
        break;
      case 'M':
        l *= 1024 * 1024;
        break;
      case 'k':
        l *= 1024;
        break;
      case 'w':
        l *= 2;
        break;
      case 'c':
      case '\0':
        break;
      case 'b':
        l *= 512;
        break;
      default:
        rpr = -1;
        l = 0;
        break;
      }
    }
    else
    {
      rpr = -1;
      l = 0;
    }
  }
  if ( pr )
    *pr = rpr;
  return l;
}

long long
mas_strtoll_suff( const char *s, int *pr )
{
  int rpr = 0;
  long l = 0;
  char *pe = NULL;

  if ( s )
  {
    l = strtoll( s, &pe, 10 );
    if ( pe )
    {
      switch ( *pe )
      {
      case 'G':
        l *= 1024 * 1024 * 1024;
        break;
      case 'M':
        l *= 1024 * 1024;
        break;
      case 'k':
        l *= 1024;
        break;
      case 'w':
        l *= 2;
        break;
      case 'c':
      case '\0':
        break;
      case 'b':
        l *= 512;
        break;
      default:
        rpr = -1;
        l = 0;
        break;
      }
    }
    else
    {
      rpr = -1;
      l = 0;
    }
  }
  if ( pr )
    *pr = rpr;
  return l;
}
