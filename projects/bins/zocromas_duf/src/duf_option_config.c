#undef MAS_TRACING
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"

#include "duf_option_extended.h"
#include "duf_option_longopts.h"

#include "duf_option_defs.h"
#include "duf_option_stage.h"
#include "duf_option_source.h"
#include "duf_options_enum.h"                                        /* duf_option_code_t */

/* ###################################################################### */
#include "duf_option_config_credel.h"
#include "duf_option_config.h"
/* ###################################################################### */

static duf_config_cli_t *config_cli = NULL;

void
duf_cli_options_init_global( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_table_t * const *xtable_list,
                             const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc )
{
  config_cli = cli;
  duf_cli_options_init( cli, argc, argv, xtable_list, config_dir, commands_dir, varfunc );
}

void
duf_cli_options_shut_global( void )
{
  duf_cli_options_shut( config_cli );
}

duf_config_cli_t *
duf_cli_options_config( void )
{
  assert( config_cli );
  return config_cli;
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
duf_cli_options_get_cargvn( int n )
{
  char *const *cargv;

  cargv = duf_cli_options_get_cargv(  );
  return cargv ? cargv[n] : NULL;
}

const char *
duf_cli_options_bin_name( void )
{
  const char *binname;

  binname = duf_cli_options_get_cargvn( 0 );
  {
    if ( binname )
      binname = strrchr( binname, '/' );
    if ( binname )
      binname++;
  }
  return binname;
}

const char *
duf_cli_options_config_file_name( void )
{
  const char *binname;

  binname = duf_cli_options_bin_name(  );
  return binname ? binname : DUF_CONFIG_FILE_NAME;
}

const char *
duf_cli_options_get_shorts( void )
{
/* return cli_options_shorts; */
  return config_cli ? config_cli->shorts : NULL;
}

const char *
duf_cli_options_config_dir( void )
{
/* return cli_options_shorts; */
  return config_cli ? config_cli->config_dir : NULL;
}

const char *
duf_cli_options_commands_dir( void )
{
/* return cli_options_shorts; */
  return config_cli ? config_cli->cmds_dir : NULL;
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

duf_longval_extended_vtable_t **
duf_cli_options_xvtable_multi( void )
{
  return config_cli ? config_cli->xvtable_multi : NULL;
}

char
duf_cli_options_delimiter( void )
{
  return config_cli ? config_cli->option_delimiter : ':';
}

duf_option_adata_t *
duf_cli_options_aod( void )
{
  return config_cli ? &config_cli->aod : NULL;
}

mas_arg_get_cb_arg_t
duf_cli_options_varfunc( void )
{
  return config_cli ? config_cli->varfunc : NULL;
}

/* 20160220.190632 */
duf_longval_extended_vtable_t **
duf_cli_options_xtable_list2xvtable( const duf_longval_extended_table_t * const *xtable_multi )
{
  unsigned numtabs = 0;
  duf_option_code_t maxcodeval = 0;
  duf_longval_extended_vtable_t **vtable_multi = NULL;

  for ( numtabs = 0; xtable_multi[numtabs] && xtable_multi[numtabs]->xlist; numtabs++ );
  assert( vtable_multi == NULL );
  vtable_multi = mas_malloc( sizeof( duf_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  memset( vtable_multi, 0, sizeof( duf_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  {
    for ( size_t itab = 0; itab < numtabs; itab++ )
    {
      const duf_longval_extended_t *xlist;

      xlist = xtable_multi[itab]->xlist;
      for ( const duf_longval_extended_t * x = xlist; x->o.name; x++ )
      {
        size_t xn = x - xlist;

        if ( xlist[xn].o.val && xlist[xn].o.val > maxcodeval )
          maxcodeval = xlist[xn].o.val;
      }
    }
    maxcodeval += 100;
    maxcodeval /= 100;
    maxcodeval *= 100;
  }
  for ( size_t itab = 0; itab < numtabs; itab++ )
  {
    duf_longval_extended_vtable_t *vtable;

    vtable = mas_malloc( sizeof( duf_longval_extended_t ) );
    memset( vtable, 0, sizeof( duf_longval_extended_t ) );
#if 0
    memcpy( &vtable, &xtable_multi[itab], sizeof( vtable ) );
#else
    vtable->name = xtable_multi[itab]->name;
    vtable->id = xtable_multi[itab]->id;
    vtable->stage_opts = xtable_multi[itab]->stage_opts;
#endif
  /* T( "@@%lu. tab.name: '%s' : [%p:%p]", itab, vtable->name, vtable, vtable->xlist ); */
    {
      size_t xcnt = 0;

      for ( const duf_longval_extended_t * x = xtable_multi[itab]->xlist; x->o.name; x++ )
        xcnt++;
      vtable->xlist = mas_malloc( sizeof( duf_longval_extended_t ) * ( xcnt + 1 ) );
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        vtable->xlist[xn] = xtable_multi[itab]->xlist[xn];
      }
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        if ( !vtable->xlist[xn].o.val )
        {
          T( "@\"%s\" no codeval; setting automatically to %d", vtable->xlist[xn].o.name, maxcodeval );
          vtable->xlist[xn].o.val = maxcodeval++;
        }
        else
        {
        /* T( "@@%s --", vtable->xlist[xn].o.name ); */
        }
      }
    }
    vtable_multi[itab] = vtable;
  }
  return vtable_multi;
}
