#include <assert.h>
#include <string.h>
#include <errno.h>

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

/* #include "duf_config.h"                                              (* duf_get_config ✗ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config_db_get.h"

/* ###################################################################### */
#include "duf_config_db.h"
/* ###################################################################### */

SR( CONFIG, config_make_db_main_path, void )
{
  duf_config_db_t *db_cfg = duf_get_config_db(  );

/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", duf_get_config_db_dir(  ),
              duf_get_config_db_main_name(  ) );
/* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
/* MAST_TRACE( action, 4, "db.dir:%s; db.name:%s", DUF_CONFIGGSP(db.dir), DUF_CONFIGGSP(db.main.name) ); */
  mas_free( db_cfg->main.fpath );
/* TODO to use something like duf_expand_selected_db() here TODO */

/* db_cfg->main.fpath=mas_strdup( duf_get_config_db_path() ) ; */
  db_cfg->main.fpath = mas_normalize_path( duf_get_config_db_path(  ) );
  {
    int ry;
    struct stat st;

    ry = stat( duf_get_config_db_main_fpath(  ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  db_cfg->main.fpath = mas_strcat_x( db_cfg->main.fpath, duf_get_config_db_main_name(  ) );
  if ( 0 != strcmp( db_cfg->main.fpath + strlen( db_cfg->main.fpath ) - 3, ".db" ) )
    db_cfg->main.fpath = mas_strcat_x( db_cfg->main.fpath, ".db" );
  MAST_TRACE( db, 0, "@@@@@main.fpath:%s", duf_get_config_db_main_fpath(  ) );
  MAST_TRACE( explain, 0, "config->db.main.fpath set: %s", duf_get_config_db_main_fpath(  ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_main_path, void );
}

SR( CONFIG, config_make_db_adm_path, void )
{
  duf_config_db_t *db_cfg = duf_get_config_db(  );

/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.adm.fpath by db.dir: %s and db.adm.name: %s", duf_get_config_db_dir(  ),
              duf_get_config_db_adm_name(  ) );
  mas_free( db_cfg->adm.fpath );
/* db_cfg->adm.fpath=mas_strdup( duf_get_config_db_path() ) ; */
/* db_cfg->adm.fpath=mas_normalize_path( duf_get_config_db_adm_fpath() ) ; */
  db_cfg->adm.fpath = mas_normalize_path( duf_get_config_db_path(  ) );
  {
    int ry;
    struct stat st;

    ry = stat( duf_get_config_db_adm_fpath(  ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  {
    const char *n;

    n = strchr( duf_get_config_db_main_name(  ), '.' );
    if ( !n )
      n = duf_get_config_db_main_name(  ) + strlen( duf_get_config_db_main_name(  ) );
    db_cfg->adm.fpath = mas_strncat_x( db_cfg->adm.fpath, duf_get_config_db_main_name(  ), n - duf_get_config_db_main_name(  ) );
    db_cfg->adm.fpath = mas_strcat_x( db_cfg->adm.fpath, "_$_" );
  }
  db_cfg->adm.fpath = mas_strcat_x( db_cfg->adm.fpath, duf_get_config_db_adm_name(  ) );
  if ( 0 != strcmp( db_cfg->adm.fpath + strlen( db_cfg->adm.fpath ) - 3, ".db" ) )
    db_cfg->adm.fpath = mas_strcat_x( db_cfg->adm.fpath, ".db" );
  MAST_TRACE( db, 0, "@@@@@adm.fpath:%s", duf_get_config_db_adm_fpath(  ) );
  MAST_TRACE( explain, 0, "config->db.adm.fpath set: %s", duf_get_config_db_adm_fpath(  ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_adm_path, void );
}

SR( CONFIG, config_make_db_temp_path, void )
{
  duf_config_db_t *db_cfg = duf_get_config_db(  );

/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 0, "setting config->db.tempo.fpath by db.dir: %s and db.tempo.name: %s", duf_get_config_db_dir(  ),
              duf_get_config_db_tempo_name(  ) );
  mas_free( db_cfg->tempo.fpath );
/* db_cfg->tempo.fpath=mas_strdup( duf_get_config_db_path() ) ; */
/* db_cfg->tempo.fpath=mas_normalize_path( duf_get_config_db_tempo_fpath() ) ; */
  db_cfg->tempo.fpath = mas_normalize_path( duf_get_config_db_path(  ) );
  {
    int ry;
    struct stat st;

    ry = stat( duf_get_config_db_tempo_fpath(  ), &st );
    if ( ry < 0 || !S_ISDIR( st.st_mode ) )
      ERRMAKE( STAT );
  }

  {
    const char *n;

    n = strchr( duf_get_config_db_main_name(  ), '.' );
    if ( !n )
      n = duf_get_config_db_main_name(  ) + strlen( duf_get_config_db_main_name(  ) );
    db_cfg->tempo.fpath = mas_strncat_x( db_cfg->tempo.fpath, duf_get_config_db_main_name(  ), n - duf_get_config_db_main_name(  ) );
    db_cfg->tempo.fpath = mas_strcat_x( db_cfg->tempo.fpath, "_$_" );
  }
  db_cfg->tempo.fpath = mas_strcat_x( db_cfg->tempo.fpath, duf_get_config_db_tempo_name(  ) );
  if ( 0 != strcmp( db_cfg->tempo.fpath + strlen( db_cfg->tempo.fpath ) - 3, ".db" ) )
    db_cfg->tempo.fpath = mas_strcat_x( db_cfg->tempo.fpath, ".db" );
  MAST_TRACE( db, 0, "@@@@@tempo.fpath:%s", duf_get_config_db_tempo_fpath(  ) );
  MAST_TRACE( explain, 0, "config->db.tempo.fpath set: %s", duf_get_config_db_tempo_fpath(  ) );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_temp_path, void );
}

SR( CONFIG, config_make_db_paths, void )
{
  duf_config_db_t *db_cfg = duf_get_config_db(  );

/* DUF_STARTR( r ); */
  MAST_TRACE( db, 0, "@@@@@db.dir:%s", duf_get_config_db_dir(  ) );
  assert( !duf_get_config_db_path(  ) );
  db_cfg->path = duf_config_db_path_add_subdir( duf_get_config_db_dir(  ), QPERRIND );

  MAST_TRACE( db, 0, "@@@@@db.path:%s", duf_get_config_db_path(  ) );
  if ( duf_get_config_db_path(  ) && duf_get_config_db_main_name(  ) )
  {
    CR( config_make_db_main_path );

#ifdef MAS_SPLIT_DB
    if ( duf_get_config_db_adm_name(  ) )
      CR( config_make_db_adm_path );
# ifndef DUF_SQL_TTABLES_TEMPORARY
    if ( duf_get_config_db_tempo_name(  ) )
      CR( config_make_db_temp_path );
# endif
#endif
  /* assert( duf_get_config_db_main_fpath() ); */
    {
      MAST_TRACE( db, 0, "dbfile: %s", duf_get_config_db_main_fpath(  ) );
#ifdef MAS_SPLIT_DB
      MAST_TRACE( db, 0, "adm dbfile: %s", duf_get_config_db_adm_fpath(  ) );
# ifndef DUF_SQL_TTABLES_TEMPORARY
      MAST_TRACE( db, 0, "tempo dbfile: %s", duf_get_config_db_tempo_fpath(  ) );
# endif
    /* MAST_TRACE( any, 0, "selected dbfile: %s", duf_get_config_db_selected_fpath() ); */
#endif
    }
  }
  else if ( !duf_get_config_db_path(  ) )
  {
  /* DUF_SHOW_ERROR( "db.path not set" ); */
    ERRMAKE_M( PTR, "db.path not set" );
  }
  else if ( !duf_get_config_db_main_name(  ) )
  {
  /* DUF_SHOW_ERROR( "db.main.name not set" ); */
    ERRMAKE_M( PTR, "db.main.name not set" );
  }
  else
    ERRMAKE( UNKNOWN );

/* DUF_ENDR( r ); */
  ER( CONFIG, config_make_db_paths, void );
}

/* char *                                                    */
/* duf_config_db_path_add_subdir( const char *dir, int *pr ) */
SRP( OTHER, char *, path, NULL, config_db_path_add_subdir, const char *dir )
{
/* int rpr = 0; */
/* char *path = NULL; */

  if ( duf_get_config_db_subdir(  ) )
  {
    if ( strchr( duf_get_config_db_subdir(  ), '/' ) )
    {
      ERRMAKE( MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, duf_get_config_db_subdir(  ) );
      {
        struct stat st;

        ry = stat( path, &st );
        if ( ry < 0 )
        {
          if ( errno == ENOENT )
          {
            ry = mkdir( path, S_IRWXU );

            if ( ry < 0 )
            {
              char serr[1024] = "";
              char *s;

              s = strerror_r( errno, serr, sizeof( serr ) );
              ERRMAKE_M( MKDIR, "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
            /* DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
            }
          }
        }
        else if ( !S_ISDIR( st.st_mode ) )
        {
          ERRMAKE( STAT );
        }
      }
    }
  }
/* if ( pr )    */
/*   *pr = rpr; */
/* return path; */
  ERP( OTHER, char *, path, NULL, config_db_path_add_subdir, const char *dir );
}
