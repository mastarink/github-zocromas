/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *)                   */

/* #include "duf_se_only.h"                                             (* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ *) */

/* #include "duf_option_extended.h" */
#include "duf_option_longopts.h"                                     /* duf_options_create_longopts_table ♠ */

/* #include "duf_option_defs.h" */
#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */
#include "duf_options_enum.h"                                        /* duf_option_code_t ♠ */

/* ###################################################################### */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_config_credel.h"
/* ###################################################################### */

static char *
duf_cli_option_shorts_create( duf_config_cli_t * cli MAS_UNUSED, duf_longval_extended_vtable_t * *xvtables )
{
  const duf_longval_extended_vtable_t *xtable;
  char shorts[1024 * 4] = "";
  char *p = shorts;

  while ( ( xtable = *xvtables++ ) )
  {
    const duf_longval_extended_t *xtended;

    xtended = xtable->xlist;
    while ( xtended->o.name )
    {
      if ( xtended->o.val && xtended->o.val < 0xFF )
      {
        DUF_TRACE( options, 50, "@@@+ shorts[%s]: %c : %x", xtended->o.name, xtended->o.val, xtended->o.val );
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

duf_config_cli_t *
duf_cli_options_create( int argc, char **argv, const duf_longval_extended_table_t * const *xtable_list, const char *config_dir,
                        const char *commands_dir, mas_arg_get_cb_arg_t varfunc, const mas_config_trace_t * ptracecfg )
{
  duf_config_cli_t *cli;

  cli = mas_malloc( sizeof( duf_config_cli_t ) );
  duf_cli_options_init( cli, argc, argv, xtable_list, config_dir, commands_dir, varfunc, ptracecfg );
  return cli;
}

void
duf_cli_options_init( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_table_t * const *xtable_list, const char *config_dir,
                      const char *commands_dir, mas_arg_get_cb_arg_t varfunc, const mas_config_trace_t * ptracecfg )
{
  if ( cli )
  {
    memset( cli, 0, sizeof( duf_config_cli_t ) );
    cli->ptracecfg = ptracecfg;
    cli->carg.argc = argc;
    cli->carg.argv = argv;
  /* const duf_longval_extended_vtable_t * const *xvtables
   * cli->xvtable_multi = xvtables;
   * */
    cli->xvtable_multi = duf_cli_options_xtable_list2xvtable( cli, xtable_list ); /* allocates */
    cli->shorts = duf_cli_option_shorts_create( cli, cli->xvtable_multi );
    cli->varfunc = varfunc;

    cli->longopts_table = duf_options_create_longopts_table( cli->xvtable_multi );
    assert( cli->longopts_table );

    cli->config_dir = mas_strdup( config_dir );
    cli->cmds_dir = mas_strdup( commands_dir );
  }
/*
  TODO
    config_dir (duf_options_file.c)
    cmds_dir (duf_options_file.c)
    opt.output.history_filename (duf_options_interactive.c) =>  cli.history_filename
*/
}

void
duf_cli_options_delete( duf_config_cli_t * cli )
{
  duf_cli_options_shut( cli );
  mas_free( cli );
}

void
duf_cli_options_shut( duf_config_cli_t * cli )
{
  duf_options_delete_longopts_table( cli->longopts_table );
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
      duf_option_stage_t stage = DUF_OPTION_STAGE_NONE;
      duf_option_source_t source = {.sourcecode = DUF_OPTION_SOURCE_NONE };

      for ( size_t iod = 0; iod < cli->aod.count; iod++ )
      {
        duf_option_data_t *pod;

        pod = &cli->aod.pods[iod];
      /* T( "%lu. %s.pod %s => %s", iod, duf_optstage_name( pod->stage ), duf_optsource_name( pod->source ), pod->name ); */
        if ( source.sourcecode != pod->source.sourcecode )
          fprintf( f, "* SOURCE %s\n", duf_optsource_name( cli, ( source = pod->source ) ) );
        if ( stage != pod->stage )
          fprintf( f, "* STAGE %s\n", duf_optstage_name( cli, ( stage = pod->stage ) ) );
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
      duf_option_data_t *pod;

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