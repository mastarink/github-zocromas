/* #include <stdarg.h> */
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_dbg.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

#include "duf_option_extended.h"
#include "duf_option_names.h"

#include "duf_utils_path.h"
#include "duf_config_ref.h"
/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;
const duf_config_t *duf_config4trace = NULL;

duf_config_t *
duf_cfg_create( void )
{
  duf_config_t *cfg = NULL;

  DEBUG_START(  );

  assert( !cfg );
  cfg = mas_malloc( sizeof( duf_config_t ) );
  memset( cfg, 0, sizeof( duf_config_t ) );

  cfg->puz = duf_ufilter_create(  );
  assert( cfg->puz );
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
  DUF_CFGWSP( cfg, db.main.name, mas_strdup( "duf-main" ) );
  DUF_CFGWSP( cfg, db.adm.name, mas_strdup( "duf-adm" ) );
  DUF_CFGWSP( cfg, db.tempo.name, mas_strdup( "duf-tempo" ) );
  DUF_CFGWSP( cfg, db.selected.name, mas_strdup( "duf-selected" ) );
  DUF_CFGWN( cfg, cli.trace.any, DUF_CFGW( cfg, cli.trace.error ) += 1 );
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
  assert( cfg->puz );
  assert( cfg->longopts_table );




  DEBUG_END(  );
  return cfg;
}

void
duf_cfg_string_delete( duf_config_string_t * cs )
{
  DEBUG_START(  );
  mas_free( cs->value );
  cs->value = NULL;
  mas_free( cs->expanded );
  cs->expanded = NULL;
  DEBUG_END(  );
}

void
duf_cfg_delete( duf_config_t * cfg )
{
  DEBUG_START(  );

  if ( cfg )
  {
/* xchanges = di.changes; --- needless!? */
    duf_pdi_kill( &cfg->pdi );

    duf_ufilter_delete( cfg->puz );
    cfg->puz = NULL;

    mas_free( cfg->config_path );
    cfg->config_path = NULL;

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

    mas_free( cfg->longopts_table );
    cfg->longopts_table = NULL;

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
      mas_free( cfg->cli.output.sformat.files_tree );
      cfg->cli.output.sformat.files_tree = NULL;

      mas_free( cfg->cli.output.sformat.dirs_tree );
      cfg->cli.output.sformat.dirs_tree = NULL;

      mas_free( cfg->cli.output.sformat.prefix_gen_tree );
      cfg->cli.output.sformat.prefix_gen_tree = NULL;

      mas_free( cfg->cli.output.sformat.prefix_files_tree );
      cfg->cli.output.sformat.prefix_files_tree = NULL;

      mas_free( cfg->cli.output.sformat.prefix_dirs_tree );
      cfg->cli.output.sformat.prefix_dirs_tree = NULL;

      mas_free( cfg->cli.output.sformat.files_list );
      cfg->cli.output.sformat.files_list = NULL;

      mas_free( cfg->cli.output.sformat.dirs_list );
      cfg->cli.output.sformat.dirs_list = NULL;

      mas_free( cfg->cli.output.sformat.files_gen );
      cfg->cli.output.sformat.files_gen = NULL;

      mas_free( cfg->cli.output.sformat.dirs_gen );
      cfg->cli.output.sformat.dirs_gen = NULL;

      mas_free( cfg->cli.output.history_filename );
      cfg->cli.output.history_filename = NULL;

      mas_free( cfg->cli.output.header_tty );
      cfg->cli.output.header_tty = NULL;
    }

    mas_free( cfg );
    cfg = NULL;
  }
  DEBUG_END(  );
}

void
duf_config_create( int argc, char **argv )
{
  DEBUG_START(  );
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
  DEBUG_END(  );
}

void
duf_config_delete( void )
{
  DEBUG_START(  );
  duf_cfg_delete( duf_config );
#ifdef MAS_TRACING
  duf_config4trace = duf_config = NULL;
#endif
  DEBUG_END(  );
}

