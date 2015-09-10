/* #include <stdarg.h> */
#include <string.h>
#include <sys/time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_dbg.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

#include "duf_option_extended.h"
#include "duf_option_names.h"

/* #include "duf_config_ref.h" */
/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;
const duf_config_t *duf_config4trace = NULL;

duf_config_t *
duf_cfg_create( void )
{
  duf_config_t *cfg = NULL;

  assert( !cfg );
  cfg = mas_malloc( sizeof( duf_config_t ) );
  memset( cfg, 0, sizeof( duf_config_t ) );

  cfg->pu = duf_ufilter_create(  );
  assert( cfg->pu );
  if ( 0 )
  {
    cfg->db.dir = mas_strdup( getenv( "MSH_SHN_PROJECTS_DIR" ) );
    if ( cfg->db.dir )
    {
      cfg->db.dir = mas_strcat_x( cfg->db.dir, "/../duf_db" );
    }
  }
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      cfg->loadtime = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  cfg->db.main.name = mas_strdup( "duf-main.db" );
  cfg->db.adm.name = mas_strdup( "duf-adm.db" );
  cfg->db.tempo.name = mas_strdup( "duf-tempo.db" );
  cfg->db.selected.name = mas_strdup( "duf-selected.db" );
  cfg->cli.trace.any = cfg->cli.trace.error += 1;
  /* cfg->cli.trace.options = 1; */
  /* cfg->cli.trace.fs += 1; */
  cfg->cli.trace.temp += 1;

  {
    int tbcount = 0;
    size_t tbsize = 0;
    const duf_longval_extended_table_t **xtables;
    const duf_longval_extended_table_t *xtable;

    tbcount = duf_longindex_extended_count( lo_extended_table_multi );
    xtables = lo_extended_table_multi;
    tbsize = tbcount * ( sizeof( duf_longval_extended_t ) + 1 );
    {
      duf_option_t *longopts_ptr;

      cfg->longopts_table = longopts_ptr = mas_malloc( tbsize );
      memset( cfg->longopts_table, 0, tbsize );

      while ( ( xtable = *xtables++ ) )
      {
        const duf_longval_extended_t *xtended;

        xtended = xtable->table;
        while ( xtended->o.name )
        {
          {
            longopts_ptr->name = xtended->o.name;
            longopts_ptr->has_arg = xtended->o.has_arg;
            longopts_ptr->val = xtended->o.val;
            longopts_ptr++;
          }
          xtended++;
        }
      }

      /* for ( int ilong = 0; ilong < lo_extended_count; ilong++ )            */
      /* {                                                                    */
      /*   cfg->longopts_table[ilong].name = lo_extended[ilong].o.name;       */
      /*   cfg->longopts_table[ilong].has_arg = lo_extended[ilong].o.has_arg; */
      /*   cfg->longopts_table[ilong].val = lo_extended[ilong].o.val;         */
      /* }                                                                    */
    }
    /* assert(cfg->longopts_table); */
    /* {                                                                                                                                          */
    /*   DUF_PRINTF( 0, "%u -- %u", lo_extended_count, duf_longopts_count );                                                                      */
    /*   for ( int i = 0; i < duf_longopts_count && duf_longopts[i].name && cfg->longopts_table[i].name; i++ )                             */
    /*   {                                                                                                                                        */
    /*     if ( 0 != strcmp( duf_longopts[i].name, cfg->longopts_table[i].name )                                                           */
    /*          || duf_longopts[i].has_arg != cfg->longopts_table[i].has_arg || duf_longopts[i].val != cfg->longopts_table[i].val ) */
    /*     {                                                                                                                                      */
    /*       DUF_PRINTF( 0, "%d: %30s :: %30s", i, duf_longopts[i].name, cfg->longopts_table[i].name );                                    */
    /*     }                                                                                                                                      */
    /*   }                                                                                                                                        */
    /* }                                                                                                                                          */
  }
  cfg->pdi = duf_pdi_create( "selected" );
  assert( cfg->pu );
  assert( cfg->longopts_table );




  return cfg;
}

