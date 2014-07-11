/* #include <stdarg.h> */
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_print_defs.h"
#include "duf_trace_defs.h"

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_service.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"


#include "duf_option.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */



const duf_longval_extended_t lo_extended[] = {
  {.val = DUF_OPTION_OUTPUT_LEVEL,.help = "this is output-level option"},
  {.val = DUF_OPTION_HELP,.help = "this is help option"},
  {.val = DUF_OPTION_SMART_HELP,.help = "this is smart help option"},
  {.val = DUF_OPTION_EXAMPLES,.help = "this is examples option"},
  {.val = DUF_OPTION_FLAG_MIME,.help = "collect mime data while scanning ( reading ) files"},
  {.val = DUF_OPTION_FLAG_MD5,.help = "collect md5 data while scanning ( reading ) files"},
  {.val = DUF_OPTION_FLAG_COLLECT,.help = "collect mode - get data from filesystem"},
  {.val = DUF_OPTION_FLAG_FILES,.help = "get file info while collecting data"},
  {.val = 0,.help = NULL},
};

extern const duf_option_t *duf_longopts;
extern const int duf_longopts_count;

const duf_longval_extended_t *
duf_find_longval_extended( duf_option_code_t code )
{
  const duf_longval_extended_t *extended = NULL;

  for ( int i = 0; code && i < sizeof( lo_extended ) / sizeof( lo_extended[0] ); i++ )
  {
    if ( lo_extended[i].val == code )
    {
      extended = &lo_extended[i];
      break;
    }
  }

  return extended;
}

const char *
_duf_find_longval_help( duf_option_code_t code )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended = duf_find_longval_extended( code );

  if ( extended )
    ph = extended->help;
  return ph;
}

const char *
duf_find_longval_help( duf_option_code_t code )
{
  const char *ph = _duf_find_longval_help( code );

  return ph ? ph : "-";
}