int
duf_config_show( void )
{
  DEBUG_STARTR( r );

  if ( duf_config )
  {
    DUF_FPRINTF( 0, stderr, "db.dir: %s\n", DUF_CONFIGGSP( db.dir ) );
    DUF_FPRINTF( 0, stderr, "db.path: %s\n", DUF_CONFIGGS( db.path ) );
  }
  for ( int ia = 0; ia < duf_config->targ.argc; ia++ )
    DUF_FPRINTF( 0, stderr, "targ.argv[%d]: %s\n", ia, duf_config->targ.argv[ia] );

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

#if 0
  /* Obsolete ??? */
  DUF_TRACE( temporary, 0, "@ maxitems.total %lld", DUF_CONFIGG( puz )->maxitems.total );
  DUF_TRACE( temporary, 0, "@ maxitems.files %lld", DUF_CONFIGG( puz )->maxitems.files );
  DUF_TRACE( temporary, 0, "@ maxitems.dirs %lld", DUF_CONFIGG( puz )->maxitems.dirs );
  DUF_TRACE( temporary, 0, "@ dirfiles.min %u", DUF_CONFIGG( puz )->dirfiles.min );
  DUF_TRACE( temporary, 0, "@ dirfiles.max %u", DUF_CONFIGG( puz )->dirfiles.max );
#endif

#ifdef MAS_TRACING
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->puz->globx.include_fs_files.argc, duf_config->puz->globx.include_fs_files.argv, 0 );
    DUF_TRACE( temporary, 0, "@ include-fs %s", sif );
    mas_free( sif );
  }
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->puz->globx.exclude_fs_files.argc, duf_config->puz->globx.exclude_fs_files.argv, 0 );
    DUF_TRACE( temporary, 0, "@ exclude-fs %s", sif );
    mas_free( sif );
  }
#endif

  DEBUG_ENDR( r );
}


int
duf_config_make_db_main_path( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", DUF_CONFIGGSP( db.dir ), DUF_CONFIGGSP( db.main.name ) );
  /* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
  /* DUF_TRACE( action, 4, "db.dir:%s; db.name:%s", DUF_CONFIGGSP(db.dir), DUF_CONFIGGSP(db.main.name) ); */
  mas_free( DUF_CONFIGW( db.main.fpath ) );
  /* TODO to use something like duf_expand_selected_db() here TODO */
  DUF_CONFIGWS( db.main.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) );
  DUF_CONFIGWS( db.main.fpath, duf_normalize_path( DUF_CONFIGG( db.main.fpath ) ) );
  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.main.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }


  DUF_CONFIGWS( db.main.fpath, mas_strcat_x( DUF_CONFIGG( db.main.fpath ), DUF_CONFIGGSP( db.main.name ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.main.fpath + strlen( DUF_CONFIGG( db.main.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.main.fpath, mas_strcat_x( DUF_CONFIGG( db.main.fpath ), ".db" ) );
  DUF_TRACE( db, 0, "@@@@@main.fpath:%s", DUF_CONFIGG( db.main.fpath ) );
  DUF_TRACE( explain, 0, "config->db.main.fpath set: %s", DUF_CONFIGG( db.main.fpath ) );

  DEBUG_ENDR( r );
}

int
duf_config_make_db_adm_path( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( explain, 0, "setting config->db.adm.fpath by db.dir: %s and db.adm.name: %s", DUF_CONFIGGSP( db.dir ), DUF_CONFIGGSP( db.adm.name ) );
  mas_free( DUF_CONFIGW( db.adm.fpath ) );
  DUF_CONFIGWS( db.adm.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) );
  DUF_CONFIGWS( db.adm.fpath, duf_normalize_path( DUF_CONFIGG( db.adm.fpath ) ) );
  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.adm.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }


  {
    char *n;

    n = strchr( DUF_CONFIGGSP( db.main.name ), '.' );
    if ( !n )
      n = DUF_CONFIGGSP( db.main.name ) + strlen( DUF_CONFIGGSP( db.main.name ) );
    DUF_CONFIGWS( db.adm.fpath, mas_strncat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGGSP( db.main.name ), n - DUF_CONFIGGSP( db.main.name ) ) );
    DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), "_$_" ) );
  }
  DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGGSP( db.adm.name ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.adm.fpath + strlen( DUF_CONFIGG( db.adm.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), ".db" ) );
  DUF_TRACE( db, 0, "@@@@@adm.fpath:%s", DUF_CONFIGG( db.adm.fpath ) );
  DUF_TRACE( explain, 0, "config->db.adm.fpath set: %s", DUF_CONFIGG( db.adm.fpath ) );

  DEBUG_ENDR( r );
}

