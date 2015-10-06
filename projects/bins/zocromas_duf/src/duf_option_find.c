#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"

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

  DUF_TRACE( options, 3, "checking stage; istage:%d xflag:%d xuse:%d xminmax:%d/%d", istage, extended->stage.flag, extended->use_stage,
             extended->stage.min, extended->stage.max );
  DUF_TRACE( options, 3, "checking stage; istage:%d tflag:%d tuse:%d tminmax:%d/%d", istage, xtable->stage.flag, xtable->use_stage,
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

  DUF_TRACE( options, 2, "checked stage; istage:%d; r0:%d", istage, r0 );
  return r0;
}

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
    for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; !rxtended && *xtables; xtables++, ntable++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      for ( const duf_longval_extended_t * xtended = xtable->table; !rxtended && xtended->o.name; xtended++, tbcount++ )
      {
        if ( xtended )
        {
          DUF_TRACE( options, 5, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xtended->o.name, xtended->o.val, codeval );
          if ( xtended->o.val == codeval )
          {
            rxtended = xtended;
            if ( pxtable )
              *pxtable = xtable;
            DUF_TRACE( options, 2, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xtended->o.name );
#if 0
            ok = 1;
#endif
            break;              /* ? */
          }
        }
      }
    }
    DUF_TRACE( options, 2, "@li2ex ? %d:%d [%s]", ntable, tbcount, rxtended ? rxtended->o.name : NULL );
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
duf_find_name_long_exact( const char *name, int witharg_unused DUF_UNUSED, const duf_longval_extended_t * xtended /*, int *pr */  )
{
  const duf_longval_extended_t *extended = NULL;

#if 0
  int ok = 0;
  int rpr = 0;
#endif

  if ( name && *name )
    for ( ; xtended->o.name; xtended++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s`", xtended->o.name, name );
      if ( 0 == strcmp( name, xtended->o.name ) )
      {
        DUF_TRACE( options, 3, "found name:`%s`", xtended->o.name );
#if 0
        ok = 1;
#endif
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
duf_find_name_long_soft( const char *name, int witharg, const duf_longval_extended_t * xtended, int *pr )
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
      DUF_TRACE( options, 8, "compare name:`%s` ? `%s`", xtended->o.name, name );
      if ( 0 == strncmp( name, xtended->o.name, l ) )
      {
        int ha;
        const char *tname;

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
            DUF_TRACE( options, 3, "found exact name:`%s`", xtended->o.name );
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
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name_i( rpr ), extended->o.name );
  if ( pr )
    *pr = rpr;
  return extended;
}
/*
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *   */
static const duf_longval_extended_t *
duf_find_name_long( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  DUF_TRACE( options, 6, "to find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    extended = duf_find_name_long_soft( name, witharg, xtended, &rpr );
  else
    extended = duf_find_name_long_exact( name, witharg, xtended /*, &rpr */  );
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name_i( pr ? *pr : -1 ), extended->o.name );
  if ( pr )
    *pr = rpr;
  return extended;
}

/*
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *   */
const duf_longval_extended_t *
duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pr )
{
  int rpr = 0;
  const duf_longval_extended_t *extended = NULL;

  extended = duf_find_name_long( name, witharg, xtended, soft, &rpr );
  DUF_TRACE( options, 4, "@@name:%s; extended:%d", name, extended ? 1 : 0 );
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name_i( rpr ), extended->o.name );
  DUF_TRACE( options, 6, "name:%s; witharg:%d; soft:%d", name, witharg, soft );
  if ( pno && DUF_IS_ERROR_N( rpr, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
    *pno = 0;
    DUF_TRACE( options, 6, "(try 'no') name:%s; witharg:%d; soft:%d", name, witharg, soft );
    if ( 0 == strncmp( name, DUF_NO_PREFIX, strlen( DUF_NO_PREFIX ) ) && strlen( name ) > strlen( DUF_NO_PREFIX ) )
    {
      /* DUF_CLEAR_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND ); (* DUF_ERROR_OPTION_NOT_FOUND impossible here !?  *) */
      extended = duf_find_name_long( name + strlen( DUF_NO_PREFIX ), witharg, xtended, soft, &rpr );
      if ( extended )
        DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name_i( rpr ), extended->o.name );
      *pno = 1;
    }
  }
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name_i( rpr ), extended->o.name );
  if ( pr )
    *pr = rpr;
  return extended;
}
