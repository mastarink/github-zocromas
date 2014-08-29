#include <assert.h>
#include <string.h>
/* #include <getopt.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_print_defs.h"

/* #include "duf_types.h" */
#include "duf_utils.h"
#include "duf_service.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"


#include "duf_option.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */



extern const duf_longval_extended_t *lo_extended;
extern unsigned lo_extended_count;

int
duf_find_long( duf_option_code_t codeval )
{
  int rv = -1;

  assert( lo_extended_count );
  for ( int ilong = 0; codeval && ilong < lo_extended_count; ilong++ )
  {
    DUF_TRACE( explain, 4, "%4d. find codeval codeval: %d ? %d", ilong, codeval, lo_extended[ilong].o.val );
    if ( lo_extended[ilong].o.val == codeval )
    {
      rv = ilong;
      break;
    }
  }

  return rv;
}

/* extern const duf_option_t *duf_longopts; */
/* extern unsigned duf_longopts_count; */

static const duf_longval_extended_t *
duf_find_longval_extended( duf_option_code_t codeval )
{
  int ix;
  const duf_longval_extended_t *extended = NULL;

  ix = duf_find_long( codeval );
  extended = duf_longindex_extended( ix );

  return extended;
}

const duf_longval_extended_t *
duf_longindex_extended( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = &lo_extended[longindex];

  return extended;
}

const char *
duf_longindex_extended_name( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex );

  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex );

  return extended ? extended->o.val : 0;
}




static const char *
_duf_find_longval_help( duf_option_code_t codeval )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_find_longval_extended( codeval );
  /* extended = &lo_extended[ilong]; */

  if ( extended )
    ph = extended->help;
  return extended ? ( ph ? ph : "no description" ) : "not found";
}

const char *
duf_find_longval_help( duf_option_code_t codeval )
{
  const char *ph = _duf_find_longval_help( codeval );

  return ph ? ph : "-";
}