int
duf_config_make_db_temp_path( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( explain, 0, "setting config->db.tempo.fpath by db.dir: %s and db.tempo.name: %s", DUF_CONFIGGSP( db.dir ),
             DUF_CONFIGGSP( db.tempo.name ) );
  mas_free( DUF_CONFIGW( db.tempo.fpath ) );
  DUF_CONFIGWS( db.tempo.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) );
  DUF_CONFIGWS( db.tempo.fpath, duf_normalize_path( DUF_CONFIGG( db.tempo.fpath ) ) );

  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.tempo.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_STAT );
  }

  {
    char *n;

    n = strchr( DUF_CONFIGGSP( db.main.name ), '.' );
    if ( !n )
      n = DUF_CONFIGGSP( db.main.name ) + strlen( DUF_CONFIGGSP( db.main.name ) );
    DUF_CONFIGWS( db.tempo.fpath, mas_strncat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGGSP( db.main.name ), n - DUF_CONFIGGSP( db.main.name ) ) );
    DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), "_$_" ) );
  }
  DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGGSP( db.tempo.name ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.tempo.fpath + strlen( DUF_CONFIGG( db.tempo.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), ".db" ) );
  DUF_TRACE( db, 0, "@@@@@tempo.fpath:%s", DUF_CONFIGG( db.tempo.fpath ) );
  DUF_TRACE( explain, 0, "config->db.tempo.fpath set: %s", DUF_CONFIGG( db.tempo.fpath ) );

  DEBUG_ENDR( r );
}

int
duf_config_make_db_paths( void )
{
  DEBUG_STARTR( r );
  DUF_TRACE( db, 0, "@@@@@db.dir:%s", DUF_CONFIGGSP( db.dir ) );

  DUF_CONFIGWS( db.path, duf_config_db_path_add_subdir( DUF_CONFIGGSP( db.dir ), &r ) );

  DUF_TRACE( db, 0, "@@@@@db.path:%s", DUF_CONFIGGS( db.path ) );
  if ( DUF_CONFIGGS( db.path ) && DUF_CONFIGGSP( db.main.name ) )
  {
    DOR( r, duf_config_make_db_main_path(  ) );

#ifdef MAS_SPLIT_DB
    if ( DUF_CONFIGGSP( db.adm.name ) )
      DOR( r, duf_config_make_db_adm_path(  ) );
#  ifndef DUF_SQL_TTABLES_TEMPORARY
    if ( DUF_CONFIGGSP( db.tempo.name ) )
      DOR( r, duf_config_make_db_temp_path(  ) );
#  endif
#endif
    assert( DUF_CONFIGG( db.main.fpath ) );
    {
      DUF_TRACE( any, 0, "dbfile: %s", DUF_CONFIGG( db.main.fpath ) );
#ifdef MAS_SPLIT_DB
      DUF_TRACE( any, 0, "adm dbfile: %s", DUF_CONFIGG( db.adm.fpath ) );
#  ifndef DUF_SQL_TTABLES_TEMPORARY
      DUF_TRACE( any, 0, "tempo dbfile: %s", DUF_CONFIGG( db.tempo.fpath ) );
#  endif
      /* DUF_TRACE( any, 0, "selected dbfile: %s", DUF_CONFIGG(db.selected.fpath) ); */
#endif
    }
  }
  else if ( !DUF_CONFIGGS( db.path ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.path not set" );
  }
  else if ( !DUF_CONFIGGSP( db.main.name ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.main.name not set" );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );

  DEBUG_ENDR( r );
}
