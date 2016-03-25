#include <assert.h>
#include <string.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */

#include <mastar/multiconfig/muc_option_names.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */
#include "duf_config_ref.h"
/* #include "duf_config_db.h" */
#include "duf_config_db_get.h"

#include "duf_optimpl_defs.h"                                        /* DUF_UG_FLAG; DUF_ACT_FLAG etc. ✗ */
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

#include "duf_config_structs.h"
/* ###################################################################### */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* ###################################################################### */

#if 0
static void *
duf_get_offset( void *ptr, unsigned long off )
{
  assert( ptr );
  return ptr ? ( ( ( char * ) ptr ) + off ) : NULL;
}

void *
duf_get_config_offset( unsigned long off )
{
  return duf_get_offset( duf_get_config(  ), off );
}

void *
duf_get_config_puz_offset( unsigned long off )
{
  return duf_get_config(  )? duf_get_offset( duf_get_config(  )->vars.puz, off ) : NULL;
}
#endif

muc_config_cli_t *
duf_get_config_cli( void )
{
  assert( duf_get_config(  ) );
#if 0
  return duf_get_config(  )? &duf_get_config(  )->cli : NULL;
#else
  return duf_get_config(  )? duf_get_config(  )->pcli : NULL;
#endif
}

duf_config_opt_t *
duf_get_config_opt( void )
{
  return duf_get_config(  )? &duf_get_config(  )->opt : NULL;
}

duf_ufilter_t *
duf_get_config_ufilter( void )
{
  return duf_get_config(  )? duf_get_config(  )->vars.puz : NULL;
}

duf_config_db_t *
duf_get_config_db( void )
{
  return duf_get_config(  )? &duf_get_config(  )->db : NULL;
}

#ifdef MAS_TRACING
mas_config_trace_t *
duf_get_trace_config( void )
{
  assert( duf_get_config(  ) );
  return duf_get_config(  )? duf_get_config(  )->opt.ptracecfg : NULL;
}
#endif

#define DUF_GET_FLAG_BIT( _word, _num ) _word & ( ( ( typeof( _word ) ) 1 ) << ( _num - 1 ) ) ? 1 : 0

typeof( duf_get_config(  )->opt.disable.sbit ) duf_get_config_flag_opt_disable_bits( void )
{
  return duf_get_config(  )->opt.disable.sbit;
}

static unsigned
duf_get_config_flag_opt_disable( int num )
{
  assert( duf_get_config(  ) );
  return duf_get_config(  ) && DUF_GET_FLAG_BIT( duf_get_config_flag_opt_disable_bits(  ), num );
}

typeof( duf_get_config(  )->opt.act.v.bit ) duf_get_config_flag_opt_act_bits( void )
{
  return duf_get_config(  )->opt.act.v.bit;
}

static unsigned
duf_get_config_flag_opt_act( int num )
{
  assert( duf_get_config(  ) );
  return duf_get_config(  ) && DUF_GET_FLAG_BIT( duf_get_config_flag_opt_act_bits(  ), num );
}

typeof( duf_get_config(  )->opt.flow.v.sbit ) duf_get_config_flag_opt_flow_bits( void )
{
  return ( unsigned long long ) duf_get_config(  )->opt.flow.v.sbit;
}

static unsigned
duf_get_config_flag_opt_flow( int num )
{
  assert( duf_get_config(  ) );
  return duf_get_config(  ) && DUF_GET_FLAG_BIT( duf_get_config_flag_opt_flow_bits(  ), num );
}

typeof( duf_get_config(  )->vars.puz->v.sbit ) duf_get_config_flag_vars_puz_bits( void )
{
  return duf_get_config(  )->vars.puz->v.sbit;
}

static unsigned
duf_get_config_flag_vars_puz( int num )
{
  assert( duf_get_config(  ) );
  return duf_get_config(  ) && DUF_GET_FLAG_BIT( duf_get_config_flag_vars_puz_bits(  ), num );
}

