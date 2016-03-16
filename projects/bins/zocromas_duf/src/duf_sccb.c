/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <dlfcn.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

/* #include "duf_config.h"                                              (* duf_get_config ✗ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sccbh_shortcuts.h"
#include "std_mod_sets.h"

/* Working with sccb structure */
/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */

/* TODO rename _duf_uni_scan_action_title => _duf_sccb_title */
static const char *
uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  const char *stitle = NULL;

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
  return stitle;
}

/* TODO rename duf_uni_scan_action_title => duf_sccb_title */
const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", uni_scan_action_title( sccb ) );
  return tbuf;
}

static const duf_scan_callbacks_t *
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

static const duf_scan_callbacks_t *
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

      MAST_TRACE( sccb, 0, "@@@@@@to load %s", path );
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
            MAST_TRACE( sccb, 0, "[%p] %s => %s", han, path, sccb->name );
            for ( duf_scan_callbacks_t * tsccb = first; tsccb; tsccb = tsccb->next )
              prev = tsccb;
            if ( prev )
            {
              prev->next = sccb;
              MAST_TRACE( sccb, 0, "[han:%p] : %s", han, sccb ? sccb->name : NULL );
            }
            else
            {
            /* dlclose ?? */
            }
          }
        }
      }
      MAST_TRACE( sccb, 0, "[han:%p] %s : %s", han, symbol, sccb ? sccb->name : NULL );
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
    MAST_TRACE( sccb, 0, "loaded %s", sccb ? sccb->name : NULL );
  }
  return sccb;
}

const duf_scan_callbacks_t **
duf_find_or_load_sccb_by_evnamen_plus( const char *name0, size_t namelen, duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t **psccb = NULL;
  size_t cnt = 0;
  char *name = NULL;
  const char *p;
  const char *sname;

  name = mas_strndup( name0, namelen );
  for ( cnt = 0, p = name; p < name + namelen && p && *p; p++, cnt++ )
  {
    /* QT( "@-- %lu: %s", cnt, p ); */
    sname = p;
    p = strchr( p, '+' );
    if ( !p )
      p = sname + strlen( sname );
    /* QT( "@++ %lu: %s ~ %lu", cnt, sname, p - sname ); */
  }
  if ( cnt )
  {
    cnt += 2;
    psccb = mas_malloc( sizeof( const duf_scan_callbacks_t ) * cnt );
    memset( psccb, 0, sizeof( const duf_scan_callbacks_t ) * cnt );
    for ( cnt = 0, p = name; p < name + namelen && p && *p; p++, cnt++ )
    {
      sname = p;
      p = strchr( p, '+' );
      if ( !p )
        p = sname + strlen( sname );
      psccb[cnt] = duf_find_or_load_sccb_by_evnamen( sname, p - sname, first );
      /* QT( "@!! %lu: %s ~ %lu = %p", cnt, sname, p - sname, psccb[cnt] ); */
    }
  }
  mas_free( name );
/* while ( p < name + namelen && p && *p ); */
  return psccb;
}

void
duf_sccb_dlclose( duf_scan_callbacks_t * first )
{
  for ( duf_scan_callbacks_t * sccb = first; sccb; sccb = sccb->next )
  {
    if ( sccb->dlhan )
      dlclose( sccb->dlhan );
    sccb->dlhan = NULL;
  }
}

const duf_scan_callbacks_t *
duf_find_sccb_by_evname( const char *name, const duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;

  sccb = duf_find_sccb_by_evnamen( name, strlen( name ), first );
  return sccb;
}

#if 0
static const duf_scan_callbacks_t *
duf_find_or_load_sccb_by_evname( const char *name, duf_scan_callbacks_t * first )
{
  const duf_scan_callbacks_t *sccb = NULL;

  sccb = duf_find_or_load_sccb_by_evnamen( name, strlen( name ), first );
  return sccb;
}
#endif