char *
duf_cli_option_shorts( void )
{
  char shorts[512] = "";
  char *p = shorts;

  for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
  {
    if ( duf_config->longopts_table[ilong].val < 0xFF )
    {
      /* DUF_ERROR( "S:%c %x - %s", duf_longopts[ilong].val, duf_longopts[ilong].val, shorts ); */
      if ( !strchr( shorts, ( char ) duf_config->longopts_table[ilong].val ) )
      {
        *p++ = ( char ) duf_config->longopts_table[ilong].val;
        if ( duf_config->longopts_table[ilong].has_arg == no_argument )
        {
        }
        else if ( duf_config->longopts_table[ilong].has_arg == required_argument )
        {
          *p++ = ':';
        }
        else if ( duf_config->longopts_table[ilong].has_arg == optional_argument )
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
  DUF_TRACE( explain, 0, "genereated options shorts: %s", shorts );
  return *shorts ? mas_strdup( shorts ) : NULL;
}

const char *
duf_option_cnames_tmp( int index, duf_option_code_t codeval )
{
  const char *x = NULL;

  if ( duf_config )
  {
    if ( index < 0 )
    {
      index = duf_config->tmp->explanation_index++;
      if ( duf_config->tmp->explanation_index >= DUF_TMP_EXPLANATION_MAX )
        duf_config->tmp->explanation_index = 0;
    }

    if ( index >= 0 && index < DUF_TMP_EXPLANATION_MAX )
    {
      mas_free( duf_config->tmp->option_explanation[index] );
      duf_config->tmp->option_explanation[index] = NULL;
      duf_config->tmp->option_explanation[index] = duf_option_names( codeval );
      x = duf_config->tmp->option_explanation[index];
    }
  }
  return x;
}

char *
duf_option_names( duf_option_code_t codeval )
{
  return duf_option_names_d( codeval, NULL );
}

char *
duf_option_names_d( duf_option_code_t codeval, const char *delim )
{
  char *names = NULL;
  int cnt = 0;

  for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
  {
    if ( duf_config->longopts_table[ilong].val == codeval )
    {
      const char *spaces = "                                                            ";
      size_t l;
      size_t mln = 14;

      if ( !cnt )
        names = mas_strcat_x( names, "≪" );
      else
        names = mas_strcat_x( names, delim ? delim : " | " );
      names = mas_strcat_x( names, "--" );
      names = mas_strcat_x( names, duf_config->longopts_table[ilong].name );
      l = strlen( duf_config->longopts_table[ilong].name );
      if ( l < mln )
        names = mas_strncat_x( names, spaces, mln - l );
      cnt++;
    }
  }
  if ( names && codeval && ( ( unsigned ) codeval ) < 0xff )
  {
    char sh[10] = "- ";

    sh[1] = codeval & 0xff;
    if ( !cnt )
      names = mas_strcat_x( names, "≫" );
    if ( cnt )
      names = mas_strcat_x( names, delim ? delim : " | " );
    names = mas_strcat_x( names, sh );
    cnt++;
  }
  if ( cnt )
    names = mas_strcat_x( names, "≫" );
  return names;
}

char *
duf_option_description( int longindex, const duf_longval_extended_t * extended )
{
  return duf_option_description_d( longindex, NULL, NULL );
}

char *
duf_option_description_d( int longindex, const char *delimh, const char *delim )
{
  char *s = NULL;
  duf_option_code_t codeval;

  codeval = duf_config->longopts_table[longindex].val;
  s = duf_option_names_d( codeval, delim );
  if ( s )
  {
    const char *h;

    h = _duf_find_longval_help( codeval );
    if ( h )
    {
      s = mas_strcat_x( s, delimh ? delimh : " :: " );
      s = mas_strcat_x( s, h );
    }
  }
  return s;
}

static const char *oclass_titles[DUF_OPTION_CLASS_MAX + 1] = {
  [DUF_OPTION_CLASS_HELP] = "Help",
  [DUF_OPTION_CLASS_SYSTEM] = "System",
  [DUF_OPTION_CLASS_CONTROL] = "Control",
  [DUF_OPTION_CLASS_REFERENCE] = "Reference",
  [DUF_OPTION_CLASS_COLLECT] = "Collect",
  [DUF_OPTION_CLASS_SCAN] = "Scan",
  [DUF_OPTION_CLASS_UPDATE] = "Update",
  [DUF_OPTION_CLASS_REQUEST] = "Request",
  [DUF_OPTION_CLASS_PRINT] = "Print",
  [DUF_OPTION_CLASS_TRACE] = "Trace",
  [DUF_OPTION_CLASS_OBSOLETE] = "Obsolete",
  [DUF_OPTION_CLASS_OTHER] = "Other",
  [DUF_OPTION_CLASS_NONE] = "None",
  [DUF_OPTION_CLASS_DEBUG] = "DEBUG",
  [DUF_OPTION_CLASS_NODESC] = "No desc",
};


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
    for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
    {
      if ( 0 == strcmp( name, duf_config->longopts_table[ilong].name ) )
      {
        /* const duf_longval_extended_t *extended; */

        /* extended = _duf_find_longval_extended( duf_config->longopts_table[ilong].val ); */
        /* extended = &lo_extended[ilong]; */
        /* extended = duf_longindex_extended( ilong ); */

        r = duf_parse_option_long( ilong, arg );
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
  const char *varname = "MSH_DUF_OPTIONS";

  eo = getenv( "MSH_DUF_OPTIONS" );

  DUF_TRACE( options, 0, "getting env options from variable %s", varname );
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
    /* DUF_TRACE( explain, 0, "env peo \"%s\"", peo ); */
    DUF_TRACE( explain, 0, "got env options from %s", varname );
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
  cfgpath = mas_strcat_x( cfgpath, DUF_CONFIG_FILE_NAME );
  DUF_TRACE( options, 0, "opening conf file %s", cfgpath );
  if ( cfgpath )
  {
    f = fopen( cfgpath, "r" );
  }
  if ( duf_config )
  {
    mas_free( duf_config->config_path );
    duf_config->config_path = cfgpath;
  }
  else
    mas_free( cfgpath );
  return f;
}

int
duf_infile_options( int argc, char *argv[] )
{
  int r = 0;
  const char *h = NULL;
  FILE *f = NULL;

  h = getenv( DUF_CONFIG_PATH_FROM_ENV );
  DUF_TRACE( options, 0, "getting variable " DUF_CONFIG_PATH_FROM_ENV " value for config path : %s", h );
  if ( h )
    f = duf_infile( 0, h );
  if ( !f )
  {
    h = getenv( "HOME" );
    DUF_TRACE( options, 0, "getting variable HOME value for config path (secondary) : %s", h );
    f = duf_infile( 1, h );
  }
  DUF_TRACE( explain, 0, "to read config file" );
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
_duf_restore_option_i( char *ptr, duf_option_code_t codeval, int value, int maxlen )
{
  for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
  {
    if ( duf_config->longopts_table[ilong].val == codeval )
    {
      snprintf( ptr, maxlen, " --%s='%d'", duf_config->longopts_table[ilong].name, value );
      break;
    }
  }
  return 0;
}

static int
_duf_restore_option_s( char *ptr, duf_option_code_t codeval, const char *value, int maxlen )
{
  if ( value )
    for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
    {
      if ( duf_config->longopts_table[ilong].val == codeval )
      {
        snprintf( ptr, maxlen, " --%s='%s'", duf_config->longopts_table[ilong].name, value );
        break;
      }
    }
  return 0;
}

static int
_duf_restore_option_b( char *ptr, duf_option_code_t codeval, int value, int maxlen )
{
  if ( value )
    for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
    {
      if ( duf_config->longopts_table[ilong].val == codeval )
      {
        snprintf( ptr, maxlen, " --%s", duf_config->longopts_table[ilong].name );
        break;
      }
    }
  return 0;
}

void
duf_restore_option( char *ptr, duf_option_code_t codeval, size_t maxlen )
{
  /* most probably not all options mentioned here - FIXME */
  DUF_OPTION_RESTORE_NUM( codeval, ptr, VERBOSE, verbose, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, DEBUG, debug, cli.dbg, maxlen );
  /* DUF_OPTION_RESTORE_TRACE(ptr, ALL, all ); */
  DUF_OPTION_RESTORE_NUM( codeval, ptr, TRACE_NONEW, nonew, cli.trace, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, OUTPUT_LEVEL, level, cli.output, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DRY_RUN, dry_run, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, EXPLAIN, explain, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SEQ, seq, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, OPTIONS, options, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CALLS, calls, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ANY, any, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CURRENT, current, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ACTION, action, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ERROR, error, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN, scan, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN_DE_DIR, scan_de_dir, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN_DE_REG, scan_de_reg, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, TEMP, temp, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, PATH, path, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, FS, fs, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SAMPUPD, sampupd, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SAMPLE, sample, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DELETED, deleted, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MDPATH, mdpath, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DIRENT, dirent, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MD5, md5, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SD5, sd5, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CRC32, crc32, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MIME, mime, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, EXIF, exif, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, COLLECT, collect, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, INTEGRITY, integrity, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SQL, sql, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SELECT, select, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, INSERT, insert, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, UPDATE, update, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MIN_DBGLINE, min_line, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAX_DBGLINE, max_line, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, PROGRESS, progress, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, SUMMARY, summary, cli.act, maxlen );
  /* DUF_OPTION_TREE_TO_DB: */
  /* DUF_OPTION_ZERO_DB: */
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DROP_TABLES, drop_tables, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, CREATE_TABLES, create_tables, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, ADD_PATH, add_path, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, UNI_SCAN, uni_scan, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, INFO, info, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, RECURSIVE, recursive, u, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DRY_RUN, dry_run, cli, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, SAMPLE, sample, cli.act, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, SAMPUPD, sampupd, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MDPATH, mdpath, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DIRENT, dirent, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MD5, md5, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, SD5, sd5, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, CRC32, crc32, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MIME, mime, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, EXIF, exif, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, COLLECT, collect, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, INTEGRITY, integrity, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, PRINT, print, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, TREE, tree, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, FILES, files, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DIRS, dirs, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_CALCULATE, calculate, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_INSERT, insert, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_UPDATE, update, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXSEQ, max_seq, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MINSIZE, size.min, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXSIZE, size.max, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MINDIRFILES, dirfiles.min, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXDIRFILES, dirfiles.max, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS, maxitems.total, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS_FILES, maxitems.files, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS_DIRS, maxitems.dirs, u, maxlen );
  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDERR, nonew, cli.trace ); */
  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDOUT, nonew, cli.trace ); */
  DUF_OPTION_RESTOREV_B( codeval, ptr, TRACE_STDERR, out, cli.trace, duf_config->cli.trace.out == stderr, maxlen );
  DUF_OPTION_RESTOREV_B( codeval, ptr, TRACE_STDOUT, out, cli.trace, duf_config->cli.trace.out == stdout, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, OUTPUT_FILE, file, cli.output, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, TRACE_FILE, file, cli.trace, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_DIRECTORY, dir, db, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_NAME_ADM, name, db.adm, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_NAME_MAIN, name, db.main, maxlen );
  /* DUF_OPTION_RESTORE_STR( FILE, file, cli.trace ); */
}

int
duf_show_options( const char *a0 )
{
  int r = 0;
  int seq = 0;

  DUF_TRACE( options, 0, "%s", a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_NONE; codeval < DUF_OPTION_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buffer[BUFSZ] = "";

    duf_restore_option( buffer, codeval, BUFSZ );
    if ( *buffer )
    {
      seq++;
      DUF_TRACE( options, 0, "%2d. %s", seq, buffer );
    }
  }
  DUF_TRACE( options, 0, " --" );
  return r;
}

char *
duf_restore_options( const char *a0 )
{
  char *str;

  str = mas_strdup( a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_NONE; codeval < DUF_OPTION_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buf[BUFSZ] = "";

    duf_restore_option( buf, codeval, BUFSZ );
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

#define DUF_H2C( codename, val ) case DUF_OPTION_HELP_ ## codename: val=DUF_OPTION_CLASS_ ## codename;
duf_option_class_t
duf_help_option2class( duf_option_code_t codeval )
{
  duf_option_class_t rv = DUF_OPTION_CLASS_BAD;

/*
  DUF_OPTION_CLASS_NONE,

  DUF_OPTION_CLASS_HELP,
  DUF_OPTION_CLASS_SYSTEM,
  DUF_OPTION_CLASS_CONTROL,
  DUF_OPTION_CLASS_REFERENCE,
  DUF_OPTION_CLASS_COLLECT,
  DUF_OPTION_CLASS_SCAN,
  DUF_OPTION_CLASS_UPDATE,
  DUF_OPTION_CLASS_REQUEST,
  DUF_OPTION_CLASS_PRINT,
  
  DUF_OPTION_CLASS_TRACE,
  DUF_OPTION_CLASS_DEBUG,
  DUF_OPTION_CLASS_OBSOLETE,
  DUF_OPTION_CLASS_NODESC,
  DUF_OPTION_CLASS_OTHER,

*/

  switch ( codeval )
  {
    DUF_H2C( NONE, rv );
    break;
    DUF_H2C( HELP, rv );
    break;
    DUF_H2C( SYSTEM, rv );
    break;
    DUF_H2C( CONTROL, rv );
    break;
    DUF_H2C( REFERENCE, rv );
    break;
    DUF_H2C( COLLECT, rv );
    break;
    DUF_H2C( SCAN, rv );
    break;
    DUF_H2C( UPDATE, rv );
    break;
    DUF_H2C( REQUEST, rv );
    break;
    DUF_H2C( PRINT, rv );
    break;
    DUF_H2C( TRACE, rv );
    break;
    DUF_H2C( DEBUG, rv );
    break;
    DUF_H2C( OBSOLETE, rv );
    break;
    DUF_H2C( NODESC, rv );
    break;
    DUF_H2C( OTHER, rv );
    break;
    DUF_H2C( ALL, rv );
    break;
  case DUF_OPTION_SMART_HELP:
    rv = DUF_OPTION_CLASS_ANY;
    break;
  default:
    break;
  }
  return rv;
}

void
duf_option_smart_help( duf_option_class_t oclass )
{
  int *ashown;
  size_t ss = lo_extended_count * sizeof( int );

  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );
  for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
  {
  }
  if ( oclass <= DUF_OPTION_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    DUF_PRINTF( 0, "-=-=-=-=- %s -=-=-=-=-", oclass_titles[oclass] );
  for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
  {
    char *s = NULL;
    duf_option_code_t codeval;
    const char *name;
    const duf_longval_extended_t *extd;
    int ie;

    name = duf_config->longopts_table[ilong].name;
    codeval = duf_config->longopts_table[ilong].val;
    /* extended = _duf_find_longval_extended( codeval ); */
    extd = duf_longindex_extended( ilong );
    /* ie = extended ? extended - &lo_extended[0] : -1; */
    ie = ilong;
    {
      int cnd = 0;

      cnd = ( !extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == DUF_OPTION_CLASS_NODESC ) )
            || ( extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == extd->oclass ) );
      if ( cnd )
      {
        int shown = -1;

        if ( ie >= 0 )
          shown = ashown[ie];
        if ( shown <= 0 )
        {
          int look = 1;

          /* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
          if ( duf_config->help_string )
          {
            char *s = duf_config->help_string;

            look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
          }
          if ( look )
          {
            /* duf_option_class_t hclass; */

            /* hclass = duf_help_option2class( codeval ); */
            s = duf_option_description_d( ilong, "\t", " // " );
            /* s = mas_strcat_x( s, " ...................." ); */
            if ( s )
            {
              /* if ( shown >= 0 )                    */
              /*   DUF_PRINTF( 0, " ## %d;", shown ); */

              DUF_PRINTF( 0, "\t%s", s );
              mas_free( s );
            }
            else
            {
              DUF_PRINTF( 0, " ??? %s", name );
            }
          }
        }
        if ( ie >= 0 )
          ashown[ie]++;
      }
    }
  }
  mas_free( ashown );
}

void
duf_option_help( int argc, char **argv )
{
  DUF_PRINTF( 0, "Usage: %s [OPTION]... [PATH]...", argv[0] );
  DUF_PRINTF( 0, "  -h, --help			[%s]", duf_find_longval_help( DUF_OPTION_HELP ) );
  DUF_PRINTF( 0, "  -x, --example			[%s]", duf_find_longval_help( DUF_OPTION_EXAMPLES ) );
  DUF_PRINTF( 0, "  --output-level		[%s]", duf_find_longval_help( DUF_OPTION_OUTPUT_LEVEL ) );
  DUF_PRINTF( 0, "Database ----------" );
  DUF_PRINTF( 0, "  -N, --db-name=%s", duf_config->db.main.name );
  DUF_PRINTF( 0, "  -D, --db-directory=%s", duf_config->db.dir );
  DUF_PRINTF( 0, "  --drop-tables		DANGEROUS!" );
  DUF_PRINTF( 0, "  --remove-database		DANGEROUS!" );
  DUF_PRINTF( 0, "  --create-tables" );
  DUF_PRINTF( 0, "  -V, --vacuum		vacuum" );
  DUF_PRINTF( 0, "Manipulations --------------" );
  DUF_PRINTF( 0, "  --add-path" );
  DUF_PRINTF( 0, "  -R, --recursive		recursive" );
  DUF_PRINTF( 0, "  --uni-scan" );
  DUF_PRINTF( 0, "    --md5" );
  DUF_PRINTF( 0, "    -f, --files" );
  DUF_PRINTF( 0, "    --tree" );
  DUF_PRINTF( 0, "    --print" );
  DUF_PRINTF( 0, "    --sample" );
  DUF_PRINTF( 0, "    --collect" );
  DUF_PRINTF( 0, "Old options" );
  DUF_PRINTF( 0, "  --zero-duplicates" );
  /* DUF_PRINTF(0,  "  --update-duplicates"  ); */
  DUF_PRINTF( 0, "  --zero-filedatas" );
  DUF_PRINTF( 0, "  --update-filedatas" );
  DUF_PRINTF( 0, "  --update-mdpaths" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  --update-exif" );
#endif
  DUF_PRINTF( 0, "  --same-md5" );
  DUF_PRINTF( 0, "Debug ----------" );
  DUF_PRINTF( 0, "  --debug" );
  DUF_PRINTF( 0, "  -v, --verbose=%d", duf_config->cli.dbg.verbose );
  DUF_PRINTF( 0, "  --min-dbg-lines=%lu", duf_config->cli.dbg.max_line );
  DUF_PRINTF( 0, "  --max-dbg-lines=%lu", duf_config->cli.dbg.min_line );
  DUF_PRINTF( 0, "Trace ----------" );
  DUF_PRINTF( 0, "  -A, --trace-action=%d", duf_config->cli.trace.action );
  DUF_PRINTF( 0, "  -C, --trace-current=%d", duf_config->cli.trace.current );
  DUF_PRINTF( 0, "  -S, --trace-scan=%d", duf_config->cli.trace.scan );
  DUF_PRINTF( 0, "  -Q, --trace-sql=%d", duf_config->cli.trace.sql );
  DUF_PRINTF( 0, "  --trace-sample=%d", duf_config->cli.trace.sample );
  DUF_PRINTF( 0, "  --trace-path=%d", duf_config->cli.trace.path );
  DUF_PRINTF( 0, "  -F, --trace-collect=%d", duf_config->cli.trace.collect );
  DUF_PRINTF( 0, "----------------" );
}

void
duf_option_examples( int argc, char **argv )
{
  DUF_PRINTF( 0, "Examples" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --drop-tables --create-tables" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file" );
  DUF_PRINTF( 0,
              "  run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/  --uni-scan -R --collect --dirent --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/chromium/   --print    -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print  --tree  -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print   -R --files" );
  DUF_PRINTF( 0, "----------------" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-duplicates" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-exif" );
#endif
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample=2 /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -Ri5fd /home/mastar/a/down/ --trace-md5 --trace-new=0 --trace-stdout" );

  DUF_PRINTF( 0, "===== Transition ============================" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run --update-duplicates --update-mdpath --update-filedata --update-exif" );
#endif
  DUF_PRINTF( 0, "  run --db-name=test`datem`.db  --uni-scan  --mdpath --collect --dirent -dfR  /mnt/new_media/media/down/" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "Collect basic file info recursively to db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db   --uni-scan -R --collect --dirent --files   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "Calculate checksums recursively for files in db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -R --collect --dirent --md5 --files --dirs /home/mastar/a/down/" );
  DUF_PRINTF( 0, "List files recursively from db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan   --print -R -d  --files /home/mastar/a/down/google-chrome/ " );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run --db-name=test.db  -AAPB   /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "    same as prev:" );
  DUF_PRINTF( 0, "  run --db-name=test.db --trace-action=2 --create-tables --uni-scan --recursive --collect --dirent"
              " --files --dirs --md5 /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan --print  --md5 --noopenat -Rdf --max-dirs=300  --min-dirfiles=5 --min-size=10" );
  DUF_PRINTF( 0, "=============================================" );

  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6 --summary"
              " --format=filesize,seq,filename,md5" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6"
              " --summary --format=filesize,seq,filename,md5 --tree" );
  DUF_PRINTF( 0, "  run  --trace-any=0  --db-name=test20140416.db  --uni-scan   --print  -df --max-seq=76 --max-depth=6 --summary"
              " --format=filesize,seq,filename,nlink,mtime,mode,gid,uid /mnt/new_media/media/down" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test$$.db  --uni-scan --print  --md5 --noopenat  -Rdf --max-dirs=300 --max-items=200 "
              " --min-dirfiles=5 --min-size=10 --noopenat" );

  DUF_PUTS( 0, "" );
  DUF_PUTS( 0, NULL );
  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "=== Working with the tree as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --remove-database" );
  DUF_PRINTF( 0, "  run -OPA /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run --uni-scan -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf" );
  DUF_PRINTF( 0, "========================= as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --sample=2 -Rdf /home/mastar/a/down/////  --printf --max-seq=2000" );
  DUF_PRINTF( 0, "========================= as for 20140501 ===================" );
  DUF_PRINTF( 0, "  run -PRdifE5 /home/mastar/a/down/chromium/ -SSS" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rd --summary  --tree   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf --max-seq=1000" );
  DUF_PRINTF( 0, "========================= as for 20140504 ===================" );
  DUF_PRINTF( 0, "= full - cycle collect in one command =======================" );
  DUF_PRINTF( 0, "  run  --remove-database -OP  -RdifE5 /home/mastar/a/down/chromium/"
              " --exclude='*.doc' --exclude='*.jpg' --exclude='*.css'" " --exclude='*.html' --exclude='*.png'  --exclude='*.gif'"
              " --exclude='*.js'  --exclude='*.php'" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-10 --size=+0" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-1b" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=+1G" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --same=+1 --format=filename,md5id" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --md5id=1285" );
  DUF_PRINTF( 0, "========================= as for 20140505 ===================" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/ --md5id=121549" " --format=human,inode=0,dataid=0" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/" " --format=human,inode=0,dataid=0,seq=0,prefix=0" " --include='*Speckled*'" );
  DUF_PRINTF( 0, "========================= as for 20140506 ===================" );
  DUF_PRINTF( 0, "  run -O -Rdif --mime /mnt/new_media/media/photo --max-seq=2000 (%gsec)", 2.8 );
  DUF_PRINTF( 0, "========================= as for 20140509 ===================" );
  DUF_PRINTF( 0, "  run --remove-database" );
  DUF_PRINTF( 0, "  run   -O						- %s", "creates DB" );
  DUF_PRINTF( 0, "  run   -P     /mnt/new_media/media/photo		- %s", "adds initial path" );
  DUF_PRINTF( 0, "  run   -OP    /mnt/new_media/media/photo		- %s", "creates DB AND adds initial path" );
  DUF_PRINTF( 0, "  run   -RdE   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rd    /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rdf   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rid   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -RidE  /mnt/new_media/media/photo		- %s", "collect directories information" );
  DUF_PRINTF( 0, "  run   -RiDE  /mnt/new_media/media/photo		- %s", "        files data (nameless) information" );
  DUF_PRINTF( 0, "  run   -RinE  /mnt/new_media/media/photo		- %s", "        files names information" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo		- %s", "        files md5 information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -Rife  /mnt/new_media/media/photo		- %s", "        files mime information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo		- %s", "print tree w/o files" );
  DUF_PRINTF( 0, "  run   -pRTdf /mnt/new_media/media/photo		- %s", "      tree with files" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo --max-depth=2	- %s", "      tree with files limited depth" );
  DUF_PRINTF( 0, "  run   -pRdf  /mnt/new_media/media/photo		- %s", "      files (full paths) with dir headers linear" );
  DUF_PRINTF( 0, "  run   -pRd   /mnt/new_media/media/photo		- %s", "      dirs (full paths) linear" );
  DUF_PRINTF( 0, "  run   -pRf   /mnt/new_media/media/photo		- %s", "      files (full paths) linear" );
  DUF_PRINTF( 0, "========================= as for 20140510 ===================" );
  DUF_PRINTF( 0, "  run   -Rdf --sample  --output /mnt/new_media/media/photo/" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo --progress" );
  DUF_PRINTF( 0, "========================= as for 20140513 ===================" );
  DUF_PRINTF( 0, "  run   -RifX  /mnt/new_media/media/photo		- %s", "        files exif information (-f pre-open file)" );
  DUF_PRINTF( 0, "========================= as for 20140717 ===================" );
  DUF_PRINTF( 0, "  export MSH_DUF_OPTIONS=explain=1			- %s", "" );
  DUF_PRINTF( 0, "  run   -RidDnE  /mnt/new_media/media/photo		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140718 ===================" );
  DUF_PRINTF( 0, "  run   -OPRidDnf5E  /mnt/new_media/media/photo		- %s", "" );


  DUF_PRINTF( 0, "========================= as for 20140818 ===================" );
  DUF_PRINTF( 0, "  run   --help-short 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help       		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-help   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-system 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-refer		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-collect		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-scan   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-update 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-request		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-print  		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-nodesc 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-examples		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140828 ===================" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/ -iPO --progress  --md5 --sd5 --crc32 --mime --exif --dirs --files --dirent --filenames --filedata --add-path --create-tables --drop-tables --remove-database --vacuum  --print --integrity --recursive --info  --disable-calculate  --disable-update --summary --version		- %s",
              "" );
  DUF_PRINTF( 0, "  run  -OPRdEifndD -532Xe /mnt/new_media/media/photo/ --trace-options		- %s", "" );
  DUF_PRINTF( 0, "  run --print -Rdf --max-depth=2   /mnt/new_media/media/photo/		- %s", "" );
  DUF_PRINTF( 0, "=============================================================" );
}

void
duf_option_version( int argc, char **argv )
{
  extern int __MAS_LINK_DATE__, __MAS_LINK_TIME__;
  char *sargv1, *sargv2;

  sargv1 = mas_argv_string( argc, argv, 1 );
  sargv2 = duf_restore_options( argv[0] );

  DUF_PRINTF( 0, "CFLAGS:          (%s)", MAS_CFLAGS );
  DUF_PRINTF( 0, "LDFLAGS:         (%s)", MAS_LDFLAGS );
  DUF_PRINTF( 0, "configire        (%s)", MAS_CONFIG_ARGS );
  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "UUID             %s", MAS_UUID );

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "prefix    [%2lu]   %s", sizeof( MAS_CONFIG_PREFIX ), MAS_CONFIG_PREFIX );
  DUF_PRINTF( 0, "C version:[%2lu]   %lu", sizeof( __STDC_VERSION__ ), __STDC_VERSION__ );
  DUF_PRINTF( 0, "O.        [%2lu]   %s", sizeof( MAS_OSVER ), MAS_OSVER );
  DUF_PRINTF( 0, "U.        [%2lu]   %s", sizeof( MAS_UNAME ), MAS_UNAME );
  DUF_PRINTF( 0, "V.        [%2lu]   %s", sizeof( PACKAGE_STRING ), PACKAGE_STRING );
  DUF_PRINTF( 0, "d.        [%2lu]   %s", sizeof( MAS_C_DATE ), MAS_C_DATE );
  DUF_PRINTF( 0, "Link d.   [%lu+%lu]  %lx.%06lx", sizeof( ( unsigned long ) & __MAS_LINK_DATE__ ),
              sizeof( ( unsigned long ) & __MAS_LINK_TIME__ ), ( unsigned long ) &__MAS_LINK_DATE__, ( unsigned long ) &__MAS_LINK_TIME__ );
  DUF_PRINTF( 0, "DATE/TIME          %s/%s", __DATE__, __TIME__ );
#ifdef MAS_SPLIT_DB
  DUF_PRINTF( 0, "MAS_SPLIT_DB" );
#endif

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "args:            (%s)", sargv1 );
  DUF_PRINTF( 0, "restored opts:   (%s)", sargv2 );

  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "config from %s ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", duf_config->config_path );
  DUF_PRINTF( 0, "cli.      [%2lu]   %x", sizeof( duf_config->cli.v.sbit ), duf_config->cli.v.sbit );
  DUF_PRINTF( 0, "u.        [%2lu]   %x", sizeof( duf_config->u.v.sbit ), duf_config->u.v.sbit );
  {
    unsigned u = duf_config->cli.act.v.bit;

    DUF_PRINTF( 0, "cli.act   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->cli.act.v ), sizeof( typeof( u ) ),
                ( unsigned long ) duf_config->cli.act.v.bit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
      /* *INDENT-OFF*  */
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --info" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --vacuum" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --remove_database" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --drop_tables" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --create_tables" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --add_path" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --print" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --tree" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --sd5" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --md5" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ │ └─ --crc32" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ │ └─ --mime" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ │ └─ --exif" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ │ └─ --mdpath" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ │ └─ --dirs" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ │ └─ --files" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ │ └─ --dirent" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ │ └─ --filedata" );
      DUF_PRINTF( 0, "                    │ │ │ │ │ └─ --filenames" );
      DUF_PRINTF( 0, "                    │ │ │ │ └─ --integrity" );
      DUF_PRINTF( 0, "                    │ │ │ └─ --collect" );
      DUF_PRINTF( 0, "                    │ │ └─ --uni-scan" );
      DUF_PRINTF( 0, "                    │ └─ --progress" );
      DUF_PRINTF( 0, "                    └─ --summary" );
      /* *INDENT-ON*  */
  DUF_PRINTF( 0, ">>> %lx", ( ( unsigned long ) 1 ) << ( ( sizeof( unsigned long ) * 8 ) - 1 ) );

  {
    unsigned u = duf_config->u.v.sbit;

    DUF_PRINTF( 0, "u   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->u.v ), sizeof( typeof( u ) ), ( unsigned long ) duf_config->u.v.sbit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                                  └─ --recursive" );

  {
    unsigned u = duf_config->cli.disable.sbit;

    DUF_PRINTF( 0, "cli.disable   [%2lu->%2lu]   %8lx :: ", sizeof( duf_config->cli.disable ), sizeof( typeof( u ) ),
                ( unsigned long ) duf_config->cli.disable.sbit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( int i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                              │ │ └─ --disable-calculate" );
  DUF_PRINTF( 0, "                                                              │ └─ --disable-insert" );
  DUF_PRINTF( 0, "                                                              └─ --disable-update" );



  mas_free( sargv2 );
  mas_free( sargv1 );
}
