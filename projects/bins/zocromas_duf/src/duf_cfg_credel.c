/* #include <stdarg.h> */
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_config_util.h"

#include "duf_cli.h"

#include "duf_dbg.h"

#include "duf_expandable.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

#include "duf_option_config.h"
#include "duf_option_extended.h" /* duf_options_create_longopts_table */
#include "duf_option_names.h"
#include "duf_xtended_table.h"

/* #include "duf_config_ref.h" */
/* ###################################################################### */
#include "duf_cfg_credel.h"
/* ###################################################################### */

static duf_config_t *
duf_cfg_create_main( void )
{
  duf_config_t *cfg = NULL;

  assert( !cfg );
  cfg = mas_malloc( sizeof( duf_config_t ) );
  memset( cfg, 0, sizeof( duf_config_t ) );
  return cfg;
}

duf_config_t *
duf_cfg_create( void )
{
  duf_config_t *cfg = NULL;

  DEBUG_START(  );

  cfg = duf_cfg_create_main(  );

  cfg->scn.puz = duf_ufilter_create(  );
  assert( cfg->scn.puz );
#if 0
  {
    DUF_CFGWSP( cfg, db.dir, mas_strdup( getenv( "MSH_SHN_PROJECTS_DIR" ) ) );
    if ( DUF_CFGGSP( cfg, db.dir ) )
    {
      DUF_CFGWSP( cfg, db.dir, mas_strcat_x( DUF_CFGGSP( cfg, db.dir ), "/../duf_db" ) );
    }
  }
#endif
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      cfg->loadtime = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  {
    const char *cfgdir;

    cfgdir = getenv( DUF_CONFIG_DIR_FROM_ENV );
    /* DUF_TRACE( config, 0, "getting variable " DUF_CONFIG_DIR_FROM_ENV " value for config path : %s", cfgdir ); */
    DUF_CFGWS( cfg, config_dir, mas_strdup( cfgdir ) );

  }
  DUF_CFGWSP( cfg, db.main.name, mas_strdup( "duf-main" ) );
  DUF_CFGWSP( cfg, db.adm.name, mas_strdup( "duf-adm" ) );
  DUF_CFGWSP( cfg, db.tempo.name, mas_strdup( "duf-tempo" ) );
  DUF_CFGWSP( cfg, db.selected.name, mas_strdup( "duf-selected" ) );
  DUF_CFGW( cfg, opt.trace.errors ) += 2;
  /* DUF_CFGWN( cfg, opt.trace.any, DUF_CFGG( cfg, opt.trace.error ) ); */
  /* cfg->opt.trace.options = 1; */
  /* cfg->opt.trace.fs += 1; */

  /* DUF_CFGW( cfg, opt.trace.options ) = 11; */
  /* DUF_CFGW( cfg, opt.trace.options ) = 71; */
  DUF_CFGW( cfg, opt.trace.temp ) += 2;

  cfg->scn.pdi = duf_pdi_create( "selected" );
  assert( cfg->scn.puz );
  /* assert( cfg->cli.longopts_table ); */

  DEBUG_END(  );
  return cfg;
}

