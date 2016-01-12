#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_option_extended.h"
#include "duf_option_longopts.h"

/* ###################################################################### */
#include "duf_option_config.h"
/* ###################################################################### */

static duf_config_cli_t *config_cli = NULL;

duf_config_cli_t *
duf_cli_options_config( void )
{
  assert( config_cli );
  return config_cli;
}

static char *
duf_cli_option_shorts_create( const duf_longval_extended_table_t ** xtables )
{
  const duf_longval_extended_table_t *xtable;
  char shorts[1024 * 4] = "";
  char *p = shorts;

  while ( ( xtable = *xtables++ ) )
  {
    const duf_longval_extended_t *xtended;

    xtended = xtable->table;
    while ( xtended->o.name )
    {
      if ( xtended->o.val && xtended->o.val < 0xFF )
      {
        DUF_TRACE( options, 50,  "@@@+ shorts[%s]: %c : %x", xtended->o.name, xtended->o.val, xtended->o.val );
        /* DUF_SHOW_ERROR( "S:%c %x - %s", duf_longopts[ilong].val, duf_longopts[ilong].val, shorts ); */
        if ( !strchr( shorts, ( char ) xtended->o.val ) )
        {
          *p++ = ( char ) xtended->o.val;
          if ( xtended->o.has_arg == no_argument );
          else if ( xtended->o.has_arg == required_argument )
            *p++ = ':';
          else if ( xtended->o.has_arg == optional_argument )
          {
            /* *p++ = ':'; */
            /* *p++ = ':'; */
          }
          else
          {
            *p++ = ':';
            *p++ = ':';
          }
        }
        *p = 0;
      }
      else
      {
        DUF_TRACE( options, 60, "@@@- shorts[%s]: %c : %x", xtended->o.name, ' ', xtended->o.val );
      }
      xtended++;
    }
  }
  return *shorts ? mas_strdup( shorts ) : NULL;
}

void
duf_cli_options_init( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_table_t ** xtables )
{
  if ( cli )
  {
    config_cli = cli;
    memset( config_cli, 0, sizeof( duf_config_cli_t ) );
    config_cli->carg.argc = argc;
    config_cli->carg.argv = argv;
    config_cli->xtable_multi = xtables;
    config_cli->shorts = duf_cli_option_shorts_create( xtables );

    config_cli->longopts_table = duf_options_create_longopts_table( xtables );
    assert( config_cli->longopts_table );
  }
/*
  TODO
    config_dir (duf_options_file.c)
    cmds_dir (duf_options_file.c)
    opt.output.history_filename (duf_options_interactive.c) =>  cli.history_filename
    scn.pdi (duf_options_interactive.c)
*/
}

void
duf_cli_options_shut( duf_config_cli_t * cli )
{
  assert( cli == config_cli );
  duf_options_delete_longopts_table( config_cli->longopts_table );
  config_cli->longopts_table = NULL;
  mas_del_argv( config_cli->targ.argc, config_cli->targ.argv, 0 );
  config_cli->targ.argc = 0;
  config_cli->targ.argv = NULL;
  mas_free( config_cli->shorts );
  config_cli->shorts = NULL;
  mas_free( config_cli->history_filename );
  config_cli->history_filename = NULL;
}

const char *
duf_cli_options_get_history_filename( void )
{
  return config_cli ? config_cli->history_filename : NULL;
}

mas_argvc_t *
duf_cli_options_get_targ( void )
{
  return config_cli ? &config_cli->targ : NULL;
}

int
duf_cli_options_get_targc( void )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ ? targ->argc : 0;
}

int *
duf_cli_options_get_ptargc( void )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ ? &targ->argc : 0;
}

char **
duf_cli_options_get_targv( void )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ ? targ->argv : 0;
}

char ***
duf_cli_options_get_ptargv( void )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ ? &targ->argv : 0;
}

char *
duf_cli_options_get_targi( int ia )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ && ia < targ->argc ? targ->argv[ia] : NULL;
}

char **
duf_cli_options_get_ptargi( int ia )
{
  mas_argvc_t *targ;

  targ = duf_cli_options_get_targ(  );
  return targ && ia < targ->argc ? &targ->argv[ia] : NULL;
}

mas_cargvc_t *
duf_cli_options_get_carg( void )
{
  return config_cli ? &config_cli->carg : NULL;
}

int
duf_cli_options_get_cargc( void )
{
  return config_cli ? config_cli->carg.argc : 0;
}

char *const *
duf_cli_options_get_cargv( void )
{
  return config_cli ? config_cli->carg.argv : NULL;
}

const char *
duf_cli_options_get_shorts( void )
{
  /* return cli_options_shorts; */
  return config_cli ? config_cli->shorts : NULL;
}

int
duf_cli_options_get_targ_offset( void )
{
  return config_cli ? config_cli->targ_offset : 0;
}

void
duf_cli_options_set_targ_offset( int targ_offset )
{
  if ( config_cli )
    config_cli->targ_offset = targ_offset;
}

static int
duf_reorder_argv_at_sign( int argc, char *argv[] )
{
  int ra = 0;
  int nkra = -1;
  int kra = -1;


  for ( int ia = 0; ia < argc; ia++ )
  {
    if ( *( argv[ia] ) == '@' )
      kra = ia;
    else
      nkra = ia;
    if ( kra >= 0 && nkra >= 0 )
    {
      if ( kra > nkra )
      {
        char *t;

        t = argv[kra];
        argv[kra] = argv[nkra];
        argv[nkra] = t;
        ia = 0;
        kra = nkra = -1;
      }
    }
  }
  ra = argc;
#if 1
  for ( int ia = 0; ia < argc; ia++ )
  {
    if ( argv[ia][0] != '@' )
    {
      ra = ia;
      break;
    }
  }
#else
  for ( int ia = argc - 1; ia >= 0; ia-- )
  {
    if ( argv[ia] && argv[ia][0] != '@' )
      ra = ia;
  }
#endif


  return ra;
}

/* 20150924.144047 */
static int
duf_reorder_argvc_at_sign( mas_argvc_t * ptarg )
{
  return duf_reorder_argv_at_sign( ptarg->argc, ptarg->argv );
}

void
duf_cli_options_reset_targ_offset( void )
{
  config_cli->targ_offset = duf_reorder_argvc_at_sign( duf_cli_options_get_targ(  ) );
}

duf_option_t *
duf_cli_options_get_longopts_table( void )
{
  return config_cli ? config_cli->longopts_table : NULL;
}
