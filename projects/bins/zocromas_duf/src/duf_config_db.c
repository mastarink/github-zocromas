#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

/* ###################################################################### */
#include "duf_config_db.h"
/* ###################################################################### */

SR( CONFIG, config_make_db_main_path, void )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", DUF_CONFIGGSP( db.dir_x ),
              DUF_CONFIGGSP( db.main.name_x ) );
/* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
/* MAST_TRACE( action, 4, "db.dir:%s; db.name:%s", DUF_CONFIGGSP(db.dir), DUF_CONFIGGSP(db.main.name) ); */
  mas_free( DUF_CONFIGW( db.main.fpath ) );
/* TODO to use something like duf_expand_selected_db() here TODO */

/* DUF_CONFIGWS( db.main.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
  DUF_CONFIGWS( db.main.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.main.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  DUF_CONFIGWS( db.main.fpath, mas_strcat_x( DUF_CONFIGG( db.main.fpath ), DUF_CONFIGGSP( db.main.name_x ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.main.fpath + strlen( DUF_CONFIGG( db.main.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.main.fpath, mas_strcat_x( DUF_CONFIGG( db.main.fpath ), ".db" ) );
  MAST_TRACE( db, 0, "@@@@@main.fpath:%s", DUF_CONFIGG( db.main.fpath ) );
  MAST_TRACE( explain, 0, "config->db.main.fpath set: %s", DUF_CONFIGG( db.main.fpath ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_main_path, void );
}

SR( CONFIG, config_make_db_adm_path, void )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.adm.fpath by db.dir: %s and db.adm.name: %s", DUF_CONFIGGSP( db.dir_x ),
              DUF_CONFIGGSP( db.adm.name_x ) );
  mas_free( DUF_CONFIGW( db.adm.fpath ) );
/* DUF_CONFIGWS( db.adm.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
/* DUF_CONFIGWS( db.adm.fpath, mas_normalize_path( DUF_CONFIGG( db.adm.fpath ) ) ); */
  DUF_CONFIGWS( db.adm.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.adm.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  {
    const char *n;

    n = strchr( DUF_CONFIGGSP( db.main.name_x ), '.' );
    if ( !n )
      n = DUF_CONFIGGSP( db.main.name_x ) + strlen( DUF_CONFIGGSP( db.main.name_x ) );
    DUF_CONFIGWS( db.adm.fpath, mas_strncat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGGSP( db.main.name_x ), n - DUF_CONFIGGSP( db.main.name_x ) ) );
    DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), "_$_" ) );
  }
  DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGGSP( db.adm.name_x ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.adm.fpath + strlen( DUF_CONFIGG( db.adm.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.adm.fpath, mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), ".db" ) );
  MAST_TRACE( db, 0, "@@@@@adm.fpath:%s", DUF_CONFIGG( db.adm.fpath ) );
  MAST_TRACE( explain, 0, "config->db.adm.fpath set: %s", DUF_CONFIGG( db.adm.fpath ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_adm_path, void );
}

SR( CONFIG, config_make_db_temp_path, void )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.tempo.fpath by db.dir: %s and db.tempo.name: %s", DUF_CONFIGGSP( db.dir_x ),
              DUF_CONFIGGSP( db.tempo.name_x ) );
  mas_free( DUF_CONFIGW( db.tempo.fpath ) );