void
duf_cfg_delete( duf_config_t * cfg )
{
  if ( cfg )
  {
/* xchanges = di.changes; --- needless!? */
    duf_pdi_kill( &cfg->pdi );

    duf_ufilter_delete( cfg->pu );
    cfg->pu = NULL;

    mas_free( cfg->config_path );
    cfg->config_path = NULL;

    mas_free( cfg->db.dir );
    cfg->db.dir = NULL;

    mas_free( cfg->longopts_table );
    cfg->longopts_table = NULL;

    mas_free( cfg->help_string );
    cfg->help_string = NULL;

    mas_free( cfg->db.main.name );
    cfg->db.main.name = NULL;

    mas_free( cfg->db.main.fpath );
    cfg->db.main.fpath = NULL;

    mas_free( cfg->db.adm.name );
    cfg->db.adm.name = NULL;

    mas_free( cfg->db.adm.fpath );
    cfg->db.adm.fpath = NULL;

    mas_free( cfg->db.tempo.name );
    cfg->db.tempo.name = NULL;

    mas_free( cfg->db.tempo.fpath );
    cfg->db.tempo.fpath = NULL;

    mas_free( cfg->db.selected.name );
    cfg->db.selected.name = NULL;

    mas_free( cfg->db.selected.fpath );
    cfg->db.selected.fpath = NULL;

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

    mas_del_argv( cfg->targ.argc, cfg->targ.argv, 0 );
    cfg->targ.argc = 0;
    cfg->targ.argv = NULL;

    mas_free( cfg->cli.shorts );
    cfg->cli.shorts = NULL;

    mas_free( cfg->cli.trace.output.file );
    cfg->cli.trace.output.file = NULL;

    mas_free( cfg->cli.output.file );
    cfg->cli.output.file = NULL;

    {
      mas_del_argv( cfg->cli.output.as_formats.tree.files.argc, cfg->cli.output.as_formats.tree.files.argv, 0 );
      mas_del_argv( cfg->cli.output.as_formats.tree.dirs.argc, cfg->cli.output.as_formats.tree.dirs.argv, 0 );
      mas_del_argv( cfg->cli.output.as_formats.list.files.argc, cfg->cli.output.as_formats.list.files.argv, 0 );
      mas_del_argv( cfg->cli.output.as_formats.list.dirs.argc, cfg->cli.output.as_formats.list.dirs.argv, 0 );
      mas_del_argv( cfg->cli.output.as_formats.gen.files.argc, cfg->cli.output.as_formats.gen.files.argv, 0 );
      mas_del_argv( cfg->cli.output.as_formats.gen.dirs.argc, cfg->cli.output.as_formats.gen.dirs.argv, 0 );
    }
    {
      mas_free( cfg->cli.output.sformat_files_tree );
      cfg->cli.output.sformat_files_tree = NULL;

      mas_free( cfg->cli.output.sformat_dirs_tree );
      cfg->cli.output.sformat_dirs_tree = NULL;

      mas_free( cfg->cli.output.sformat_prefix_gen_tree );
      cfg->cli.output.sformat_prefix_gen_tree = NULL;

      mas_free( cfg->cli.output.sformat_prefix_files_tree );
      cfg->cli.output.sformat_prefix_files_tree = NULL;

      mas_free( cfg->cli.output.sformat_prefix_dirs_tree );
      cfg->cli.output.sformat_prefix_dirs_tree = NULL;

      mas_free( cfg->cli.output.sformat_files_list );
      cfg->cli.output.sformat_files_list = NULL;

      mas_free( cfg->cli.output.sformat_dirs_list );
      cfg->cli.output.sformat_dirs_list = NULL;

      mas_free( cfg->cli.output.sformat_files_gen );
      cfg->cli.output.sformat_files_gen = NULL;

      mas_free( cfg->cli.output.history_filename );
      cfg->cli.output.history_filename = NULL;

      mas_free( cfg->cli.output.header );
      cfg->cli.output.header = NULL;

      mas_free( cfg->cli.output.sformat_dirs_gen );
      cfg->cli.output.sformat_dirs_gen = NULL;
    }

    mas_free( cfg );
    cfg = NULL;
  }
}

void
duf_config_create( int argc, char **argv )
{
  duf_config = duf_cfg_create(  );
  assert( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config;

  duf_config->carg.argc = argc;
  duf_config->carg.argv = argv;
  if ( !duf_config->cli.shorts )
    duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );




  assert( duf_config4trace );
#endif
}

void
duf_config_delete( void )
{
  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config = NULL;
#endif
}

int
duf_config_show( void )
{
  DEBUG_STARTR( r );

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config )
    fprintf( stderr, "db.dir: %s\n", duf_config->db.dir );
  for ( int ia = 0; ia < duf_config->targ.argc; ia++ )
    fprintf( stderr, "targ.argv[%d]: %s\n", ia, duf_config->targ.argv[ia] );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );

  DEBUG_ENDR( r );
}

int
duf_config_optionally_show( void )
{
  DEBUG_STARTR( r );

  if ( duf_config->cli.dbg.verbose )
  {
    DOR( r, duf_config_show(  ) );
  }
  else
  {
    DUF_TRACE( explain, 0, "not showing config: not verbose" );
  }

  DEBUG_ENDR( r );
}
