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

#define  DUF_NO_PREFIX "no-"

/* static int                                                                                                        */
/* duf_find_codeval_long( duf_option_code_t codeval, const duf_longval_extended_t * xtable )                         */
/* {                                                                                                                 */
/*   DEBUG_STARTR( r );                                                                                              */
/*                                                                                                                   */
/*   r = DUF_ERROR_OPTION_NOT_FOUND;                                                                                 */
/*   DUF_TRACE( explain, 2, "to find codeval : %d (%c)", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?' ); */
/*   for ( int ilong = 0; codeval && xtable[ilong].o.name; ilong++ )                                                 */
/*   {                                                                                                               */
/*     if ( xtable[ilong].o.val == codeval )                                                                         */
/*     {                                                                                                             */
/*       DUF_TRACE( explain, 2, "%4d. find codeval codeval: %d ? %d", ilong, codeval, xtable[ilong].o.val );         */
/*       r = ilong;                                                                                                  */
/*       break;                                                                                                      */
/*     }                                                                                                             */
/*   }                                                                                                               */
/*   DEBUG_ENDRN( r );                                                                                               */
/* }                                                                                                                 */
/*                                                                                                                   */
/* static int                                                                                                        */
/* duf_find_codeval_long_xtables( duf_option_code_t codeval, const duf_longval_extended_t * xtables[] )              */
/* {                                                                                                                 */
/*   DEBUG_STARTR( r );                                                                                              */
/*   r = DUF_ERROR_OPTION_NOT_FOUND;                                                                                 */
/*                                                                                                                   */
/*   const duf_longval_extended_t *xtable;                                                                           */
/*                                                                                                                   */
/*   while ( ( xtable = *xtables++ ) )                                                                               */
/*   {                                                                                                               */
/*     r = duf_find_codeval_long( codeval, xtable );                                                                 */
/*     if ( r >= 0 )                                                                                                 */
/*       break;                                                                                                      */
/*     xtable++;                                                                                                     */
/*   }                                                                                                               */
/*   DEBUG_ENDRN( r );                                                                                               */
/* }                                                                                                                 */
/*                                                                                                                   */
/* static int                                                                                                        */
/* duf_find_codeval_long_std( duf_option_code_t codeval )                                                            */
/* {                                                                                                                 */
/*   DEBUG_STARTR( r );                                                                                              */
/*   r = duf_find_codeval_long_xtables( codeval, lo_extended_multi );                                                */
/*   DEBUG_ENDRN( r );                                                                                               */
/* }                                                                                                                 */

const duf_longval_extended_t *
duf_find_codeval_extended_std( duf_option_code_t codeval, int *pr )
{
  int r = 0;

  const duf_longval_extended_t *extended = NULL;
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_t ** xtables = lo_extended_multi; *xtables; xtables++, ntable++ )
  {
    for ( const duf_longval_extended_t * xtable = *xtables; xtable->o.name; xtable++, tbcount++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d:%d [%s]", ntable, tbcount, xtable->o.name );
      if ( xtable  && xtable->o.val == codeval )
        extended = xtable;
    }
  }

  /* const duf_longval_extended_t *extended = NULL;  */
  /*                                                 */
  /* DOR( r, duf_find_codeval_long_std( codeval ) ); */
  /* if ( r >= 0 )                                   */
  /*   extended = duf_longindex2extended( r, &r );   */
  if ( pr )
    *pr = r;
  return extended;
}

/* return so called `longindex` */
static int
duf_find_name_long_exact( const char *name, int witharg, const duf_longval_extended_t * xtable )
{
  DEBUG_STARTR( r );
  r = DUF_ERROR_OPTION_NOT_FOUND;
  if ( name && *name )
    for ( int ilong = 0; xtable[ilong].o.name; ilong++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s` at %d", xtable[ilong].o.name, name, ilong );
      if ( 0 == strcmp( name, xtable[ilong].o.name ) )
      {
        DUF_TRACE( options, 3, "found name:`%s` at %d", xtable[ilong].o.name, ilong );
        r = ilong;
        break;
      }
    }
  DEBUG_ENDRN( r );
}

/* return so called `longindex` */
static int
duf_find_name_long_soft( const char *name, int witharg, const duf_longval_extended_t * xtable )
{
  DEBUG_STARTR( r );
  int cnt = 0;
  int rexact;

  r = rexact = DUF_ERROR_OPTION_NOT_FOUND;
  if ( name && *name )
    for ( int ilong = 0; xtable[ilong].o.name; ilong++ )
    {
      int l;

      l = strlen( name );
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s` at %d", xtable[ilong].o.name, name, ilong );
      if ( 0 == strncmp( name, xtable[ilong].o.name, l ) )
      {
        int ha;
        const char *tname;

        tname = xtable[ilong].o.name;
        ha = xtable[ilong].o.has_arg;
        if ( ( witharg && ha == required_argument ) || ( !witharg && ha == no_argument ) || ha == optional_argument )
        {
          if ( r < 0 )
            r = ilong;
          if ( !tname[l] )
            rexact = ilong;
          DUF_TRACE( options, 0, "@@found name:`%s` at %d", tname, ilong );
          cnt++;
        }
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

/* return so called `longindex` */
int
duf_find_name_long( const char *name, int witharg, const duf_longval_extended_t * xtable, int soft )
{
  DEBUG_STARTR( r );
  DUF_TRACE( options, 2, " find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    DOR( r, duf_find_name_long_soft( name, witharg, xtable ) );
  else
    DOR( r, duf_find_name_long_exact( name, witharg, xtable ) );
  DEBUG_ENDRN( r );
}

/* return so called `longindex` */
int
duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtable, int soft, int *pno )
{
  DEBUG_STARTR( r );

  r = duf_find_name_long( name, witharg, xtable, soft );
  DUF_TRACE( options, 4, "name:%s; witharg:%d; soft:%d", name, witharg, soft );
  if ( pno && r == DUF_ERROR_OPTION_NOT_FOUND )
  {
    *pno = 0;
    DUF_TRACE( options, 4, "(try 'no') name:%s; witharg:%d; soft:%d", name, witharg, soft );
    if ( r == DUF_ERROR_OPTION_NOT_FOUND && 0 == strncmp( name, DUF_NO_PREFIX, strlen( DUF_NO_PREFIX ) ) && strlen( name ) > strlen( DUF_NO_PREFIX ) )
    {
      r = duf_find_name_long( name + strlen( DUF_NO_PREFIX ), witharg, xtable, soft );
      *pno = 1;
    }
  }
  DEBUG_ENDRN( r );
}
