#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_xtended_table.h"
#include "duf_option_stage.h"

/* ###################################################################### */
#include "duf_option_find.h"
/* ###################################################################### */

#define  DUF_NO_PREFIX "no-"

/* TODO: rename => duf_codeval2extended */
/* codeval => extended, by standard multi-table */
const duf_longval_extended_t *
duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_table_t ** result_pxtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

#if 0
  int ok;
#endif
  int rpr = 0;
  int ntable = 0;
  int tbcount = 0;

  if ( codeval && codeval != '?' )
  {
    for ( const duf_longval_extended_table_t ** multix = duf_extended_table_multi(  ); !rxtended && *multix; multix++, ntable++ )
    {
      const duf_longval_extended_table_t *xtable = *multix;

      for ( const duf_longval_extended_t * xarr = xtable->table; !rxtended && xarr->o.name; xarr++, tbcount++ )
      {
        if ( xarr )
        {
          DUF_TRACE( findopt, +1, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xarr->o.name, xarr->o.val, codeval );
          if ( xarr->o.val == codeval )
          {
            rxtended = xarr;
            if ( result_pxtable )
              *result_pxtable = xtable;
            DUF_TRACE( findopt, +1, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xarr->o.name );
#if 0
            ok = 1;
#endif
            break;              /* ? */
          }
        }
      }
    }
    DUF_TRACE( findopt, +1, "@li2ex ? %d:%d [%s]", ntable, tbcount, rxtended ? rxtended->o.name : NULL );
  }
#if 0
  if ( !ok )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
#endif
  if ( pr )
    *pr = rpr;
  return rxtended;
}

/* return so called `longindex` */
/* name + xtended_array => extended ; duf_noption_xfind */
static const duf_longval_extended_t *
duf_noption_xfind_exact_at_xarr( const char *name, int witharg_unused DUF_UNUSED, const duf_longval_extended_t * xarr, int *pcnt /*, int *pr */  )
{
  const duf_longval_extended_t *extended = NULL;

#if 0
  int ok = 0;
  int rpr = 0;
#endif

  if ( name && *name )
    for ( ; xarr->o.name; xarr++ )
    {
      DUF_TRACE( findopt, +1, "compare name:`%s` ? `%s`", xarr->o.name, name );
      if ( 0 == strcmp( name, xarr->o.name ) )
      {
        DUF_TRACE( findopt, +1, "found name:`%s`", xarr->o.name );
#if 0
        ok = 1;
#endif
        if ( pcnt )
          ( *pcnt )++;
        extended = xarr;
        break;
      }
    }
#if 0
  if ( !ok )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
  if ( pr )
    *pr = rpr;
#endif
  return extended;
}

/* return so called `longindex`
 *
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *
 * */
static const duf_longval_extended_t *
duf_noption_xfind_soft_at_xarr( const char *name, int witharg, const duf_longval_extended_t * xarr, int *pcnt, int *pr )
{
  int rpr = 0;
  int cnt = 0;
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_t *extended_soft = NULL;
  const duf_longval_extended_t *extended_exact = NULL;
  int dbgcnt = 0;

  if ( name && *name )
    for ( ; xarr->o.name; xarr++, dbgcnt++ )
    {
      int l;

      l = strlen( name );
      DUF_TRACE( findopt, +1, "compare name:`%s` ? `%s`", xarr->o.name, name );
      if ( 0 == strncmp( name, xarr->o.name, l ) )
      {
        int ha;
        const char *tname;

        DUF_TRACE( findopt, +1, "@by %s match name:`%s` (len:%d)", name, xarr->o.name, l );
        tname = xarr->o.name;
        ha = xarr->o.has_arg;
        if ( ( witharg && ha == required_argument ) || ( !witharg && ha == no_argument ) || ha == optional_argument )
        {
          if ( !extended_soft ) /* find first!? */
          {
            extended_soft = xarr;
          }
          if ( !tname[l] && !extended_exact )
          {
            extended_exact = xarr;
            DUF_TRACE( findopt, +1, "@by %s found exact name:`%s`", name, xarr->o.name );
          }
          cnt++;
        }
      }
    }
  if ( cnt > 1 )
  {
    if ( extended_exact )       /* several soft but one(first?) exact */
    {
      extended = extended_exact;
      /* cnt = 1; */
    }
    else
      DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_MULTIPLE );
  }
  else if ( cnt == 1 && extended_exact )
  {
    extended = extended_exact;
    /* DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_MULTIPLE ); XXX ?? XXX */
  }
  else if ( cnt == 1 && extended_soft )
  {
    extended = extended_soft;
  }
#if 0
  if ( !extended && DUF_NOERROR( rpr ) )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
#endif
  if ( extended )
    DUF_TRACE( findopt, +1, "@@@(%s) by %s found (%d;exact:%d) name:`%s`", mas_error_name_i( rpr ), name, cnt, extended_exact?1:0, extended->o.name );
  if ( pcnt )
    *pcnt = cnt;
  if ( pr )
    *pr = rpr;
  return extended;
}

