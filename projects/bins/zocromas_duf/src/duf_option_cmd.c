#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_find.h"
#include "duf_option_extended.h"
#include "duf_option.h"

/* ###################################################################### */
#include "duf_option_cmd.h"
/* ###################################################################### */


/* return so called `longindex` */
int
duf_find_cmd_long_no( const char *string, const duf_longval_extended_t * xtable, char vseparator, char **parg, int *pno )
{
  DEBUG_STARTR( r );
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
    arg = mas_strdup( barg );

  DUF_TRACE( options, 6, "vseparator:'%c'; name:`%s`; arg:`%s`", vseparator, name, arg );

  DORN( r, duf_find_name_long_no( name, arg ? 1 : 0, xtable, 1 /* soft */ , pno ) );
  if ( r >= 0 && parg )
    *parg = arg;
  else
    mas_free( arg );
  arg = NULL;

  mas_free( name );
  DEBUG_ENDRN( r );
}

int
duf_execute_cmd_long_xtable( const char *string, const duf_longval_extended_t * xtable, char vseparator, int stage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  char *arg = NULL;
  int no = 0;

  r = duf_find_cmd_long_no( string, xtable, vseparator, &arg, &no );
  DUF_TRACE( options, 6, "string:%s; no:%d", string, no );
  if ( r >= 0 )
  {
    extended = &xtable[r];
    r = 0;
  }
/* 
 * duf_parse_option_long return 
 *   =0 for other option
 *   errorcode<0 for error
 * */
  /* if ( r >= 0 && extended && ( ( !arg && extended->o.has_arg == required_argument ) || ( arg && extended->o.has_arg == no_argument ) ) ) */
  /* {                                                                                                                                      */
  /*   r = DUF_ERROR_OPTION_VALUE;                                                                                                          */
  /* }                                                                                                                                      */


  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND );
  if ( extended )
  DOR( r, duf_parse_option_long_full( extended, arg, stage, no ) );
  mas_free( arg );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_execute_cmd_long_xtables( const char *string, const duf_longval_extended_t * xtables[], char vseparator, int stage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
    /* PF0( "table %s; search %s", xtable->o.name, string ); */
    DOR( r, duf_execute_cmd_long_xtable( string, xtable, vseparator, stage ) );
    if ( r >= 0 )
      break;
  }
  if ( r == DUF_ERROR_OPTION || r == DUF_ERROR_OPTION_NOT_FOUND )
  {
    DUF_SHOW_ERROR( "Invalid command -- '%s'", string );
  }
  DEBUG_ENDR( r );
}

int
duf_execute_cmd_long_xtables_std( const char *string, char vseparator, int stage )
{
  DEBUG_STARTR( r );
  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
  DOR( r, duf_execute_cmd_long_xtables( string, lo_extended_multi, vseparator, stage ) );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
}
