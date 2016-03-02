/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include "muc_tracen_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "muc_errorn_defs.h"                                         (* DUF_NOERROR; MUC_CLEAR_ERROR; DUF_E_(LOWER|UPPER); MUC_TEST_R ... ♠ *) */

#include "muc_option_longopts.h"                                     /* muc_options_create_longopts_table ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

/* ###################################################################### */
#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_config_credel.h"
/* ###################################################################### */

static muc_option_gen_code_t
muc_cli_option_count_maxcodeval( muc_config_cli_t * cli MAS_UNUSED, muc_longval_extended_vtable_t * *xvtables )
{
  const muc_longval_extended_vtable_t *xtable;
  muc_option_gen_code_t maxcodeval = 0;

  while ( ( xtable = *xvtables++ ) )
  {
    const muc_longval_extended_t *xtended;

    xtended = xtable->xlist;
    while ( xtended->o.name )
    {
      if ( maxcodeval < xtended->o.val )
        maxcodeval = xtended->o.val;
      xtended++;
    }
  }
  return maxcodeval;
}

static char *
muc_cli_option_shorts_create( muc_config_cli_t * cli MAS_UNUSED, muc_longval_extended_vtable_t * *xvtables )
{
  const muc_longval_extended_vtable_t *xtable;
  char shorts[1024 * 4] = "";
  char *p = shorts;

  while ( ( xtable = *xvtables++ ) )
  {
    const muc_longval_extended_t *xtended;

    xtended = xtable->xlist;
    while ( xtended->o.name )
    {
      if ( xtended->o.val && xtended->o.val < 0xFF )
      {
        MAST_TRACE( options, 50, "@@@+ shorts[%s]: %c : %x", xtended->o.name, xtended->o.val, xtended->o.val );
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
        MAST_TRACE( options, 60, "@@@- shorts[%s]: %c : %x", xtended->o.name, ' ', xtended->o.val );
      }
      xtended++;
    }
  }
  return *shorts ? mas_strdup( shorts ) : NULL;
}

muc_config_cli_t *
muc_cli_options_create( int argc, char **argv, const muc_longval_extended_table_t * const *xtable_list, unsigned mandatory_config,
                        const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc, const mas_config_trace_t * ptracecfg )
{
  muc_config_cli_t *cli;

  cli = mas_malloc( sizeof( muc_config_cli_t ) );
  assert( cli );
  muc_cli_options_init( cli, argc, argv, xtable_list, mandatory_config, config_dir, commands_dir, varfunc, ptracecfg );
  return cli;
}

void
muc_cli_options_init( muc_config_cli_t * cli, int argc, char **argv, const muc_longval_extended_table_t * const *xtable_list,
                      unsigned mandatory_config, const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc,
                      const mas_config_trace_t * ptracecfg )
{
  assert( cli );
  {
    memset( cli, 0, sizeof( muc_config_cli_t ) );
    cli->ptracecfg = ptracecfg;
    cli->carg.argc = argc;
    cli->carg.argv = argv;
    cli->mandatory_config = mandatory_config;
  /* const muc_longval_extended_vtable_t * const *xvtables
   * cli->xvtable_multi = xvtables;
   * */
    cli->xvtable_multi = muc_cli_options_xtable_list2xvtable( cli, xtable_list ); /* allocates */
    cli->shorts = muc_cli_option_shorts_create( cli, cli->xvtable_multi );
    cli->maxcodeval = muc_cli_option_count_maxcodeval( cli, cli->xvtable_multi );
    cli->varfunc = varfunc;

    cli->longopts_table = muc_options_create_longopts_table( cli->xvtable_multi );
    assert( cli->longopts_table );

    cli->config_dir = mas_strdup( config_dir );
    cli->cmds_dir = mas_strdup( commands_dir );
  }
/*
  TODO
    config_dir (..._options_file.c)
    cmds_dir (..._options_file.c)
    opt.output.history_filename (..._options_interactive.c) =>  cli.history_filename
*/
}

void
muc_cli_options_delete( muc_config_cli_t * cli )
{
  muc_cli_options_shut( cli );
  mas_free( cli );
}

static void
cli_options_shut( muc_config_cli_t * cli )
{
  cli->mandatory_config = 0;
  muc_options_delete_longopts_table( cli->longopts_table );
  cli->longopts_table = NULL;
  mas_del_argv( cli->targ.argc, cli->targ.argv, 0 );
  cli->targ.argc = 0;
  cli->targ.argv = NULL;
  mas_free( cli->shorts );
  cli->shorts = NULL;
  mas_free( cli->history_filename );
  cli->history_filename = NULL;
  mas_free( cli->config_dir );
  cli->config_dir = NULL;
  mas_free( cli->cmds_dir );
  cli->cmds_dir = NULL;

  {
    for ( size_t itab = 0; cli->xvtable_multi && cli->xvtable_multi[itab]; itab++ )
    {
      mas_free( cli->xvtable_multi[itab]->xlist );
      mas_free( cli->xvtable_multi[itab] );
      cli->xvtable_multi[itab] = NULL;
    }

    mas_free( cli->xvtable_multi );
    cli->xvtable_multi = NULL;
  }

  {
    FILE *f = NULL;

    f = fopen( "options.tmp", "w" );
    if ( f )
    {
      muc_option_stage_t stage = MUC_OPTION_STAGE_NONE;
      muc_option_source_t source = {.sourcecode = MUC_OPTION_SOURCE_NONE };

      for ( size_t iod = 0; iod < cli->aod.count; iod++ )
      {
        muc_option_data_t *pod;

        pod = &cli->aod.pods[iod];
      /* T( "%lu. %s.pod %s => %s", iod, muc_optstage_name( pod->stage ), muc_optsource_name( pod->source ), pod->name ); */
        if ( source.sourcecode != pod->source.sourcecode )
          fprintf( f, "* SOURCE %s\n", muc_optsource_name( cli, ( source = pod->source ) ) );
        if ( stage != pod->stage )
          fprintf( f, "* STAGE %s\n", muc_optstage_name( cli, ( stage = pod->stage ) ) );
        if ( pod->doindex >= 0 )
        {
          fprintf( f, "\t%c(%2ld) %lu. --%s", ( pod->clarified[stage] ? '+' : ' ' ), pod->doindex, iod,
                   pod->xfound.xarray[pod->doindex].xtended->o.name );
          if ( pod->optarg )
            fprintf( f, "='%s'", pod->optarg );
        }
        fprintf( f, "\t\t[%c(%2ld) %lu. --%s", ( pod->clarified[stage] ? '+' : ' ' ), pod->doindex, iod, pod->name );
        if ( pod->optarg )
          fprintf( f, "='%s'", pod->optarg );
        fprintf( f, "]\n" );
      }
      fclose( f );
    }
  }
  {
    for ( size_t iod = 0; iod < cli->aod.count; iod++ )
    {
      muc_option_data_t *pod;

      pod = &cli->aod.pods[iod];
      mas_free( pod->xfound.xarray );
      pod->xfound.xarray = NULL;
      mas_free( pod->name );
      pod->name = NULL;
      mas_free( pod->optarg );
      pod->optarg = NULL;
      mas_free( pod->string_copy );
      pod->string_copy = NULL;
    }
    mas_free( cli->aod.pods );
    cli->aod.pods = NULL;
    cli->aod.size = cli->aod.count = 0;
  }
}

void
muc_cli_options_shut( muc_config_cli_t * cli )
{
  if ( cli )
    cli_options_shut( cli );
}
