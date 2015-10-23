#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


#include "duf_option_find.h"
#include "duf_xtended_table.h"

#include "duf_option.h"
#include "duf_option_names.h"

/* ###################################################################### */
/* #include "duf_options.h" */
#include "duf_option_cmd.h"
/* ###################################################################### */


/*
 * possible error(s):
 *   DUF_ERROR_OPTION_MULTIPLE
 *   */
static const duf_longval_extended_t *
duf_find_cmd_long_no( const char *string, const duf_longval_extended_t * xtended, char vseparator, char **parg, int *pno, int *pcnt, int *pr )
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
  DUF_TRACE( options, 6, "vseparator:'%c'; name:`%s`; arg:`%s`", vseparator, name, arg );

  extended = duf_find_name_long_no( name, arg ? 1 : 0, xtended, 1 /* soft */ , pno, pcnt, &rpr );
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

/* look xtable for cmd from string and exec if found */
static int
duf_exec_cmd_xtable( const char *string, const duf_longval_extended_table_t * xtable, char vseparator, duf_option_stage_t istage, int all_matched,
                     duf_option_source_t source )
{
  DEBUG_STARTR( r );
  int found = 0;
  int cntfound = 0;
  const duf_longval_extended_t *xtended = xtable->table;

  /* DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND ); */
  while ( xtended /* && DUF_CLEARED_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ) */  && DUF_NOERROR( r ) )
  {
    const duf_longval_extended_t *extended = NULL;
    char *arg = NULL;
    int no = 0;

    DUF_TRACE( findopt, 5, "to find %s at %s [%ld]", string, xtable->name, xtended - xtable->table );
    extended = duf_find_cmd_long_no( string, xtended, vseparator, &arg, &no, &cntfound, &r );
    DUF_TRACE( options, 9, "string:%s; no:%d extended(+-):%d", string, no, extended ? 1 : 0 );

    if ( extended && DUF_NOERROR( r ) )
    {
      DUF_TRACE( findopt, 5, "@(%s:%d) at %s by %s found cmd for %s", mas_error_name_i( r ), found, xtable->name, string, extended->o.name );
      DORF( r, DUF_WRAPPED( duf_clarify_xcmd_full ), extended, arg, istage, xtable, no, source );
      DUF_TRACE( findopt, 5, "@(%s:%d) at %s by %s full done for %s", mas_error_name_i( r ), found, xtable->name, string, extended->o.name );
      found += ( extended ? 1 : 0 );
    }
    else
    {
      /* DUF_SHOW_ERRORO( "extended is NULL after duf_find_cmd_long_no for %s", string ); */
    }
    mas_free( arg );
    arg = NULL;
    xtended = extended;
    if ( !all_matched )
      break;
    if ( xtended )
      xtended++;
  }
  /* assert( ( found && DUF_NOERROR( r ) ) || ( !found && DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) ) ); */
  if ( found )
  {
    /* DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    /* if ( DUF_NOERROR( r ) ) */
    /*   r = found;            */
  }
  if ( found )
    DUF_TRACE( options, 3, "@executed" );
  if ( !found && DUF_NOERROR( r ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  /* DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND ); */
  DEBUG_ENDR( r );
}

/* look all xtables for cmd from string and exec if found */
static int
duf_exec_cmd_long_xtables( const char *string, const duf_longval_extended_table_t ** xtables, char vseparator, duf_option_stage_t istage,
                           int all_matched, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  int found = 0;
  const duf_longval_extended_table_t *xtable = NULL;

  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  /* find and execute ALL matching commands */
  while ( ( xtable = *xtables++ ) && DUF_CLEARED_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
/* 
 * duf_exec_cmd_xtable >0 if found
 * duf_exec_cmd_xtable == 0 if ???
 * duf_exec_cmd_xtable < 0 if error
 * */
/* look xtable for cmd from string and exec if found */
    DOR( r, duf_exec_cmd_xtable( string, xtable, vseparator, istage, all_matched, source ) );
    DUF_TRACE( options, 10, "(%d:%s) executed cmd; xs=%s", r, r < 0 ? mas_error_name_i( r ) : "-", string );
    if ( DUF_NOERROR( r ) )     /* DUF_NOERROR(r) equal to r>=0 ?? */
    {
      /* found one; continue to possibly find more */
      found += 1;
      /* r = 0; */
    }
    /* if ( DUF_NOERROR( r ) ) */
    /*   break;      */
    if ( DUF_IS_ERROR( r ) )
      DUF_TRACE( options, 7, "@@%s at %s", mas_error_name_i( r ), xtable->name ? xtable->name : "??" );
  }
  /* no error if at least one found */
  if ( found )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  DUF_TRACE( options, 6, "(%d:%s) executed cmd; xs=%s", r, mas_error_name_i( r ), string );
  if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
    DUF_SHOW_ERRORO( "@@@@@@@Invalid command -- '%s' at %s stage", string, duf_optstage_name( istage ) );
    /* TODO DUF_POP_ERROR( r ); */
  }

  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  DEBUG_ENDR( r );
}

/* look all std xtables for cmd from string and exec if found */
int
duf_exec_cmd_long_xtables_std( const char *string, char vseparator, duf_option_stage_t istage, int all_matched, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
  DUF_TRACE( options, 4, "(%d) executed cmd", r );
/* look all xtables for cmd from string and exec if found */
  DOR( r, duf_exec_cmd_long_xtables( string, duf_extended_table_multi(  ), vseparator, istage, all_matched, source ) );
  DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
}
