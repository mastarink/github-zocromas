#include <string.h>
#include <dlfcn.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>


#include "duf_maintenance.h"

#include "duf_config.h"

#include "duf_sccbh_shortcuts.h"
#include "std_mod_sets.h"

/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */

/* TODO rename _duf_uni_scan_action_title => _duf_sccb_title */
static const char *
_duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  const char *stitle;

  DUF_START(  );
  if ( sccb && sccb->title )
  {
    stitle = strrchr( sccb->title, '/' );
    if ( stitle )
      stitle++;
    else
      stitle = sccb->title;
  }
  else
  {
    stitle = "";
  }
  DUF_ENDCS( stitle );
}

/* TODO rename duf_uni_scan_action_title => duf_sccb_title */
const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", _duf_uni_scan_action_title( sccb ) );
  return tbuf;
}

const duf_scan_callbacks_t *
duf_find_sccb_by_evnamen( const char *name, size_t namelen, const duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;
  char *n;

  n = mas_strndup( name, namelen );
  for ( sccb = first; sccb; sccb = sccb->next )
  {
    if ( 0 == strcmp( n, sccb->name ) )
      break;
  }
  mas_free( n );
  return sccb;
}

const duf_scan_callbacks_t *
duf_load_sccb_by_evnamen( const char *name, size_t namelen, duf_scan_callbacks_t * first )
{
  duf_scan_callbacks_t *sccb = NULL;

  if ( namelen && name && first )
  {
    duf_scan_callbacks_t *prev = NULL;
    char *path = NULL;
    char *symbol = NULL;

    path = mas_normalize_path_plus( MAS_LIBDIR, "dufmod", NULL );
    path = mas_strncat_x( path, name, namelen );
    path = mas_strcat_x( path, ".so" );
    symbol = mas_strdup( "duf_" );
    symbol = mas_strncat_x( symbol, name, namelen );
    symbol = mas_strcat_x( symbol, "_callbacks" );
    {
      void *han = NULL;

      DUF_TRACE( sccb, 0, "@@@@@@to load %s", path );
      han = dlopen( path, RTLD_NOLOAD | RTLD_LAZY );
      if ( !han )
      {
        han = dlopen( path, RTLD_LOCAL | RTLD_LAZY );
        if ( han )
        {
          sccb = ( duf_scan_callbacks_t * ) dlsym( han, symbol );
          if ( sccb )
          {
            sccb->dlhan = han;
            DUF_TRACE( sccb, 0, "[%p] %s => %s", han, path, sccb->name );
            for ( duf_scan_callbacks_t * tsccb = first; tsccb; tsccb = tsccb->next )
              prev = tsccb;
            if ( prev )
            {
              prev->next = sccb;
              DUF_TRACE( sccb, 0, "[han:%p] : %s", han, sccb ? sccb->name : NULL );
            }
            else
            {
              /* dlclose ?? */
            }
          }
        }
      }
      DUF_TRACE( sccb, 0, "[han:%p] %s : %s", han, symbol, sccb ? sccb->name : NULL );
    }
    mas_free( symbol );
    mas_free( path );
  }
  return sccb;
}

const duf_scan_callbacks_t *
duf_find_or_load_sccb_by_evnamen( const char *name, size_t namelen, duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;

  sccb = duf_find_sccb_by_evnamen( name, namelen, first );
  if ( !sccb )
  {
    sccb = duf_load_sccb_by_evnamen( name, namelen, first );
    DUF_TRACE( sccb, 0, "loaded %s", sccb ? sccb->name : NULL );
  }
  return sccb;
}

const duf_scan_callbacks_t *
duf_find_sccb_by_evname( const char *name, const duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;

  sccb = duf_find_sccb_by_evnamen( name, strlen( name ), first );
  return sccb;
}

const duf_scan_callbacks_t *
duf_find_or_load_sccb_by_evname( const char *name, duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;

  sccb = duf_find_or_load_sccb_by_evnamen( name, strlen( name ), first );
  return sccb;
}
