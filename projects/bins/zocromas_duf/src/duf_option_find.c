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

int
duf_check_stage( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  int r = 0;
  int r1 = 0;
  int r2 = 0;

  r = ( istage == -1 );
  /* DUF_PRINTF( 0, "1.  (%d)  check_stage:%s: %d", istage, extended->o.name, r ); */
  r = r || extended->stage.flag;
  /* DUF_PRINTF( 0, "2.  (%d)  check_stage:%s: %d", istage, extended->o.name, r ); */
  r1 = ( !extended->use_stage || ( extended->stage.min <= istage && extended->stage.max >= istage ) );
  /* DUF_PRINTF( 0, "3a. (%d)  check_stage:%s: %d [%d:%d]", istage, extended->o.name, r1, extended->stage.min, extended->stage.max ); */
  r2 = ( !xtable || ( !xtable->use_stage || ( xtable->stage.min <= istage && xtable->stage.max >= istage ) ) );
  /* DUF_PRINTF( 0, "3b. (%d)  check_stage:%s: %d [%d:%d]", istage, extended->o.name, r2, xtable ? xtable->stage.min : 999,
     xtable ? xtable->stage.max : 999 ); */
  r = r || ( r1 && r2 );
  /* DUF_PRINTF( 0, "%d:%s: sm:%lx [%x]", istage, extended->o.name, xtable->stage_mask, ( 1 << istage ) ); */
  if ( ( extended->use_stage_mask && ( extended->stage_mask & ( 1 << istage ) ) )
       || ( xtable->use_stage_mask && ( xtable->stage_mask & ( 1 << istage ) ) ) )
  {
    r = 0;
  }
  /* DUF_PRINTF( 0, "3.  (%d)  check_stage:%s: %d", istage, extended->o.name, r ); */
  return r;
}

const duf_longval_extended_t *
duf_find_codeval_extended_std( duf_option_code_t codeval, const duf_longval_extended_table_t ** pxtable, int *pr )
{
  const duf_longval_extended_t *rextended = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; !rextended && *xtables; xtables++, ntable++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    for ( const duf_longval_extended_t * xtended = xtable->table; !rextended && xtended->o.name; xtended++, tbcount++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d:%d [%s]", ntable, tbcount, xtended->o.name );
      if ( xtended && xtended->o.val == codeval )
      {
        rextended = xtended;
        if ( pxtable )
          *pxtable = xtable;
        r = 0;
        break;                  /* ? */
      }
    }
  }

  if ( pr )
    *pr = r;
  return rextended;
}

/* return so called `longindex` */
static const duf_longval_extended_t *
duf_find_name_long_exact( const char *name, int witharg, const duf_longval_extended_t * xtended, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;

  if ( name && *name )
    for ( ; xtended->o.name; xtended++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s`", xtended->o.name, name );
      if ( 0 == strcmp( name, xtended->o.name ) )
      {
        DUF_TRACE( options, 3, "found name:`%s`", xtended->o.name );
        r = 0;
        extended = xtended;
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
duf_find_name_long_soft( const char *name, int witharg, const duf_longval_extended_t * xtended, int *pr )
{
  int cnt = 0;
  const duf_longval_extended_t *extended = NULL;
  const duf_longval_extended_t *extended_soft = NULL;
  const duf_longval_extended_t *extended_exact = NULL;
  int r = DUF_ERROR_OPTION_NOT_FOUND;

  if ( name && *name )
    for ( ; xtended->o.name; xtended++ )
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
            DUF_TRACE( options, 3, "@(%s) found soft name:`%s`", duf_error_name( r ), xtended->o.name );
          }
          if ( !tname[l] && !extended_exact )
          {
            extended_exact = xtended;
            DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
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
      DOR( r, DUF_ERROR_OPTION_MULTIPLE );
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
duf_find_name_long( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  DUF_TRACE( options, 6, "to find name:`%s`; witharg:%d; soft:%d", name, witharg, soft );
  if ( soft )
    extended = duf_find_name_long_soft( name, witharg, xtended, pr );
  else
    extended = duf_find_name_long_exact( name, witharg, xtended, pr );
  if ( extended )
    DUF_TRACE( options, 3, "@(%s) found name:`%s`", duf_error_name( pr ? *pr : -1 ), extended->o.name );
  return extended;
}

const duf_longval_extended_t *
duf_find_name_long_no( const char *name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pr )
{
  int r = 0;
  const duf_longval_extended_t *extended = NULL;

  extended = duf_find_name_long( name, witharg, xtended, soft, &r );
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
      extended = duf_find_name_long( name + strlen( DUF_NO_PREFIX ), witharg, xtended, soft, &r );
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