char *
duf_cli_option_shorts( void )
{
  char shorts[512] = "";
  char *p = shorts;

  for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
  {
    if ( duf_longopts[i].val < 0xFF )
    {
      /* DUF_ERROR( "S:%c %x - %s", duf_longopts[i].val, duf_longopts[i].val, shorts ); */
      if ( !strchr( shorts, ( char ) duf_longopts[i].val ) )
      {
        *p++ = ( char ) duf_longopts[i].val;
        if ( duf_longopts[i].has_arg == no_argument )
        {
        }
        else if ( duf_longopts[i].has_arg == required_argument )
        {
          *p++ = ':';
        }
        else if ( duf_longopts[i].has_arg == optional_argument )
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
  }
  return *shorts ? mas_strdup( shorts ) : NULL;
}

char *
duf_option_names( duf_option_code_t code )
{
  char *names = NULL;

  for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
  {
    if ( duf_longopts[i].val == code )
    {
      if ( names )
        names = mas_strcat_x( names, " OR " );
      names = mas_strcat_x( names, "--" );
      names = mas_strcat_x( names, duf_longopts[i].name );
    }
  }
  if ( names && code && ( ( unsigned ) code ) < 0xff )
  {
    char sh[10] = "- ";

    sh[1] = code & 0xff;
    names = mas_strcat_x( names, " OR " );
    names = mas_strcat_x( names, sh );
  }
  return names;
}

char *
duf_option_description( duf_option_code_t code )
{
  char *s = NULL;

  s = duf_option_names( code );
  if ( s )
  {
    const char *h;

    h = _duf_find_longval_help( code );
    if ( h )
    {
      s = mas_strcat_x( s, " :: " );
      s = mas_strcat_x( s, h );
    }
  }
  return s;
}

void
duf_option_smart_help( void )
{
  for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
  {
    int look = 1;
    char *s = NULL;

    if ( duf_config->help_string )
    {
      char *s = duf_config->help_string;

      look = 0;

      if ( s && *s && !s[1] && duf_longopts[i].val == *s )
      {
        look = 1;
      }
      else if ( 0 == strcmp( s, duf_longopts[i].name ) )
      {
        look = 1;
      }
    }
    if ( look )
    {
      s = duf_option_description( duf_longopts[i].val );
      if ( s )
      {
        DUF_PUTS( 0, s );
        mas_free( s );
      }
      else
      {
        DUF_PRINTF( 0, " ??? %s", duf_longopts[i].name );
      }
    }
  }
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
    for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
    {
      if ( 0 == strcmp( name, duf_longopts[i].name ) )
      {
        const duf_longval_extended_t *extended = duf_find_longval_extended( duf_longopts[i].val );

        r = duf_parse_option( duf_longopts[i].val, arg, i, extended );
        DUF_TEST_R( r );
        break;
      }
    }
    /* DUF_TEST_R( r ); */
  }
  mas_free( name );
  mas_free( arg );
  if ( r == DUF_ERROR_OPTION )
  {
    DUF_ERROR( "Invalid option -- '%s'", string );
  }
  else
    DUF_TEST_R( r );
  return r;
}

int
duf_env_options( int argc, char *argv[] )
{
  int r = 0;
  const char *eo = NULL;
  const char *peo, *e;

  eo = getenv( "MSH_DUF_OPTIONS" );
  DUF_TRACE( explain, 0, "env options tuple: %s", eo );
  peo = eo;
  while ( peo && *peo )
  {
    char *s;

    s = NULL;
    e = strchr( peo, ':' );
    if ( e )
    {
      s = mas_strndup( peo, e - peo );
      DUF_TRACE( explain, 0, "env option from \"%s\"", s );
      e++;
    }
    else
    {
      s = mas_strdup( peo );
      DUF_TRACE( explain, 0, "env option (last) from \"%s\"", s );
    }
    if ( s )
    {
      char *xs;

      DUF_TRACE( explain, 0, "env s: \"%s\"", s );
      xs = mas_expand_string( s );

      DUF_TRACE( explain, 0, "env xs: \"%s\"", xs );
      r = duf_cli_option_by_string( xs );
      mas_free( xs );
    }
    mas_free( s );
    peo = e;
    DUF_TRACE( explain, 0, "env peo \"%s\"", peo );
  }
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
  DUF_TRACE( any, 1, "cfg:[%s]", cfgpath );
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
  DUF_TRACE( any, 1, "MSH_CONF_DIR:[%s]", h );
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

static int
_duf_restore_option_i( char *ptr, duf_option_code_t code, int value )
{
  for ( int i = 0; i < duf_longopts_count; i++ )
  {
    if ( duf_longopts[i].val == code )
    {
      sprintf( ptr, " --%s='%d'", duf_longopts[i].name, value );
      break;
    }
  }
  return 0;
}

static int
_duf_restore_option_s( char *ptr, duf_option_code_t code, const char *value )
{
  if ( value )
    for ( int i = 0; i < duf_longopts_count; i++ )
    {
      if ( duf_longopts[i].val == code )
      {
        sprintf( ptr, " --%s='%s'", duf_longopts[i].name, value );
        break;
      }
    }
  return 0;
}

static int
_duf_restore_option_b( char *ptr, duf_option_code_t code, int value )
{
  if ( value )
    for ( int i = 0; i < duf_longopts_count; i++ )
    {
      if ( duf_longopts[i].val == code )
      {
        sprintf( ptr, " --%s", duf_longopts[i].name );
        break;
      }
    }
  return 0;
}

void
duf_restore_option( char *ptr, duf_option_code_t optcode )
{
  DUF_OPTION_RESTORE_NUM( optcode, ptr, VERBOSE, verbose, cli.dbg );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, DEBUG, debug, cli.dbg );
  /* DUF_OPTION_RESTORE_TRACE(ptr, ALL, all ); */
  DUF_OPTION_RESTORE_NUM( optcode, ptr, TRACE_NONEW, nonew, cli.trace );



  DUF_OPTION_RESTORE_NUM( optcode, ptr, OUTPUT_LEVEL, level, cli.output );



  DUF_OPTION_RESTORE_TRACE( optcode, ptr, DRY_RUN, dry_run );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, EXPLAIN, explain );

  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SEQ, seq );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, CALLS, calls );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ANY, any );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, CURRENT, current );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ACTION, action );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ERROR, error );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SCAN, scan );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, PATH, path );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, FS, fs );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SAMPUPD, sampupd );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SAMPLE, sample );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, DELETED, deleted );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, MDPATH, mdpath );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, DIRENT, dirent );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, MD5, md5 );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SD5, sd5 );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, CRC32, crc32 );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, MIME, mime );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, EXIF, exif );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, COLLECT, collect );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, INTEGRITY, integrity );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SQL, sql );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SELECT, select );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, INSERT, insert );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, UPDATE, update );

  DUF_OPTION_RESTORE_NUM( optcode, ptr, MIN_DBGLINE, min_line, cli.dbg );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAX_DBGLINE, max_line, cli.dbg );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, PROGRESS, progress, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, SUMMARY, summary, cli.act );
  /* DUF_OPTION_TREE_TO_DB: */
  /* DUF_OPTION_ZERO_DB: */

  DUF_OPTION_RESTORE_FLAG( optcode, ptr, DROP_TABLES, drop_tables, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, CREATE_TABLES, create_tables, cli.act );

  DUF_OPTION_RESTORE_FLAG( optcode, ptr, ADD_PATH, add_path, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, UNI_SCAN, uni_scan, cli.act );

  DUF_OPTION_RESTORE_FLAG( optcode, ptr, RECURSIVE, recursive, u );

  DUF_OPTION_RESTORE_NUM( optcode, ptr, SAMPLE, sample, cli.act );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, SAMPUPD, sampupd, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, MDPATH, mdpath, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, DIRENT, dirent, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, MD5, md5, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, SD5, sd5, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, CRC32, crc32, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, MIME, mime, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, EXIF, exif, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, COLLECT, collect, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, INTEGRITY, integrity, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, PRINT, print, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, TREE, tree, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, FILES, files, cli.act );
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, DIRS, dirs, cli.act );

  DUF_OPTION_RESTORE_FLAGG( optcode, ptr, DISABLE_CALCULATE, calculate, cli,.disable );
  DUF_OPTION_RESTORE_FLAGG( optcode, ptr, DISABLE_INSERT, insert, cli,.disable );
  DUF_OPTION_RESTORE_FLAGG( optcode, ptr, DISABLE_UPDATE, update, cli,.disable );

  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXSEQ, maxseq, u );

  DUF_OPTION_RESTORE_NUM( optcode, ptr, MINSIZE, size.min, u );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXSIZE, size.max, u );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MINDIRFILES, dirfiles.min, u );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXDIRFILES, dirfiles.max, u );

  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXITEMS, maxitems.total, u );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXITEMS_FILES, maxitems.files, u );
  DUF_OPTION_RESTORE_NUM( optcode, ptr, MAXITEMS_DIRS, maxitems.dirs, u );



  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDERR, nonew, cli.trace ); */
  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDOUT, nonew, cli.trace ); */

  DUF_OPTION_RESTOREV_B( optcode, ptr, TRACE_STDERR, out, cli.trace, duf_config->cli.trace.out == stderr );
  DUF_OPTION_RESTOREV_B( optcode, ptr, TRACE_STDOUT, out, cli.trace, duf_config->cli.trace.out == stdout );

  DUF_OPTION_RESTORE_STR( optcode, ptr, OUTPUT_FILE, file, cli.output );
  DUF_OPTION_RESTORE_STR( optcode, ptr, TRACE_FILE, file, cli.trace );
  DUF_OPTION_RESTORE_STR( optcode, ptr, DB_DIRECTORY, dir, db );
  DUF_OPTION_RESTORE_STR( optcode, ptr, DB_NAME_ADM, name, db.adm );
  DUF_OPTION_RESTORE_STR( optcode, ptr, DB_NAME_MAIN, name, db.main );

  /* DUF_OPTION_RESTORE_STR( FILE, file, cli.trace ); */
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