/*
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *   */
static const duf_longval_extended_t *
duf_noption_xfind_at_xarr( const char *name, int witharg, const duf_longval_extended_t * xarr, int soft, int *pcnt, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  DUF_TRACE( findopt, 7, "@@@@to find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    extended = duf_noption_xfind_soft_at_xarr( name, witharg, xarr, pcnt, &rpr );
  else
    extended = duf_noption_xfind_exact_at_xarr( name, witharg, xarr, pcnt /*, &rpr */  );
  if ( extended )
    DUF_TRACE( findopt, 3, "@(%s) soft:%d; by %s found name:`%s`", mas_error_name_i( pr ? *pr : -1 ), soft, name, extended->o.name );
  if ( pr )
    *pr = rpr;
  return extended;
}

/*
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *   */
const duf_longval_extended_t *
duf_noption_xfind_no_at_xarr( const char *name, int witharg, const duf_longval_extended_t * xarr, int soft, int *pno, int *pcnt, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  extended = duf_noption_xfind_at_xarr( name, witharg, xarr, soft, pcnt, &rpr );
  DUF_TRACE( findopt, 7, "@@name:%s; extended:%d", name, extended ? 1 : 0 );
  if ( extended )
    DUF_TRACE( findopt, 3, "@(%s) by %s found name:`%s`", mas_error_name_i( rpr ), name, extended->o.name );

  DUF_TRACE( findopt, 6, "name:%s; witharg:%d; soft:%d", name, witharg, soft );
  if ( pno && DUF_IS_ERROR_N( rpr, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
    *pno = 0;
    DUF_TRACE( findopt, +1, "(try 'no') name:%s; witharg:%d; soft:%d", name, witharg, soft );
    if ( 0 == strncmp( name, DUF_NO_PREFIX, strlen( DUF_NO_PREFIX ) ) && strlen( name ) > strlen( DUF_NO_PREFIX ) )
    {
      /* DUF_CLEAR_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND ); (* DUF_ERROR_OPTION_NOT_FOUND impossible here !?  *) */
      extended = duf_noption_xfind_at_xarr( name + strlen( DUF_NO_PREFIX ), witharg, xarr, soft, pcnt, &rpr );
      if ( extended )
        DUF_TRACE( findopt, 3, "@(%s) by %s found name:`%s`", mas_error_name_i( rpr ), name, extended->o.name );
      *pno = 1;
    }
  }
  if ( extended )
    DUF_TRACE( findopt, 4, "@(%s) by %s found name:`%s`", mas_error_name_i( rpr ), name, extended->o.name );
  if ( pr )
    *pr = rpr;
  return extended;
}
/* string2extended */
 const duf_longval_extended_t *
duf_soption_xfind_at_xarr( const char *string, const duf_longval_extended_t * xarr, char vseparator, char **parg, int *pno, int *pcnt, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int rpr = 0;
  char *barg = NULL;
  char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;

  switch ( vseparator )
  {
  case ' ':
    barg = endn = strpbrk( string, "\t\r\n " );
    while ( barg && *barg && strchr( "\t\r\n ", *barg ) )
      barg++;
    break;
  case 0:
    barg = endn = strpbrk( string, "\t\r\n =" );
    while ( barg && *barg && strchr( "\t\r\n =", *barg ) )
      barg++;
    break;
  default:
    barg = endn = strchr( string, '=' );
    if ( barg )
      barg++;
    break;
  }
  if ( endn )
    name = mas_strndup( string, endn - string );
  else
    name = mas_strdup( string );
  if ( barg )
  {
    /* arg = mas_strdup( barg ); */
    /* T( ">>>> barg:'%s'", barg ); */
    arg = duf_string_options_expand( barg, '?' );
    /* T( ">> arg:'%s' => %s", barg, arg ); */
  }
  DUF_TRACE( options, +180, "vseparator:'%c'; name:`%s`; arg:`%s`", vseparator, name, arg );

  extended = duf_noption_xfind_no_at_xarr( name, arg ? 1 : 0, xarr, 1 /* soft */ , pno, pcnt, &rpr );
  if ( extended )
    DUF_TRACE( findopt, 5, "@(%s) by %s found cmd for %s", mas_error_name_i( rpr ), string, extended->o.name );
  if ( DUF_NOERROR( rpr ) && parg )
    *parg = arg;
  else
    mas_free( arg );
  arg = NULL;

  mas_free( name );
  /* if ( extended )                                                                                               */
  /*   DUF_TRACE( findopt, 5, "@@@@(verb:%d) found name:`%s`", DUF_CONFIGG( opt.dbg.verbose ), extended->o.name ); */
  /* else                                                                                                          */
  /*   DUF_TRACE( findopt, 5, "@@@@(verb:%d) found name:`%s`", DUF_CONFIGG( opt.dbg.verbose ), name );             */
  if ( pr )
    *pr = rpr;
  return extended;
}


