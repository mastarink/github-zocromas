#include <string.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_li.h"
/* ###################################################################### */
#ifndef DUF_NO_NUMS
void
duf_li_set_nums( duf_levinfo_t * pli, unsigned long long ndirs, unsigned long long nfiles )
{
  if ( pli )
  {
    pli->numdir = ndirs;
    pli->numfile = nfiles;
    /* T("@@@@ %s>>%llu", pli->itemname, ndirs ); */
  }
}
#else
#  if 0
void
duf_li_set_childs( duf_levinfo_t * pli, unsigned long long childs )
{
  if ( pli )
  {
    pli->childs = childs;
    /* pli->numchild = childs; */
  }
}
#  endif
#endif

int
duf_li_count( const duf_levinfo_t * pli )
{
  int cnt = 0;

  while ( pli && pli->itemname )
  {
    cnt++;
    pli++;
  }
  return cnt;
}

char *
duf_li_path( const duf_levinfo_t * pli, int count )
{
  char *path = NULL;

  {
    size_t len = 2;
    char *p;

    assert( count > 0 );
    for ( int i = 0; i < count; i++ )
    {
      assert( pli[i].itemname );
      len += strlen( pli[i].itemname ) + 1;
    }
    path = mas_malloc( len );
    p = path;

    for ( int i = 0; i < count; i++ )
    {
      size_t l;

      if ( p == path || *( p - 1 ) != '/' )
        *p++ = '/';
      *p = 0;
      DUF_TRACE( path, 4, "path:%s", path );
      l = strlen( pli[i].itemname );
      if ( l > 0 )
      {
        strcpy( p, pli[i].itemname );
        p += l;
        *p++ = '/';
      }
      *p = 0;
    }
  }
  return path;
}
