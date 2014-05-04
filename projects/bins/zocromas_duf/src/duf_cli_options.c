#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_config.h"

#include "duf_option.h"

/* ###################################################################### */
#include "duf_cli_options.h"
/* ###################################################################### */



const duf_longval_extended_t lo_extended[] = {
  {.val = DUF_OPTION_OUTPUT,.help = "this is printf option"},
  {.val = DUF_OPTION_HELP,.help = "this is help option"},
  {.val = DUF_OPTION_EXAMPLES,.help = "this is examples option"},
  {.val = 0,.help = NULL},
};

const duf_option_t longopts[] = {
/* const struct option longopts[] = { */
  {.name = "help",.has_arg = no_argument,.val = DUF_OPTION_HELP},
  {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_EXAMPLES},
  {.name = "format",.has_arg = required_argument,.val = DUF_OPTION_FORMAT},
  {.name = "output",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT},
  /* --------------- */
  {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},
  {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},
  {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},
  {.name = "output-file",.has_arg = required_argument,.val = DUF_OPTION_OUTPUT_FILE},
  /* --------------- */
  {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},
  /* --------------- */
  {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_ALL_TRACE},
  /* --------------- */
  {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},
  {.name = "trace-any",.has_arg = optional_argument,.val = DUF_OPTION_ANY_TRACE},
  {.name = "trace-current",.has_arg = optional_argument,.val = DUF_OPTION_CURRENT_TRACE},
  {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},
  {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},
  {.name = "trace-collect",.has_arg = optional_argument,.val = DUF_OPTION_COLLECT_TRACE},
  {.name = "trace-integrity",.has_arg = optional_argument,.val = DUF_OPTION_INTEGRITY_TRACE},
  {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},
  {.name = "trace-dirent",.has_arg = optional_argument,.val = DUF_OPTION_DIRENT_TRACE},
  {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},
  {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},
  {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},
  {.name = "trace-samupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD_TRACE},
  {.name = "trace-match",.has_arg = optional_argument,.val = DUF_OPTION_MATCH_TRACE},
  {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},
  {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},
  {.name = "trace-fs",.has_arg = optional_argument,.val = DUF_OPTION_FS_TRACE},
  /* --------------- */
  {.name = "verbose",.has_arg = optional_argument,.val = DUF_OPTION_VERBOSE},
  {.name = "debug",.has_arg = no_argument,.val = DUF_OPTION_DEBUG},
  {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},
  {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},
  {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},
  /* --------------- */
  {.name = "noopenat",.has_arg = optional_argument,.val = DUF_OPTION_NOOPENAT},
  {.name = "totals",.has_arg = no_argument,.val = DUF_OPTION_TOTALS},
  /* --------------- */
  {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},
  {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME},
  {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DB},
  {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_DROP_TABLES},
  {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_DATABASE},
  {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_CREATE_TABLES},
  {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},
  {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},

  /* {.name = "zero-duplicates",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DUPLICATES}, */
  {.name = "zero-filedata",.has_arg = no_argument,.val = DUF_OPTION_ZERO_FILEDATA},

  {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_ADD_PATH},
  /* {.name = "update-path",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_PATH}, */
  /* {.name = "update-md5",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MD5}, */
  /* {.name = "update-duplicates",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_DUPLICATES}, */
  /* {.name = "update-mdpath",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH}, */
  /* {.name = "update-mdpath-selective",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, */
  {.name = "update-filedata",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_FILEDATA},
  {.name = "update-exif",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_EXIF},
  {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_RECURSIVE},
  /* --------------- */
  {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_UNI_SCAN},
  {.name = "collect",.has_arg = no_argument,.val = DUF_OPTION_COLLECT},
  {.name = "integrity",.has_arg = no_argument,.val = DUF_OPTION_INTEGRITY},
  {.name = "dirent",.has_arg = no_argument,.val = DUF_OPTION_DIRENT},
  {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_MD5},
  {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_MDPATH},
  {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},
  {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_VACUUM},
  /* --------------- */
  {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_PRINT},
  {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_TREE},
  {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FILES},
  {.name = "dirs",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  {.name = "directories",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  /* --------------- */
  {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},
  {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},
  {.name = "size",.has_arg = required_argument,.val = DUF_OPTION_SIZE},
  {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},
  {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},
  {.name = "min-same",.has_arg = required_argument,.val = DUF_OPTION_MINSAME},
  {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},
  {.name = "max-depth",.has_arg = required_argument,.val = DUF_OPTION_MAXDEPTH},
  {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},
  {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},
  {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},
  {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},
  {.name = "max-seq",.has_arg = required_argument,.val = DUF_OPTION_MAXSEQ},
  /* {.name = "print-paths",.has_arg = no_argument,.val = DUF_OPTION_PRINT_PATHS}, */
  /* {.name = "print-dirs",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DIRS},   */
  /* {.name = "print-files",.has_arg = no_argument,.val = DUF_OPTION_PRINT_FILES}, */
  /* {.name = "print-duplicates",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DUPLICATES}, */
  {.name = "same-files",.has_arg = no_argument,.val = DUF_OPTION_SAME_FILES},
  {.name = "same-exif",.has_arg = no_argument,.val = DUF_OPTION_SAME_EXIF},
  {.name = "same-md5",.has_arg = no_argument,.val = DUF_OPTION_SAME_MD5},
  {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},
  {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},
  {.name = "group",.has_arg = required_argument,.val = DUF_OPTION_GROUP},
  /* {.name = "limit",.has_arg = required_argument,.val = DUF_OPTION_LIMIT}, */
  {.name = "add-to-group",.has_arg = no_argument,.val = DUF_OPTION_ADD_TO_GROUP},
  {.name = "remove-from-group",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_FROM_GROUP},
  {.name = NULL,.has_arg = no_argument,.val = DUF_OPTION_NONE},
};

const duf_longval_extended_t *
duf_find_longval_extended( int val )
{
  const duf_longval_extended_t *extended = NULL;

  for ( int i = 0; val && i < sizeof( lo_extended ) / sizeof( lo_extended[0] ); i++ )
  {
    if ( lo_extended[i].val == val )
    {
      extended = &lo_extended[i];
      break;
    }
  }

  return extended;
}

const char *
duf_find_longval_help( int val )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended = duf_find_longval_extended( val );

  if ( extended )
    ph = extended->help;
  return ph ? ph : "-";
}

int
duf_cli_option_by_string( const char *string )
{
  int r = DUF_ERROR_OPTION;
  char *eq;
  char *name = NULL;
  char *arg = NULL;

  eq = strchr( string, '=' );
  if ( eq )
  {
    name = mas_strndup( string, eq - string );
    arg = mas_strdup( eq + 1 );
  }
  else
  {
    name = mas_strdup( string );
  }
  if ( name )
  {
    for ( int i = 0; longopts[i].name && i < sizeof( longopts ) / sizeof( longopts[0] ); i++ )
    {
      if ( 0 == strcmp( name, longopts[i].name ) )
      {
        const duf_longval_extended_t *extended = duf_find_longval_extended( longopts[i].val );

        r = duf_parse_option( longopts[i].val, arg, i, extended );
        DUF_TEST_R( r );
        if ( r == DUF_ERROR_OPTION )
        {
          DUF_ERROR( "Invalid option -- '%s'", string );
        }
        break;
      }
    }
    DUF_TEST_R( r );
  }
  mas_free( name );
  mas_free( arg );
  DUF_TEST_R( r );
  return r;
}

static FILE *
duf_infile( int dot, const char *at )
{
  FILE *f = NULL;
  char *cfgpath = NULL;

  cfgpath = mas_strdup( at );
  cfgpath = mas_strcat_x( cfgpath, "/" );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  cfgpath = mas_strcat_x( cfgpath, "zocromas_duf.conf" );
  DUF_TRACE( any, 0, "cfg:[%s]", cfgpath );
  if ( cfgpath )
  {
    f = fopen( cfgpath, "r" );
  }
  mas_free( cfgpath );
  return f;
}

int
duf_infile_options( int argc, char *argv[] )
{
  int r = 0;
  const char *h = NULL;
  FILE *f = NULL;

  h = getenv( "MSH_CONF_DIR" );
  DUF_TRACE( any, 0, "MSH_CONF_DIR:[%s]", h );
  if ( h )
  {
    f = duf_infile( 0, h );
  }
  if ( !f )
  {
    h = getenv( "HOME" );
    DUF_TRACE( any, 0, "HOME:[%s]", h );
    f = duf_infile( 1, h );
  }
  while ( r >= 0 && f && !feof( f ) )
  {
    char buffer[1024];
    char *s;

    s = fgets( buffer, sizeof( buffer ), f );
    if ( s )
    {
      s = mas_chomp( s );
      if ( s && ( ( *s == '#' ) || !*s ) )
        continue;
      /* DUF_TRACE( any, 0, "buffer:[%s]", buffer ); */
      {
        char *xs;

        xs = mas_expand_string( s );

        r = duf_cli_option_by_string( xs );
        mas_free( xs );
      }
    }
  }
  if ( f )
    fclose( f );

  DUF_TEST_R( r );
  return r;
}

int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  if ( duf_config )
  {
    int opt;
    int longindex = 0;

    opterr = 0;
    while ( r == 0 && ( opt = getopt_long( argc, argv, DUF_OPTIONS_SHORT, longopts, &longindex ) ) >= 0 )
    {
      const duf_longval_extended_t *extended = duf_find_longval_extended( opt );

      r = duf_parse_option( opt, optarg, longindex, extended );
      if ( r == DUF_ERROR_OPTION )
      {
        DUF_ERROR( "Invalid option -- '%c' optind=%d/%s opt=%u/%c", optopt, optind, argv[optind - 1], opt, opt );
        /* r = optopt ? optopt : opt; */
      }
    }
    if ( optind < argc )
    {
      mas_del_argv( duf_config->targc, duf_config->targv, 0 );
      duf_config->targc = 0;
      duf_config->targv = NULL;

      duf_config->targc = mas_add_argv_argv( duf_config->targc, &duf_config->targv, argc, argv, optind );
    }
  }
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  return r;
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, duf_config->pref.lo, duf_config->cli.trace.sample); \ */

#define DUF_RESTORE_OPTIONTV(ptr, typ, up, lo, pref, value) \
  if ( code==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, code, value ); \
  }