/* DUF_CONFIGWS( db.tempo.fpath, mas_strdup( DUF_CONFIGGS( db.path ) ) ); */
/* DUF_CONFIGWS( db.tempo.fpath, mas_normalize_path( DUF_CONFIGG( db.tempo.fpath ) ) ); */
  DUF_CONFIGWS( db.tempo.fpath, mas_normalize_path( DUF_CONFIGG( db.path ) ) );
  {
    int ry;
    struct stat st;

    ry = stat( DUF_CONFIGG( db.tempo.fpath ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  {
    const char *n;

    n = strchr( DUF_CONFIGGSP( db.main.name_x ), '.' );
    if ( !n )
      n = DUF_CONFIGGSP( db.main.name_x ) + strlen( DUF_CONFIGGSP( db.main.name_x ) );
    DUF_CONFIGWS( db.tempo.fpath,
                  mas_strncat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGGSP( db.main.name_x ), n - DUF_CONFIGGSP( db.main.name_x ) ) );
    DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), "_$_" ) );
  }
  DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGGSP( db.tempo.name_x ) ) );
  if ( 0 != strcmp( DUF_CONFIGG( db.tempo.fpath + strlen( DUF_CONFIGG( db.tempo.fpath ) ) - 3 ), ".db" ) )
    DUF_CONFIGWS( db.tempo.fpath, mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), ".db" ) );
  MAST_TRACE( db, 0, "@@@@@tempo.fpath:%s", DUF_CONFIGG( db.tempo.fpath ) );
  MAST_TRACE( explain, 0, "config->db.tempo.fpath set: %s", DUF_CONFIGG( db.tempo.fpath ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_temp_path, void );
}

SR( CONFIG, config_make_db_paths, void )
{
/* DUF_STARTR( r ); */
  MAST_TRACE( db, 0, "@@@@@db.dir:%s", DUF_CONFIGGSP( db.dir_x ) );
  assert( !duf_config->db.path );
  DUF_CONFIGWS( db.path, duf_config_db_path_add_subdir( DUF_CONFIGGSP( db.dir_x ), QPERRIND ) );

  MAST_TRACE( db, 0, "@@@@@db.path:%s", DUF_CONFIGGS( db.path ) );
  if ( DUF_CONFIGGS( db.path ) && DUF_CONFIGGSP( db.main.name_x ) )
  {
    CR( config_make_db_main_path );

#ifdef MAS_SPLIT_DB
    if ( DUF_CONFIGGSP( db.adm.name_x ) )
      CR( config_make_db_adm_path );
# ifndef DUF_SQL_TTABLES_TEMPORARY
    if ( DUF_CONFIGGSP( db.tempo.name_x ) )
      CR( config_make_db_temp_path );
# endif
#endif
  /* assert( DUF_CONFIGG( db.main.fpath ) ); */
    {
      MAST_TRACE( db, 0, "dbfile: %s", DUF_CONFIGG( db.main.fpath ) );
#ifdef MAS_SPLIT_DB
      MAST_TRACE( db, 0, "adm dbfile: %s", DUF_CONFIGG( db.adm.fpath ) );
# ifndef DUF_SQL_TTABLES_TEMPORARY
      MAST_TRACE( db, 0, "tempo dbfile: %s", DUF_CONFIGG( db.tempo.fpath ) );
# endif
    /* MAST_TRACE( any, 0, "selected dbfile: %s", DUF_CONFIGG(db.selected.fpath) ); */
#endif
    }
  }
  else if ( !DUF_CONFIGGS( db.path ) )
  {
  /* DUF_SHOW_ERROR( "db.path not set" ); */
    ERRMAKE_M( PTR, "db.path not set" );
  }
  else if ( !DUF_CONFIGGSP( db.main.name_x ) )
  {
  /* DUF_SHOW_ERROR( "db.main.name not set" ); */
    ERRMAKE_M( PTR, "db.main.name not set" );
  }
  else
    ERRMAKE( UNKNOWN );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_paths, void );
}

const char *
duf_get_config_db_main_fpath( void )
{
  return DUF_CONFIGGS( db.main.fpath );
}

const char *
duf_get_config_db_adm_fpath( void )
{
  return DUF_CONFIGGS( db.adm.fpath );
}

const char *
duf_get_config_db_tempo_fpath( void )
{
  return DUF_CONFIGGS( db.tempo.fpath );
}

const char *
duf_get_config_db_main_name( void )
{
  return DUF_CONFIGGSP( db.main.name_x );
}

const char *
duf_get_config_db_path( void )
{
  return DUF_CONFIGGS( db.path );
}
