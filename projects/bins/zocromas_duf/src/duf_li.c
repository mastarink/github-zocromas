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
