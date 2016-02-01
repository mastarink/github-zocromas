/* #include <stdarg.h> */
#include <string.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_expandable.h"

/* ###################################################################### */
#include "duf_config_db.h"
/* ###################################################################### */


int
duf_config_make_db_main_path( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", DUF_CONFIGGSP( db.dir ), DUF_CONFIGGSP( db.main.name ) );
  /* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
  /* DUF_TRACE( action, 4, "db.dir:%s; db.name:%s", DUF_CONFIGGSP(db.dir), DUF_CONFIGGSP(db.main.name) ); */
  mas_free( DUF_CONFIGW( db.main.fpath ) );
  /* TODO to use something like duf_expand_selected_db() here TODO */

  /* DUF_CONFIGWS( db.main.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
  DUF_CONFIGWS( db.main.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
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
  /* DUF_CONFIGWS( db.adm.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
  /* DUF_CONFIGWS( db.adm.fpath, mas_normalize_path( DUF_CONFIGG( db.adm.fpath ) ) ); */
  DUF_CONFIGWS( db.adm.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
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
  /* DUF_CONFIGWS( db.tempo.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
  /* DUF_CONFIGWS( db.tempo.fpath, mas_normalize_path( DUF_CONFIGG( db.tempo.fpath ) ) ); */
  DUF_CONFIGWS( db.tempo.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
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
    /* assert( DUF_CONFIGG( db.main.fpath ) ); */
    {
      DUF_TRACE( db, 0, "dbfile: %s", DUF_CONFIGG( db.main.fpath ) );
#ifdef MAS_SPLIT_DB
      DUF_TRACE( db, 0, "adm dbfile: %s", DUF_CONFIGG( db.adm.fpath ) );
#  ifndef DUF_SQL_TTABLES_TEMPORARY
      DUF_TRACE( db, 0, "tempo dbfile: %s", DUF_CONFIGG( db.tempo.fpath ) );
#  endif
      /* DUF_TRACE( any, 0, "selected dbfile: %s", DUF_CONFIGG(db.selected.fpath) ); */
#endif
    }
  }
  else if ( !DUF_CONFIGGS( db.path ) )
  {
    /* DUF_SHOW_ERROR( "db.path not set" ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_PTR, "db.path not set" );
  }
  else if ( !DUF_CONFIGGSP( db.main.name ) )
  {
    /* DUF_SHOW_ERROR( "db.main.name not set" ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_PTR, "db.main.name not set" );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );

  DEBUG_ENDR( r );
}