#define DUF_RESTORE_OPTIONV_B(ptr,  up, lo, pref, value) \
  DUF_RESTORE_OPTIONTV(ptr, b, up, lo, pref, value)
#define DUF_RESTORE_OPTIONT(ptr, typ, up, lo, pref) \
  DUF_RESTORE_OPTIONTV(ptr, typ, up, lo, pref, duf_config->pref.lo)

#define DUF_RESTORE_OPTION(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, i, up, lo, pref)
#define DUF_RESTORE_OPTION_S(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, s, up, lo, pref)
#define DUF_RESTORE_OPTION_B(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, b, up, lo, pref)

#define DUF_RESTORE_OPTION_TRACE(ptr, up, lo) \
  DUF_RESTORE_OPTION(ptr,  up ## _TRACE, lo, cli.trace)


static int
_duf_restore_option_i( char *ptr, duf_option_code_t code, int val )
{
  for ( int i = 0; i < sizeof( longopts ) / sizeof( longopts[0] ); i++ )
  {
    if ( longopts[i].val == code )
    {
      sprintf( ptr, " --%s='%d'", longopts[i].name, val );
      break;
    }
  }
  return 0;
}

static int
_duf_restore_option_s( char *ptr, duf_option_code_t code, const char *val )
{
  if ( val )
    for ( int i = 0; i < sizeof( longopts ) / sizeof( longopts[0] ); i++ )
    {
      if ( longopts[i].val == code )
      {
        sprintf( ptr, " --%s='%s'", longopts[i].name, val );
        break;
      }
    }
  return 0;
}

static int
_duf_restore_option_b( char *ptr, duf_option_code_t code, int val )
{
  if ( val )
    for ( int i = 0; i < sizeof( longopts ) / sizeof( longopts[0] ); i++ )
    {
      if ( longopts[i].val == code )
      {
        sprintf( ptr, " --%s", longopts[i].name );
        break;
      }
    }
  return 0;
}

void
duf_restore_option( char *ptr, duf_option_code_t code )
{
  DUF_RESTORE_OPTION( ptr, VERBOSE, verbose, cli.dbg );
  DUF_RESTORE_OPTION( ptr, DEBUG, debug, cli.dbg );
  /* DUF_RESTORE_OPTION_TRACE(ptr, ALL, all ); */
  DUF_RESTORE_OPTION( ptr, TRACE_NONEW, nonew, cli.trace );



  DUF_RESTORE_OPTION( ptr, OUTPUT, level, cli.output );



  DUF_RESTORE_OPTION( ptr, NOOPENAT, noopenat, cli );

  DUF_RESTORE_OPTION_TRACE( ptr, CALLS, calls );
  DUF_RESTORE_OPTION_TRACE( ptr, ANY, any );
  DUF_RESTORE_OPTION_TRACE( ptr, CURRENT, current );
  DUF_RESTORE_OPTION_TRACE( ptr, ACTION, action );
  DUF_RESTORE_OPTION_TRACE( ptr, ERROR, error );
  DUF_RESTORE_OPTION_TRACE( ptr, SCAN, scan );
  DUF_RESTORE_OPTION_TRACE( ptr, PATH, path );
  DUF_RESTORE_OPTION_TRACE( ptr, FS, fs );
  DUF_RESTORE_OPTION_TRACE( ptr, SAMPUPD, sampupd );
  DUF_RESTORE_OPTION_TRACE( ptr, SAMPLE, sample );
  DUF_RESTORE_OPTION_TRACE( ptr, MDPATH, mdpath );
  DUF_RESTORE_OPTION_TRACE( ptr, DIRENT, dirent );
  DUF_RESTORE_OPTION_TRACE( ptr, MD5, md5 );
  DUF_RESTORE_OPTION_TRACE( ptr, COLLECT, collect );
  DUF_RESTORE_OPTION_TRACE( ptr, INTEGRITY, integrity );
  DUF_RESTORE_OPTION_TRACE( ptr, SQL, sql );

  DUF_RESTORE_OPTION( ptr, MIN_DBGLINE, min_line, cli.dbg );
  DUF_RESTORE_OPTION( ptr, MAX_DBGLINE, max_line, cli.dbg );
  DUF_RESTORE_OPTION_B( ptr, TOTALS, totals, cli.act );
  /* DUF_OPTION_TREE_TO_DB: */
  /* DUF_OPTION_ZERO_DB: */

  DUF_RESTORE_OPTION_B( ptr, DROP_TABLES, drop_tables, cli.act );
  DUF_RESTORE_OPTION_B( ptr, CREATE_TABLES, create_tables, cli.act );

  DUF_RESTORE_OPTION_B( ptr, ADD_PATH, add_path, cli.act );
  DUF_RESTORE_OPTION_B( ptr, UNI_SCAN, uni_scan, cli.act );

  DUF_RESTORE_OPTION( ptr, RECURSIVE, recursive, u );

  DUF_RESTORE_OPTION_B( ptr, SAMPLE, sample, cli.act );
  DUF_RESTORE_OPTION_B( ptr, SAMPUPD, sampupd, cli.act );
  DUF_RESTORE_OPTION_B( ptr, MDPATH, mdpath, cli.act );
  DUF_RESTORE_OPTION_B( ptr, DIRENT, dirent, cli.act );
  DUF_RESTORE_OPTION_B( ptr, MD5, md5, cli.act );
  DUF_RESTORE_OPTION_B( ptr, COLLECT, collect, cli.act );
  DUF_RESTORE_OPTION_B( ptr, INTEGRITY, integrity, cli.act );
  DUF_RESTORE_OPTION_B( ptr, PRINT, print, cli.act );
  DUF_RESTORE_OPTION_B( ptr, TREE, tree, cli.act );
  DUF_RESTORE_OPTION_B( ptr, FILES, files, cli.act );
  DUF_RESTORE_OPTION_B( ptr, DIRS, dirs, cli.act );

  DUF_RESTORE_OPTION( ptr, MAXSEQ, maxseq, u );

  DUF_RESTORE_OPTION( ptr, MINSIZE, minsize, u );
  DUF_RESTORE_OPTION( ptr, MAXSIZE, maxsize, u );
  DUF_RESTORE_OPTION( ptr, MINDIRFILES, mindirfiles, u );
  DUF_RESTORE_OPTION( ptr, MAXDIRFILES, maxdirfiles, u );

  DUF_RESTORE_OPTION( ptr, MAXITEMS, maxitems.total, u );
  DUF_RESTORE_OPTION( ptr, MAXITEMS_FILES, maxitems.files, u );
  DUF_RESTORE_OPTION( ptr, MAXITEMS_DIRS, maxitems.dirs, u );



  /* DUF_RESTORE_OPTION_TRACE(ptr, TRACE_STDERR, nonew, cli.trace ); */
  /* DUF_RESTORE_OPTION_TRACE(ptr, TRACE_STDOUT, nonew, cli.trace ); */

  DUF_RESTORE_OPTIONV_B( ptr, TRACE_STDERR, out, cli.trace, duf_config->cli.trace.out == stderr );
  DUF_RESTORE_OPTIONV_B( ptr, TRACE_STDOUT, out, cli.trace, duf_config->cli.trace.out == stdout );

  DUF_RESTORE_OPTION_S( ptr, OUTPUT_FILE, file, cli.output );
  DUF_RESTORE_OPTION_S( ptr, TRACE_FILE, file, cli.trace );
  DUF_RESTORE_OPTION_S( ptr, DB_DIRECTORY, dir, db );
  DUF_RESTORE_OPTION_S( ptr, DB_NAME, name, db );

  /* DUF_RESTORE_OPTION_S( FILE, file, cli.trace ); */
}

char *
duf_restore_options( const char *a0 )
{
  char *str;

  str = mas_strdup( a0 );
  for ( int i = 0; i < DUF_OPTION_MAX_LONG; i++ )
  {
    char buf[1024] = "";

    duf_restore_option( buf, i );
    if ( *buf )
    {
      /* str = mas_strcat_x( str, " @[ " ); */
      str = mas_strcat_x( str, buf );
      /* str = mas_strcat_x( str, " ]@ " ); */
    }
  }
  str = mas_strcat_x( str, " --" );
  return str;
}

int
duf_show_options( const char *a0 )
{
  int r = 0;

  DUF_TRACE( action, 0, "%s", a0 );
  for ( int i = 0; i < DUF_OPTION_MAX_LONG; i++ )
  {
    char buffer[1024] = "";

    duf_restore_option( buffer, i );
    if ( *buffer )
      DUF_TRACE( action, 0, "%s", buffer );
  }
  DUF_TRACE( action, 0, " --" );
  return r;
}
