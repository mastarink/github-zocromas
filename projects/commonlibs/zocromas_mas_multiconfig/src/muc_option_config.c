/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include <assert.h>                                                  /* assert */
#include <string.h>                                                  /* strrchr; etc. */
#include <ctype.h>                                                   /* toupper */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_longopts.h"                                     /* muc_options_create_longopts_table ♠ */

#include "muc_option_defs.h"
#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_config_credel.h"
#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
/* ###################################################################### */

/* static muc_config_cli_t *config_cli = NULL; */

const char *
muc_cli_options_get_history_filename( const muc_config_cli_t * cli )
{
  return cli ? cli->history_filename : NULL;
}

mas_argvc_t *
muc_cli_options_get_targ( muc_config_cli_t * cli )
{
  return cli ? &cli->targ : NULL;
}

int
muc_cli_options_get_targc( muc_config_cli_t * cli )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ ? targ->argc : 0;
}

int *
muc_cli_options_get_ptargc( muc_config_cli_t * cli )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ ? &targ->argc : 0;
}

char **
muc_cli_options_get_targv( muc_config_cli_t * cli )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ ? targ->argv : 0;
}

char ***
muc_cli_options_get_ptargv( muc_config_cli_t * cli )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ ? &targ->argv : 0;
}

char *
muc_cli_options_get_targi( muc_config_cli_t * cli, int ia )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ && ia < targ->argc ? targ->argv[ia] : NULL;
}

char **
muc_cli_options_get_ptargi( muc_config_cli_t * cli, int ia )
{
  mas_argvc_t *targ;

  targ = muc_cli_options_get_targ( cli );
  return targ && ia < targ->argc ? &targ->argv[ia] : NULL;
}

mas_cargvc_t *
muc_cli_options_get_carg( muc_config_cli_t * cli )
{
  return cli ? &cli->carg : NULL;
}

int
muc_cli_options_get_cargc( const muc_config_cli_t * cli )
{
  return cli ? cli->carg.argc : 0;
}

char *const *
muc_cli_options_get_cargv( const muc_config_cli_t * cli )
{
  return cli ? cli->carg.argv : NULL;
}

const char *
muc_cli_options_get_cargvn( const muc_config_cli_t * cli, int n )
{
  char *const *cargv;

  cargv = muc_cli_options_get_cargv( cli );
  assert( cargv );
  return cargv ? cargv[n] : NULL;
}

const char *
muc_cli_options_bin_name( const muc_config_cli_t * cli )
{
  const char *binname;

  binname = muc_cli_options_get_cargvn( cli, 0 );
  assert( binname );
  {
    if ( binname )
      binname = strrchr( binname, '/' );
    if ( binname )
      binname++;
  }
  return binname;
}

const char *
muc_cli_options_config_file_name( const muc_config_cli_t * cli )
{
  const char *binname;

  binname = muc_cli_options_bin_name( cli );
  return binname ? binname : MUC_CONFIG_FILE_NAME;
}

const char *
muc_cli_options_config_env_var_name( const muc_config_cli_t * cli )
{
  static unsigned evn_inited = 0;
  static char envvarname[512] = "DUF_";

  if ( !evn_inited )
  {
    const char *binname;

    binname = muc_cli_options_bin_name( cli );
    if ( binname )
    {
      strncpy( envvarname + strlen( envvarname ), binname, sizeof( envvarname ) - 20 );
      for ( char *p = envvarname; *p; p++ )
        *p = toupper( *p );
      strcpy( envvarname + strlen( envvarname ), "_OPTIONS" );
    }
    evn_inited = 1;
  }
  return *envvarname ? envvarname : "MSH_DUF_OPTIONS";
}

muc_option_gen_code_t
muc_cli_options_get_maxcodeval( const muc_config_cli_t * cli )
{
/* return cli_options_shorts; */
  return cli ? cli->maxcodeval : 0;
}

