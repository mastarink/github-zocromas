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
  {.val = DUF_OPTION_OUTPUT_LEVEL,.help = "this is output-level option",.oclass = DUF_OPTION_CLASS_NONE},

  {.val = DUF_OPTION_HELP,.help = "help",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_SMART_HELP,.help = "smart help",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_HELP,.help = "help on help",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_ALL,.help = "help on all",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_SYSTEM,.help = "help on system",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_CONTROL,.help = "help on control",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_REFERENCE,.help = "help on reference",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_COLLECT,.help = "help on collect",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_SCAN,.help = "help on scan",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_UPDATE,.help = "help on update",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_REQUEST,.help = "help on request",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_PRINT,.help = "help on print",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_TRACE,.help = "help on trace",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_DEBUG,.help = "help on debug",.oclass = DUF_OPTION_CLASS_HELP},
  {.val = DUF_OPTION_HELP_NODESC,.help = "help on nodesc",.oclass = DUF_OPTION_CLASS_HELP},

  {.val = DUF_OPTION_FLAG_ZERO_DB,.help = "zero db",.oclass = DUF_OPTION_CLASS_SYSTEM},
  {.val = DUF_OPTION_FLAG_CREATE_TABLES,.help = "create tables",.oclass = DUF_OPTION_CLASS_SYSTEM},
  {.val = DUF_OPTION_FLAG_DROP_TABLES,.help = "drop tables",.oclass = DUF_OPTION_CLASS_SYSTEM},
  {.val = DUF_OPTION_FLAG_VACUUM,.help = "vacuum db",.oclass = DUF_OPTION_CLASS_SYSTEM},

  {.val = DUF_OPTION_FLAG_RECURSIVE,.help = "recursive",.oclass = DUF_OPTION_CLASS_CONTROL},
  /* {.val = DUF_OPTION_DB_NAME,.help = "db name",.oclass = DUF_OPTION_CLASS_CONTROL}, */
  {.val = DUF_OPTION_DB_NAME_MAIN,.help = "db name main",.oclass = DUF_OPTION_CLASS_CONTROL},
  {.val = DUF_OPTION_DB_NAME_ADM,.help = "db name adm",.oclass = DUF_OPTION_CLASS_CONTROL},
  {.val = DUF_OPTION_DB_DIRECTORY,.help = "db directory",.oclass = DUF_OPTION_CLASS_CONTROL},
  {.val = DUF_OPTION_FLAG_COLLECT,.help = "collect mode - get data from filesystem",.oclass = DUF_OPTION_CLASS_CONTROL},
  {.val = DUF_OPTION_FLAG_ADD_PATH,.help = "register initial path",.oclass = DUF_OPTION_CLASS_CONTROL},

  {.val = DUF_OPTION_VERSION,.help = "db info",.oclass = DUF_OPTION_CLASS_REFERENCE},
  {.val = DUF_OPTION_FLAG_INFO,.help = "db info",.oclass = DUF_OPTION_CLASS_REFERENCE},
  {.val = DUF_OPTION_FLAG_SUMMARY,.help = "summary",.oclass = DUF_OPTION_CLASS_REFERENCE},
  {.val = DUF_OPTION_FLAG_PROGRESS,.help = "progress",.oclass = DUF_OPTION_CLASS_REFERENCE},

  {.val = DUF_OPTION_EXAMPLES,.help = "examples",.oclass = DUF_OPTION_CLASS_HELP},

  {.val = DUF_OPTION_FLAG_MIME,.help = "collect mime data while scanning ( reading ) files",.oclass = DUF_OPTION_CLASS_SCAN},
  {.val = DUF_OPTION_FLAG_EXIF,.help = "collect exif data while scanning ( reading ) files",.oclass = DUF_OPTION_CLASS_SCAN},
  {.val = DUF_OPTION_FLAG_CRC32,.help = "collect crc32 data while scanning ( reading ) files",.oclass = DUF_OPTION_CLASS_SCAN},
  {.val = DUF_OPTION_FLAG_SD5,.help = "collect sd5 data while scanning ( reading ) files",.oclass = DUF_OPTION_CLASS_SCAN},
  {.val = DUF_OPTION_FLAG_MD5,.help = "collect md5 data while scanning ( reading ) files",.oclass = DUF_OPTION_CLASS_SCAN},

  {.val = DUF_OPTION_FLAG_DIRENT,.help = "dirent",.oclass = DUF_OPTION_CLASS_COLLECT},
  {.val = DUF_OPTION_FLAG_FILES,.help = "get file info while collecting data",.oclass = DUF_OPTION_CLASS_COLLECT},
  {.val = DUF_OPTION_FLAG_DIRS,.help = "get dir info while collecting data",.oclass = DUF_OPTION_CLASS_COLLECT},
  {.val = DUF_OPTION_FLAG_FILEDATA,.help = "get filedata",.oclass = DUF_OPTION_CLASS_COLLECT},
  {.val = DUF_OPTION_FLAG_FILENAMES,.help = "get filenames",.oclass = DUF_OPTION_CLASS_COLLECT},

  {.val = DUF_OPTION_FORMAT,.help = "print format",.oclass = DUF_OPTION_CLASS_PRINT},
  {.val = DUF_OPTION_FLAG_PRINT,.help = "print",.oclass = DUF_OPTION_CLASS_PRINT},
  {.val = DUF_OPTION_FLAG_TREE,.help = "tree",.oclass = DUF_OPTION_CLASS_PRINT},

  {.val = DUF_OPTION_TRACE_FILE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},

  {.val = DUF_OPTION_TRACE_STDERR,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_TRACE_STDOUT,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_OUTPUT_FILE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  /* --------------- */
  {.val = DUF_OPTION_TRACE_NONEW,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  /* --------------- */
  {.val = DUF_OPTION_ALL_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  /* --------------- */
  {.val = DUF_OPTION_DRY_RUN_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_EXPLAIN_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_EXPLAIN_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  /* --------------- */
  {.val = DUF_OPTION_ACTION_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_ANY_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_CALLS_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_COLLECT_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_CRC32_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_CURRENT_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_DELETED_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_DIRENT_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_ERROR_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_EXIF_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_FS_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_INSERT_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_INTEGRITY_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_MATCH_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_MD5_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_MDPATH_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_MIME_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_OPTIONS_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_PATH_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SAMPLE_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SAMPUPD_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SCAN_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SCAN_DE_DIR_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SCAN_DE_REG_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_TEMP_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SD5_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SELECT_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SEQ_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_SQL_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},
  {.val = DUF_OPTION_UPDATE_TRACE,.help = "trace ....",.oclass = DUF_OPTION_CLASS_TRACE},

  {.val = DUF_OPTION_VERBOSE,.help = "verbose",.oclass = DUF_OPTION_CLASS_DEBUG},
  {.val = DUF_OPTION_DEBUG,.help = "verbose",.oclass = DUF_OPTION_CLASS_DEBUG},
  {.val = DUF_OPTION_MEMUSAGE,.help = "verbose",.oclass = DUF_OPTION_CLASS_DEBUG},
  {.val = DUF_OPTION_MIN_DBGLINE,.help = "verbose",.oclass = DUF_OPTION_CLASS_DEBUG},
  {.val = DUF_OPTION_MAX_DBGLINE,.help = "verbose",.oclass = DUF_OPTION_CLASS_DEBUG},

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
_duf_find_longval_help( duf_option_code_t code, const duf_longval_extended_t * extended )
{
  const char *ph = NULL;

  /* const duf_longval_extended_t *extended; */

  if ( !extended )
    extended = duf_find_longval_extended( code );

  if ( extended )
    ph = extended->help;
  return ph ? ph : "no description";
}

const char *
duf_find_longval_help( duf_option_code_t code )
{
  const char *ph = _duf_find_longval_help( code, NULL );

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
  DUF_TRACE( explain, 0, "genereated options shorts: %s", shorts );

  return *shorts ? mas_strdup( shorts ) : NULL;
}

const char *
duf_option_cnames_tmp( duf_option_code_t code )
{
  if ( duf_config )
  {
    mas_free( duf_config->tmp->option_explanation );
    duf_config->tmp->option_explanation = NULL;
    duf_config->tmp->option_explanation = duf_option_names( code );
    return duf_config->tmp->option_explanation;
  }
  else
  {
    return NULL;
  }
}

char *
duf_option_names( duf_option_code_t code )
{
  return duf_option_names_d( code, NULL );
}

char *
duf_option_names_d( duf_option_code_t code, const char *delim )
{
  char *names = NULL;
  int cnt = 0;

  for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
  {
    if ( duf_longopts[i].val == code )
    {
      const char *spaces = "                                                            ";
      size_t l;
      size_t mln = 12;

      if ( !cnt )
      {

        names = mas_strcat_x( names, "≪" );
      }
      if ( cnt )
        names = mas_strcat_x( names, delim ? delim : " | " );
      names = mas_strcat_x( names, "--" );
      names = mas_strcat_x( names, duf_longopts[i].name );
      l = strlen( duf_longopts[i].name );
      if ( l < mln )
        names = mas_strncat_x( names, spaces, mln - l );
      cnt++;
    }
  }
  if ( names && code && ( ( unsigned ) code ) < 0xff )
  {
    char sh[10] = "- ";

    sh[1] = code & 0xff;
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
duf_option_description( duf_option_code_t code, const duf_longval_extended_t * extended )
{
  return duf_option_description_d( code, extended, NULL, NULL );
}

char *
duf_option_description_d( duf_option_code_t code, const duf_longval_extended_t * extended, const char *delimh, const char *delim )
{
  char *s = NULL;

  s = duf_option_names_d( code, delim );
  if ( s )
  {
    const char *h;

    h = _duf_find_longval_help( code, extended );
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
  [DUF_OPTION_CLASS_DEBUG] = "DEBUG",
  [DUF_OPTION_CLASS_NODESC] = "No desc",
};

void
duf_option_smart_help( duf_option_class_t oclass )
{
  int *ashown;
  size_t ss = sizeof( lo_extended ) / sizeof( lo_extended[0] ) * sizeof( int );

  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );

  if ( oclass <= DUF_OPTION_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    DUF_PRINTF( 0, "-=-=-=-=- %s -=-=-=-=-", oclass_titles[oclass] );

  for ( int i = 0; duf_longopts[i].name && i < duf_longopts_count; i++ )
  {
    int look = 1;
    char *s = NULL;
    duf_option_code_t code;
    const char *name;
    const duf_longval_extended_t *extended;
    int ie;

    name = duf_longopts[i].name;
    code = duf_longopts[i].val;
    extended = duf_find_longval_extended( code );
    ie = extended ? extended - &lo_extended[0] : -1;

    /* DUF_PRINTF( 0, " ????? %d", oclass ); */

    if ( ( !extended && ( oclass == DUF_OPTION_CLASS_ANY || oclass == DUF_OPTION_CLASS_NODESC ) )
         || ( extended && ( oclass == DUF_OPTION_CLASS_ANY || oclass == extended->oclass ) ) )
    {
      int shown = -1;

      if ( ie >= 0 )
        shown = ashown[ie];

      if ( shown <= 0 )
      {
        if ( duf_config->help_string )
        {
          char *s = duf_config->help_string;

          look = 0;

          if ( s && *s && !s[1] && code == *s )
          {
            look = 1;
          }
          else if ( 0 == strcmp( s, name ) )
          {
            look = 1;
          }
        }
        if ( look )
        {
          s = duf_option_description_d( code, extended, "\t", " // " );
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
  mas_free( ashown );
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
  const char *varname = "MSH_DUF_OPTIONS";

  eo = getenv( "MSH_DUF_OPTIONS" );
  DUF_TRACE( explain, 0, "getting env options from variable %s", varname );

  /* DUF_TRACE( explain, 0, "env options tuple: %s", eo ); */
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
  DUF_TRACE( explain, 0, "opening conf file %s", cfgpath );
  DUF_TRACE( any, 1, "cfg:[%s]", cfgpath );
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
  DUF_TRACE( explain, 0, "getting variable " DUF_CONFIG_PATH_FROM_ENV " value for config path" );
  DUF_TRACE( any, 1, "MSH_CONF_DIR:[%s]", h );
  if ( h )
  {
    f = duf_infile( 0, h );
  }
  if ( !f )
  {
    h = getenv( "HOME" );
    DUF_TRACE( explain, 0, "getting variable HOME value for config path (secondary)" );
    DUF_TRACE( any, 0, "HOME:[%s]", h );
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
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, OPTIONS, options );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, CALLS, calls );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ANY, any );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, CURRENT, current );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ACTION, action );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, ERROR, error );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SCAN, scan );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SCAN_DE_DIR, scan_de_dir );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, SCAN_DE_REG, scan_de_reg );
  DUF_OPTION_RESTORE_TRACE( optcode, ptr, TEMP, temp );
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
  DUF_OPTION_RESTORE_FLAG( optcode, ptr, INFO, info, cli.act );

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

  DUF_TRACE( options, 0, "%s", a0 );
  for ( int i = 0; i < DUF_OPTION_MAX_LONG; i++ )
  {
    char buffer[1024] = "";

    duf_restore_option( buffer, i );
    if ( *buffer )
      DUF_TRACE( options, 0, "%s", buffer );
  }
  DUF_TRACE( options, 0, " --" );
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
