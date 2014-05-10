/* #include <stdarg.h> */
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"
#include "duf_opt_types.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_config.h"

#include "duf_option.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */



const duf_longval_extended_t lo_extended[] = {
  {.val = DUF_OPTION_OUTPUT,.help = "this is printf option"},
  {.val = DUF_OPTION_HELP,.help = "this is help option"},
  {.val = DUF_OPTION_SMART_HELP,.help = "this is smart help option"},
  {.val = DUF_OPTION_EXAMPLES,.help = "this is examples option"},
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
    char *s = NULL;

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
duf_restore_option( char *ptr, duf_option_code_t code )
{
  DUF_RESTORE_OPTION( ptr, VERBOSE, verbose, cli.dbg );
  DUF_RESTORE_OPTION( ptr, DEBUG, debug, cli.dbg );
  /* DUF_RESTORE_OPTION_TRACE(ptr, ALL, all ); */
  DUF_RESTORE_OPTION( ptr, TRACE_NONEW, nonew, cli.trace );



  DUF_RESTORE_OPTION( ptr, OUTPUT, level, cli.output );



  DUF_RESTORE_OPTION( ptr, NOOPENAT, noopenat, cli );

  DUF_RESTORE_OPTION_TRACE( ptr, SEQ, seq );
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
  DUF_RESTORE_OPTION_TRACE( ptr, DELETED, deleted );
  DUF_RESTORE_OPTION_TRACE( ptr, MDPATH, mdpath );
  DUF_RESTORE_OPTION_TRACE( ptr, DIRENT, dirent );
  DUF_RESTORE_OPTION_TRACE( ptr, MD5, md5 );
  DUF_RESTORE_OPTION_TRACE( ptr, MIME, mime );
  DUF_RESTORE_OPTION_TRACE( ptr, COLLECT, collect );
  DUF_RESTORE_OPTION_TRACE( ptr, INTEGRITY, integrity );
  DUF_RESTORE_OPTION_TRACE( ptr, SQL, sql );
  DUF_RESTORE_OPTION_TRACE( ptr, SELECT, select );
  DUF_RESTORE_OPTION_TRACE( ptr, INSERT, insert );
  DUF_RESTORE_OPTION_TRACE( ptr, UPDATE, update );

  DUF_RESTORE_OPTION( ptr, MIN_DBGLINE, min_line, cli.dbg );
  DUF_RESTORE_OPTION( ptr, MAX_DBGLINE, max_line, cli.dbg );
  DUF_RESTORE_OPTION_B( ptr, SUMMARY, summary, cli.act );
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
  DUF_RESTORE_OPTION_B( ptr, MIME, mime, cli.act );
  DUF_RESTORE_OPTION_B( ptr, COLLECT, collect, cli.act );
  DUF_RESTORE_OPTION_B( ptr, INTEGRITY, integrity, cli.act );
  DUF_RESTORE_OPTION_B( ptr, PRINT, print, cli.act );
  DUF_RESTORE_OPTION_B( ptr, TREE, tree, cli.act );
  DUF_RESTORE_OPTION_B( ptr, FILES, files, cli.act );
  DUF_RESTORE_OPTION_B( ptr, DIRS, dirs, cli.act );
  DUF_RESTORE_OPTION_B( ptr, DISABLE_INSERT, insert, cli.disable );
  DUF_RESTORE_OPTION_B( ptr, DISABLE_UPDATE, update, cli.disable );

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
  DUF_RESTORE_OPTION_S( ptr, DB_NAME_ADM, name, db.adm );
  DUF_RESTORE_OPTION_S( ptr, DB_NAME_MAIN, name, db.main );

  /* DUF_RESTORE_OPTION_S( FILE, file, cli.trace ); */
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
