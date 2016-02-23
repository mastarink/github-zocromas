#include <string.h>
#include <sys/time.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_config_defs.h"

#include "duf_expandable.h"

#include "duf_ufilter.h"

#include "duf_option_names.h"
#include "duf_option_config_credel.h"

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

  DUF_START(  );

  cfg = duf_cfg_create_main(  );

  cfg->vars.puz = duf_ufilter_create(  );
  assert( cfg->vars.puz );
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
    const char *cfgdir;

    cfgdir = getenv( DUF_CONFIG_DIR_FROM_ENV );
  /* DUF_TRACE( config, 0, "getting variable " DUF_CONFIG_DIR_FROM_ENV " value for config path : %s", cfgdir ); */
    DUF_CFGWS( cfg, conf.config_dir, mas_strdup( cfgdir ) );

  }
  assert( !cfg->db.main.name_x.value );
  DUF_CFGWSP( cfg, db.main.name_x, mas_strdup( "duf-main" ) );
  DUF_CFGWSP( cfg, db.adm.name_x, mas_strdup( "duf-adm" ) );
  DUF_CFGWSP( cfg, db.tempo.name_x, mas_strdup( "duf-tempo" ) );
  DUF_CFGWSP( cfg, db.selected.name_x, mas_strdup( "duf-selected" ) );
 
#if 0
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      cfg->opt.ptracecfg->loadtime = /* cfg->loadtime = */ ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  DUF_CFGW( cfg, opt.ptracecfg->errors ) += 2;
/* DUF_CFGWN( cfg, opt.ptracecfg->any, DUF_CFGG( cfg, opt.ptracecfg->error ) ); */
/* cfg->opt.ptracecfg->options = 1; */
/* cfg->opt.ptracecfg->fs += 1; */

/* DUF_CFGW( cfg, opt.ptracecfg->options ) = 11; */
/* DUF_CFGW( cfg, opt.ptracecfg->options ) = 71; */
  DUF_CFGW( cfg, opt.ptracecfg->temp ) += 2;
#endif

  assert( cfg->vars.puz );
/* assert( cfg->cli.longopts_table ); */

  DUF_END(  );
  return cfg;
}

void
duf_cfg_delete( duf_config_t * cfg )
{
  DUF_START(  );

  if ( cfg )
  {
/* xchanges = di.changes; --- needless!? */

    duf_ufilter_delete( cfg->vars.puz );
    cfg->vars.puz = NULL;

    mas_free( cfg->conf.config_dir );
    cfg->conf.config_dir = NULL;

    mas_free( cfg->conf.cmds_dir );
    cfg->conf.cmds_dir = NULL;

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
    duf_cfg_string_delete( DUF_CFGA( cfg, db.dir_x ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.subdir_x ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.main.name_x ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.adm.name_x ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.tempo.name_x ) );
    duf_cfg_string_delete( DUF_CFGA( cfg, db.selected.name_x ) );
#endif

    mas_free( cfg->db.main.fpath );
    cfg->db.main.fpath = NULL;

    mas_free( cfg->db.adm.fpath );
    cfg->db.adm.fpath = NULL;

    mas_free( cfg->db.tempo.fpath );
    cfg->db.tempo.fpath = NULL;

    mas_free( cfg->db.selected.fpath );
    cfg->db.selected.fpath = NULL;

    duf_cli_options_delete( cfg->pcli );

    mas_free( cfg->help_string );
    cfg->help_string = NULL;

#if 0
    mas_free( cfg->db.opened_name );
    cfg->db.opened_name = NULL;
#endif
    mas_free( cfg->save.path );
    cfg->save.path = NULL;

    mas_free( cfg->vars.tag.file );
    cfg->vars.tag.file = NULL;
    mas_free( cfg->vars.tag.dir );
    cfg->vars.tag.dir = NULL;

  /* mas_free( cfg->group ); */
  /* cfg->group = NULL;      */

#if 0
    mas_free( cfg->opt.ptracecfg->output.file );
    cfg->opt.ptracecfg->output.file = NULL;
#endif

    mas_free( cfg->opt.output.stream.file );
    cfg->opt.output.stream.file = NULL;

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

      mas_free( cfg->opt.output.stream.header_tty );
      cfg->opt.output.stream.header_tty = NULL;
    }
    mas_free( cfg );
    cfg = NULL;
  }
  DUF_END(  );
}
