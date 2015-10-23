#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_find.h"
/* ###################################################################### */

#define  DUF_NO_PREFIX "no-"

int
duf_check_stage( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;

  DUF_TRACE( options, 5, "checking stage; istage:%d xflag:%d xuse:%d xminmax:%d/%d", istage, extended->stage.flag, extended->use_stage,
             extended->stage.min, extended->stage.max );
  DUF_TRACE( options, 5, "checking stage; istage:%d tflag:%d tuse:%d tminmax:%d/%d", istage, xtable->stage.flag, xtable->use_stage,
             xtable->stage.min, xtable->stage.max );
  r0 = ( istage == DUF_OPTION_STAGE_ANY );
  r0 = r0 || extended->stage.flag;
  r1 = ( !extended->use_stage || ( extended->stage.min <= istage && extended->stage.max >= istage ) );
  r2 = ( !xtable || ( !xtable->use_stage || ( xtable->stage.min <= istage && xtable->stage.max >= istage ) ) );
  r0 = r0 || ( r1 && r2 );
  if ( ( extended->use_stage_mask && ( extended->stage_mask & ( 1 << istage ) ) )
       || ( xtable->use_stage_mask && ( xtable->stage_mask & ( 1 << istage ) ) ) )
  {
    r0 = 0;
  }

  DUF_TRACE( options, 5, "checked stage; istage:%d; r0:%d", istage, r0 );
  return r0;
}

/* TODO: rename => duf_codeval2extended */
const duf_longval_extended_t *
duf_find_codeval_extended_std( duf_option_code_t codeval, const duf_longval_extended_table_t ** pxtable, int *pr )
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
    for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); !rxtended && *xtables; xtables++, ntable++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      for ( const duf_longval_extended_t * xtended = xtable->table; !rxtended && xtended->o.name; xtended++, tbcount++ )
      {
        if ( xtended )
        {
          DUF_TRACE( findopt, 5, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xtended->o.name, xtended->o.val, codeval );
          if ( xtended->o.val == codeval )
          {
            rxtended = xtended;
            if ( pxtable )
              *pxtable = xtable;
            DUF_TRACE( findopt, 5, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xtended->o.name );
#if 0
            ok = 1;
#endif
            break;              /* ? */
          }
        }
      }
    }
    DUF_TRACE( findopt, 5, "@li2ex ? %d:%d [%s]", ntable, tbcount, rxtended ? rxtended->o.name : NULL );
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
static const duf_longval_extended_t *
duf_find_name_long_exact( const char *name, int witharg_unused DUF_UNUSED, const duf_longval_extended_t * xtended, int *pcnt /*, int *pr */  )
{
  const duf_longval_extended_t *extended = NULL;

#if 0
  int ok = 0;
  int rpr = 0;
#endif

  if ( name && *name )
    for ( ; xtended->o.name; xtended++ )
    {
      DUF_TRACE( findopt, 4, "compare name:`%s` ? `%s`", xtended->o.name, name );
      if ( 0 == strcmp( name, xtended->o.name ) )
      {
        DUF_TRACE( findopt, 3, "found name:`%s`", xtended->o.name );
#if 0
        ok = 1;
#endif
        if ( pcnt )
          ( *pcnt )++;
        extended = xtended;
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
duf_find_name_long_soft( const char *name, int witharg, const duf_longval_extended_t * xtended, int *pcnt, int *pr )
{
  int rpr = 0;
  int cnt = 0;
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_t *extended_soft = NULL;
  const duf_longval_extended_t *extended_exact = NULL;
  int dbgcnt = 0;

  if ( name && *name )
    for ( ; xtended->o.name; xtended++, dbgcnt++ )
    {
      int l;

      l = strlen( name );
      DUF_TRACE( findopt, 8, "compare name:`%s` ? `%s`", xtended->o.name, name );
      if ( 0 == strncmp( name, xtended->o.name, l ) )
      {
        int ha;
        const char *tname;

        DUF_TRACE( findopt, 3, "@by %s match name:`%s` (len:%d)", name, xtended->o.name, l );
        tname = xtended->o.name;
        ha = xtended->o.has_arg;
        if ( ( witharg && ha == required_argument ) || ( !witharg && ha == no_argument ) || ha == optional_argument )
        {
          if ( !extended_soft ) /* find first!? */
          {
            extended_soft = xtended;
          }
          if ( !tname[l] && !extended_exact )
          {
            extended_exact = xtended;
            DUF_TRACE( findopt, 5, "@by %s found exact name:`%s`", name, xtended->o.name );
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
    DUF_TRACE( findopt, 5, "@@@(%s) by %s found (%d;exact:%d) name:`%s`", mas_error_name_i( rpr ), name, cnt, extended_exact?1:0, extended->o.name );
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
duf_find_name_long( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pcnt, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  DUF_TRACE( findopt, 7, "@@@@to find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    extended = duf_find_name_long_soft( name, witharg, xtended, pcnt, &rpr );
  else
    extended = duf_find_name_long_exact( name, witharg, xtended, pcnt /*, &rpr */  );
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
duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pcnt, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  extended = duf_find_name_long( name, witharg, xtended, soft, pcnt, &rpr );
  DUF_TRACE( findopt, 7, "@@name:%s; extended:%d", name, extended ? 1 : 0 );
  if ( extended )
    DUF_TRACE( findopt, 3, "@(%s) by %s found name:`%s`", mas_error_name_i( rpr ), name, extended->o.name );

  DUF_TRACE( findopt, 6, "name:%s; witharg:%d; soft:%d", name, witharg, soft );
  if ( pno && DUF_IS_ERROR_N( rpr, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
    *pno = 0;
    DUF_TRACE( findopt, 5, "(try 'no') name:%s; witharg:%d; soft:%d", name, witharg, soft );
    if ( 0 == strncmp( name, DUF_NO_PREFIX, strlen( DUF_NO_PREFIX ) ) && strlen( name ) > strlen( DUF_NO_PREFIX ) )
    {
      /* DUF_CLEAR_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND ); (* DUF_ERROR_OPTION_NOT_FOUND impossible here !?  *) */
      extended = duf_find_name_long( name + strlen( DUF_NO_PREFIX ), witharg, xtended, soft, pcnt, &rpr );
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