void
duf_cfg_delete( duf_config_t * cfg )
{
  DEBUG_START(  );

  if ( cfg )
  {
/* xchanges = di.changes; --- needless!? */
    duf_pdi_kill( &cfg->scn.pdi );

    duf_ufilter_delete( cfg->scn.puz );
    cfg->scn.puz = NULL;

    mas_free( cfg->config_dir );
    cfg->config_dir = NULL;

    {
      for ( size_t iod = 0; iod < cfg->aod.count; iod++ )
      {
        duf_option_data_t *pod;

        pod = &cfg->aod.pods[iod];
        mas_free( pod->xfound.array );
        pod->xfound.array = NULL;
        mas_free( pod->name );
        pod->name = NULL;
        mas_free( pod->optarg );
        pod->optarg = NULL;
        mas_free( pod->string_copy );
        pod->string_copy = NULL;
      }
      mas_free( cfg->aod.pods );
      cfg->aod.pods = NULL;
      cfg->aod.size = cfg->aod.count = 0;
    }
    mas_free( cfg->cmds_dir );
    cfg->cmds_dir = NULL;

#if 0
    mas_free( cfg->config_file_path );
    cfg->config_file_path = NULL;
#endif

#if 0
    mas_free( cfg->db.dir );
    cfg->db.dir = NULL;

    mas_free( cfg->db.subdir );
    cfg->db.subdir = NULL;

    mas_free( cfg->db.main.name );
    cfg->db.main.name = NULL;

    mas_free( cfg->db.adm.name );
    cfg->db.adm.name = NULL;

    mas_free( cfg->db.tempo.name );
    cfg->db.tempo.name = NULL;

    mas_free( cfg->db.selected.name );
    cfg->db.selected.name = NULL;
#else
    mas_free( cfg->db.path );
    cfg->db.path = NULL;
    duf_cfg_string_delete( DUF_CFGA( cfg, db.dir ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.subdir ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.main.name ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.adm.name ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.tempo.name ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.selected.name ) );
#endif

    mas_free( cfg->db.main.fpath );
    cfg->db.main.fpath = NULL;

    mas_free( cfg->db.adm.fpath );
    cfg->db.adm.fpath = NULL;

    mas_free( cfg->db.tempo.fpath );
    cfg->db.tempo.fpath = NULL;

    mas_free( cfg->db.selected.fpath );
    cfg->db.selected.fpath = NULL;

    /* duf_options_delete_longopts_table( cfg->cli.longopts_table ); */
    /* cfg->cli.longopts_table = NULL;                               */

    mas_free( cfg->help_string );
    cfg->help_string = NULL;

#if 0
    mas_free( cfg->db.opened_name );
    cfg->db.opened_name = NULL;
#endif
    mas_free( cfg->save.path );
    cfg->save.path = NULL;

    mas_free( cfg->tag.file );
    cfg->tag.file = NULL;
    mas_free( cfg->tag.dir );
    cfg->tag.dir = NULL;

    /* mas_free( cfg->group ); */
    /* cfg->group = NULL;      */

#if 0
    mas_del_argv( cfg->cli.targ.argc, cfg->cli.targ.argv, 0 );
    cfg->cli.targ.argc = 0;
    cfg->cli.targ.argv = NULL;

    mas_free( cfg->cli.shorts );
    cfg->cli.shorts = NULL;
#else
    duf_cli_shut( &cfg->cli );
#endif
    mas_free( cfg->opt.trace.output.file );
    cfg->opt.trace.output.file = NULL;

    mas_free( cfg->opt.output.file );
    cfg->opt.output.file = NULL;

    {
      mas_del_argv( cfg->opt.output.as_formats.tree.files.argc, cfg->opt.output.as_formats.tree.files.argv, 0 );
      mas_del_argv( cfg->opt.output.as_formats.tree.dirs.argc, cfg->opt.output.as_formats.tree.dirs.argv, 0 );
      mas_del_argv( cfg->opt.output.as_formats.list.files.argc, cfg->opt.output.as_formats.list.files.argv, 0 );
      mas_del_argv( cfg->opt.output.as_formats.list.dirs.argc, cfg->opt.output.as_formats.list.dirs.argv, 0 );
      mas_del_argv( cfg->opt.output.as_formats.gen.files.argc, cfg->opt.output.as_formats.gen.files.argv, 0 );
      mas_del_argv( cfg->opt.output.as_formats.gen.dirs.argc, cfg->opt.output.as_formats.gen.dirs.argv, 0 );
    }
    {
      mas_free( cfg->opt.output.sformat.files_tree );
      cfg->opt.output.sformat.files_tree = NULL;

      mas_free( cfg->opt.output.sformat.dirs_tree );
      cfg->opt.output.sformat.dirs_tree = NULL;

      mas_free( cfg->opt.output.sformat.prefix_gen_tree );
      cfg->opt.output.sformat.prefix_gen_tree = NULL;

      mas_free( cfg->opt.output.sformat.prefix_files_tree );
      cfg->opt.output.sformat.prefix_files_tree = NULL;

      mas_free( cfg->opt.output.sformat.prefix_dirs_tree );
      cfg->opt.output.sformat.prefix_dirs_tree = NULL;

      mas_free( cfg->opt.output.sformat.files_list );
      cfg->opt.output.sformat.files_list = NULL;

      mas_free( cfg->opt.output.sformat.dirs_list );
      cfg->opt.output.sformat.dirs_list = NULL;

      mas_free( cfg->opt.output.sformat.files_gen );
      cfg->opt.output.sformat.files_gen = NULL;

      mas_free( cfg->opt.output.sformat.dirs_gen );
      cfg->opt.output.sformat.dirs_gen = NULL;

      /* mas_free( cfg->opt.output.history_filename ); */
      /* cfg->opt.output.history_filename = NULL;      */

      mas_free( cfg->opt.output.header_tty );
      cfg->opt.output.header_tty = NULL;
    }
    mas_free( cfg );
    cfg = NULL;
  }
  DEBUG_END(  );
}