const char *
muc_cli_options_get_shorts( const muc_config_cli_t * cli )
{
/* return cli_options_shorts; */
  return cli ? cli->shorts : NULL;
}

const char *
muc_cli_options_config_dir( const muc_config_cli_t * cli )
{
/* return cli_options_shorts; */
  return cli ? cli->config_dir : NULL;
}

const char *
muc_cli_options_commands_dir( const muc_config_cli_t * cli )
{
/* return cli_options_shorts; */
  return cli ? cli->cmds_dir : NULL;
}

int
muc_cli_options_get_targ_offset( const muc_config_cli_t * cli )
{
  return cli ? cli->targ_offset : 0;
}

void
muc_cli_options_set_targ_offset( muc_config_cli_t * cli, int targ_offset )
{
  if ( cli )
    cli->targ_offset = targ_offset;
}

static int
muc_reorder_argv_at_sign( int argc, char *argv[] )
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
muc_reorder_argvc_at_sign( mas_argvc_t * ptarg )
{
  return muc_reorder_argv_at_sign( ptarg->argc, ptarg->argv );
}

void
muc_cli_options_reset_targ_offset( muc_config_cli_t * cli )
{
  cli->targ_offset = muc_reorder_argvc_at_sign( muc_cli_options_get_targ( cli ) );
}

muc_option_t *
muc_cli_options_get_longopts_table( const muc_config_cli_t * cli )
{
  return cli ? cli->longopts_table : NULL;
}

muc_longval_extended_vtable_t **
muc_cli_options_xvtable_multi( const muc_config_cli_t * cli )
{
  return cli ? cli->xvtable_multi : NULL;
}

char
muc_cli_options_delimiter( const muc_config_cli_t * cli )
{
  return cli ? cli->option_delimiter : ':';
}

muc_option_adata_t *
muc_cli_options_aod( muc_config_cli_t * cli )
{
  return cli ? &cli->aod : NULL;
}

mas_arg_get_cb_arg_t
muc_cli_options_varfunc( const muc_config_cli_t * cli )
{
  return cli ? cli->varfunc : NULL;
}

/* 20160220.190632 */
muc_longval_extended_vtable_t **
muc_cli_options_xtable_list2xvtable( const muc_config_cli_t * cli, const muc_longval_extended_table_t * const *xtable_multi )
{
  unsigned numtabs = 0;
  muc_option_gen_code_t maxcodeval = 0;
  muc_longval_extended_vtable_t **vtable_multi = NULL;

  for ( numtabs = 0; xtable_multi[numtabs] && xtable_multi[numtabs]->xlist; numtabs++ );
  assert( vtable_multi == NULL );
  vtable_multi = mas_malloc( sizeof( muc_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  memset( vtable_multi, 0, sizeof( muc_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  {
    for ( size_t itab = 0; itab < numtabs; itab++ )
    {
      const muc_longval_extended_t *xlist;

      xlist = xtable_multi[itab]->xlist;
      for ( const muc_longval_extended_t * x = xlist; x->o.name; x++ )
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
    muc_longval_extended_vtable_t *vtable;

    vtable = mas_malloc( sizeof( muc_longval_extended_t ) );
    memset( vtable, 0, sizeof( muc_longval_extended_t ) );
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

      for ( const muc_longval_extended_t * x = xtable_multi[itab]->xlist; x->o.name; x++ )
        xcnt++;
      vtable->xlist = mas_malloc( sizeof( muc_longval_extended_t ) * ( xcnt + 1 ) );
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        vtable->xlist[xn] = xtable_multi[itab]->xlist[xn];
      }
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        if ( !vtable->xlist[xn].o.val )
        {
          muc_QT( "@\"%s\" no codeval; setting automatically to %d", vtable->xlist[xn].o.name, maxcodeval );
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

const mas_config_trace_t *
muc_get_cli_options_trace_config( const muc_config_cli_t * cli )
{
  return cli->ptracecfg;
}