#define DUF_GET_FLAG_FUNC(_group, _rg, _set, _rs, _v, _name, _uname) \
  int \
  duf_get_config_flag_## _set ## _ ## _name( void ) \
  { \
    assert( duf_get_config(  ) ); \
    assert( ( ( duf_get_config(  ) && duf_get_config(  )->_group _rg _set _rs _v.flag._name ) ? 1 : 0 ) == duf_get_config_flag_ ## _group ## _ ## _set( DUF_FLAG_## _set ## _ ## _name ) ); \
    return duf_get_config_flag_ ## _group ## _ ## _set( DUF_FLAG_ ## _set ## _ ## _name ); \
  }
#define DUF_GET_FLAGNAME_FUNC(_group, _rg, _set, _rs, _v, _name, _uname ) \
  const char *duf_get_config_flagname_## _set ## _ ## _name( int nn ) \
  { \
    return     muc_coption_cnames_tmp( duf_get_config_cli(  ), nn, DUF_OPTION_VAL_FLAG_ ## _uname, NULL ); \
  }
#define DUF_GET_NUM_FUNC(_group, _rg, _set, _rs,  _name, _uname) \
  unsigned long \
  duf_get_config_num_ ## _set ## _ ## _name( void ) \
  { \
    assert( duf_get_config(  ) ); \
    return duf_get_config(  ) ? duf_get_config(  )->_group _rg _set _rs  _name : 0L ;\
  }

DUF_GET_FLAG_FUNC( opt,., act,., v, interactive, INTERACTIVE );
DUF_GET_FLAG_FUNC( opt,., act,., v, fast, FAST );
DUF_GET_FLAG_FUNC( opt,., act,., v, fresh, FRESH );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_files, ALLOW_FILES );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_dirs, ALLOW_DIRS );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_sub, ALLOW_SUB );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_drop_tables, ALLOW_DROP_TABLES );
DUF_GET_FLAGNAME_FUNC( opt,., act,., v, allow_drop_tables, ALLOW_DROP_TABLES );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_clean_tables, ALLOW_CLEAN_TABLES );
DUF_GET_FLAGNAME_FUNC( opt,., act,., v, allow_clean_tables, ALLOW_CLEAN_TABLES );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_create_tables, ALLOW_CREATE_TABLES );
DUF_GET_FLAGNAME_FUNC( opt,., act,., v, allow_create_tables, ALLOW_CREATE_TABLES );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_create_database, ALLOW_CREATE_DATABASE );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_remove_database, ALLOW_REMOVE_DATABASE );
DUF_GET_FLAGNAME_FUNC( opt,., act,., v, allow_remove_database, ALLOW_REMOVE_DATABASE );
DUF_GET_FLAG_FUNC( opt,., act,., v, allow_vacuum, ALLOW_VACUUM );
DUF_GET_FLAGNAME_FUNC( opt,., act,., v, allow_vacuum, ALLOW_VACUUM );
DUF_GET_FLAG_FUNC( opt,., act,., v, info, INFO );
DUF_GET_FLAG_FUNC( opt,., act,., v, summary, SUMMARY );
DUF_GET_FLAG_FUNC( opt,., act,., v, testflag, TESTFLAG );
DUF_GET_FLAG_FUNC( opt,., act,., v, testiflag, TESTIFLAG );
DUF_GET_FLAG_FUNC( opt,., act,., v, testnoflag, TESTNOFLAG );
DUF_GET_FLAG_FUNC( opt,., disable,,, memusage, DISABLE_MEMUSAGE );
DUF_GET_FLAGNAME_FUNC( opt,., disable,,, memusage, DISABLE_MEMUSAGE );
DUF_GET_FLAG_FUNC( opt,., disable,,, fs, DISABLE_FS );
DUF_GET_FLAG_FUNC( opt,., disable,,, insert, DISABLE_INSERT );
DUF_GET_FLAG_FUNC( opt,., disable,,, update, DISABLE_UPDATE );
DUF_GET_FLAG_FUNC( opt,., disable,,, calculate, DISABLE_CALCULATE );
DUF_GET_FLAG_FUNC( opt,., disable,,, testflag, TESTFLAG );
DUF_GET_FLAG_FUNC( opt,., disable,,, testiflag, TESTIFLAG );
DUF_GET_FLAG_FUNC( opt,., disable,,, testnoflag, TESTNOFLAG );
DUF_GET_FLAG_FUNC( opt,., flow,., v, dry_run, DRY_RUN );
DUF_GET_FLAGNAME_FUNC( opt,., flow,., v, dry_run, DRY_RUN );
DUF_GET_NUM_FUNC( opt,., flow,., verbose, VERBOSE );
DUF_GET_FLAG_FUNC( vars,., puz,->, v, recursive, RECURSIVE );
DUF_GET_FLAG_FUNC( vars,., puz,->, v, linear, LINEAR );
DUF_GET_FLAG_FUNC( vars,., puz,->, v, testflag, TESTFLAG );
DUF_GET_FLAG_FUNC( vars,., puz,->, v, testiflag, TESTIFLAG );
DUF_GET_FLAG_FUNC( vars,., puz,->, v, testnoflag, TESTNOFLAG );
DUF_GET_NUM_FUNC( ,,,, testnum, TESTNUM );

#if 0
char
duf_option_delimiter( void )
{
  return duf_get_config(  )? duf_get_config(  )->cli.option_delimiter : ':';
}

char *
duf_path_add_subdir( const char *dir, const char *subdir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  if ( subdir )
  {
    if ( strchr( subdir, '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, subdir );
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
              DUF_MAKE_ERRORM( rpr, DUF_ERROR_MKDIR, "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
            /* DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
            }
          }
        }
        else if ( !S_ISDIR( st.st_mode ) )
        {
          DUF_MAKE_ERROR( rpr, DUF_ERROR_STAT );
        }
      }
    }
  }
  if ( pr )
    *pr = rpr;
  return path;
}
#endif

const char *
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg MAS_UNUSED )
{
  static char buf[256];
  size_t len;
  size_t llen;
  const char *label = "TIME(";
  const char *pbuf = buf;

  llen = strlen( label );
  *buf = 0;
  len = strlen( name );
  if ( len > llen && 0 == strncmp( name, "TIME(", llen ) && name[len - 1] == ')' )
  {
  /* strftime */
    char *fmt;

    fmt = mas_strndup( name + llen, len - llen - 1 );
    mas_tstrflocaltime( buf, sizeof( buf ), fmt, time( NULL ) );
    mas_free( fmt );
    pbuf = buf;
  }
  else if ( *name == '+' )
  {
    if ( 0 == strcmp( name + 1, "db_name" ) || 0 == strcmp( name + 1, "dbname" ) || 0 == strcmp( name + 1, "db-name" ) )
      pbuf = duf_get_config_db_main_name(  );
  }
/* T( "@@@@@@var %s => '%s'", name, pbuf ); */
  return pbuf;
}
