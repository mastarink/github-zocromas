#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_find.h"
/* ###################################################################### */


int
duf_find_codeval_long( duf_option_code_t codeval, const duf_longval_extended_t * xtable, unsigned xtable_size )
{
  DEBUG_STARTR( r );

  r = DUF_ERROR_OPTION;
  assert( xtable_size );
  DUF_TRACE( explain, 2, "to find codeval : %d (%c)", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?' );
  r = DUF_ERROR_OPTION;
  for ( int ilong = 0; codeval && ilong < xtable_size; ilong++ )
  {
    if ( xtable[ilong].o.val == codeval )
    {
      DUF_TRACE( explain, 2, "%4d. find codeval codeval: %d ? %d", ilong, codeval, xtable[ilong].o.val );
      r = ilong;
      break;
    }
  }
  DEBUG_ENDRN( r );
}

int
duf_find_codeval_long_std( duf_option_code_t codeval )
{
  DEBUG_STARTR( r );
  r = duf_find_codeval_long( codeval, lo_extended, lo_extended_count );
  DEBUG_ENDRN( r );
}

const duf_longval_extended_t *
duf_find_codeval_extended_std( duf_option_code_t codeval, int *pr )
{
  int r = 0;
  const duf_longval_extended_t *extended = NULL;

  r = duf_find_codeval_long_std( codeval );
  if ( r >= 0 )
    extended = duf_longindex_extended( r );

  if ( pr )
    *pr = r;
  return extended;
}

int
duf_find_name_long_exact( const char *name, const duf_longval_extended_t * xtable, unsigned xtable_size )
{
  DEBUG_STARTR( r );
  r = DUF_ERROR_OPTION;
  if ( name && *name )
    for ( int ilong = 0; xtable[ilong].o.name && ilong < xtable_size; ilong++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s` at %d", xtable[ilong].o.name, name, ilong );
      if ( 0 == strcmp( name, xtable[ilong].o.name ) )
      {
        DUF_TRACE( options, 0, "found name:`%s` at %d", xtable[ilong].o.name, ilong );
        r = ilong;
        break;
      }
    }
  DEBUG_ENDRN( r );
}

int
duf_find_name_long_soft( const char *name, const duf_longval_extended_t * xtable, unsigned xtable_size )
{
  DEBUG_STARTR( r );
  int cnt = 0;
  int rexact;

  rexact = DUF_ERROR_OPTION;
  r = DUF_ERROR_OPTION;
  if ( name && *name )
    for ( int ilong = 0; xtable[ilong].o.name && ilong < xtable_size; ilong++ )
    {
      int l;

      l = strlen( name );
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s` at %d", xtable[ilong].o.name, name, ilong );
      if ( 0 == strncmp( name, xtable[ilong].o.name, l ) )
      {
        if ( r < 0 )
          r = ilong;
        if ( !xtable[ilong].o.name[l] )
          rexact = ilong;
        DUF_TRACE( options, 0, "found name:`%s` at %d", xtable[ilong].o.name, ilong );
        cnt++;
      }
    }
  if ( cnt > 1 )
  {
    if ( rexact >= 0 )
      r = rexact;
    else
      r = DUF_ERROR_OPTION_MULTIPLE;
  }
  DEBUG_ENDRN( r );
}

int
duf_find_name_long( const char *name, const duf_longval_extended_t * xtable, unsigned xtable_size, int soft )
{
  DEBUG_STARTR( r );
  if ( soft )
    r = duf_find_name_long_soft( name, xtable, xtable_size );
  else
    r = duf_find_name_long_exact( name, xtable, xtable_size );
  DEBUG_ENDRN( r );
}
