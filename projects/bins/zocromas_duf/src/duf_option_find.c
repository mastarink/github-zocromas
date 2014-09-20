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

const duf_longval_extended_t *
duf_find_codeval_extended_std( duf_option_code_t codeval, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_t ** xtables = lo_extended_multi; !extended && *xtables; xtables++, ntable++ )
  {
    for ( const duf_longval_extended_t * xtable = *xtables; !extended && xtable->o.name; xtable++, tbcount++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d:%d [%s]", ntable, tbcount, xtable->o.name );
      if ( xtable && xtable->o.val == codeval )
      {
        extended = xtable;
        r = 0;
        break;
      }
    }
  }

  if ( pr )
    *pr = r;
  return extended;
}

/* return so called `longindex` */
static const duf_longval_extended_t *
duf_find_name_long_exact( const char *name, int witharg, const duf_longval_extended_t * xtable, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;

  r = DUF_ERROR_OPTION_NOT_FOUND;
  if ( name && *name )
    for ( ; xtable->o.name; xtable++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s`", xtable->o.name, name );
      if ( 0 == strcmp( name, xtable->o.name ) )
      {
        DUF_TRACE( options, 3, "found name:`%s`", xtable->o.name );
        r = 0;
        extended = xtable;
        if ( extended )
          DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
        break;
      }
    }
  if ( pr )
    *pr = r;
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
  return extended;
}

/* return so called `longindex` */
static const duf_longval_extended_t *
duf_find_name_long_soft( const char *name, int witharg, const duf_longval_extended_t * xtable, int *pr )
{
  int cnt = 0;
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_t *extended_soft = NULL;
  const duf_longval_extended_t *extended_exact = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;

  if ( name && *name )
    for ( ; xtable->o.name; xtable++ )
    {
      int l;

      l = strlen( name );
      DUF_TRACE( options, 8, "compare name:`%s` ? `%s`", xtable->o.name, name );
      if ( 0 == strncmp( name, xtable->o.name, l ) )
      {
        int ha;
        const char *tname;

        tname = xtable->o.name;
        ha = xtable->o.has_arg;
        if ( ( witharg && ha == required_argument ) || ( !witharg && ha == no_argument ) || ha == optional_argument )
        {
          if ( !extended_soft ) /* find first!? */
          {
            extended_soft = xtable;
            DUF_TRACE( options, 3, "@(%s) found soft name:`%s`", duf_error_name( r ), xtable->o.name );
          }
          if ( !tname[l] && !extended_exact )
          {
            extended_exact = xtable;
            DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
            DUF_TRACE( options, 3, "found exact name:`%s`", xtable->o.name );
          }
          cnt++;
        }
      }
    }
  if ( cnt > 1 )
  {
    if ( extended_exact )       /* several soft but one(first?) exact */
      extended = extended_exact;
    else
      r = DUF_ERROR_OPTION_MULTIPLE;
  }
  else if ( cnt == 1 && extended_exact )
    extended = extended_exact;
  else if ( cnt == 1 && extended_soft )
  {
    extended = extended_soft;
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  }
  if ( pr )
    *pr = r;
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
  return extended;
}

static const duf_longval_extended_t *
duf_find_name_long( const char *name, int witharg, const duf_longval_extended_t * xtable, int soft, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  DUF_TRACE( options, 6, "to find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    extended = duf_find_name_long_soft( name, witharg, xtable, pr );
  else
    extended = duf_find_name_long_exact( name, witharg, xtable, pr );
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( pr ? *pr : -1 ), extended->o.name );
  return extended;
}

const duf_longval_extended_t *
duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtable, int soft, int *pno, int *pr )
{
  int r = 0;
  const duf_longval_extended_t *extended = NULL;

  extended = duf_find_name_long( name, witharg, xtable, soft, &r );
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
  DUF_TRACE( options, 4, "name:%s; witharg:%d; soft:%d", name, witharg, soft );
  if ( pno && r == DUF_ERROR_OPTION_NOT_FOUND )
  {
    *pno = 0;
    DUF_TRACE( options, 4, "(try 'no') name:%s; witharg:%d; soft:%d", name, witharg, soft );
    if ( r == DUF_ERROR_OPTION_NOT_FOUND && 0 == strncmp( name, DUF_NO_PREFIX, strlen( DUF_NO_PREFIX ) ) && strlen( name ) > strlen( DUF_NO_PREFIX ) )
    {
      DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
      extended = duf_find_name_long( name + strlen( DUF_NO_PREFIX ), witharg, xtable, soft, &r );
      if ( extended )
        DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
      *pno = 1;
    }
  }
  if ( pr )
    *pr = r;
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
  return extended;
}
