#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "source_defaults.h"

static int
source_check_env( int count _uUu_, const void *data_ptr, const char *delims
                  _uUu_, const char *eq _uUu_, const mucs_prefix_encoder_t * pref_ids _uUu_ )
{
  return data_ptr && getenv( ( char * ) data_ptr ) ? 1 : 0;
}

static int
source_check_string( int count _uUu_, const void *data_ptr _uUu_, const char *delims
                     _uUu_, const char *eq _uUu_, const mucs_prefix_encoder_t * pref_ids _uUu_ )
{
  return 1;
}

static char *
source_load_string_string( mucs_source_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_strdup( osrc->data_ptr ) : NULL;
}

static char *
source_load_string_env( mucs_source_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_strdup( getenv( ( char * ) osrc->data_ptr ) ) : NULL;
}

static char *
source_load_string_argv( mucs_source_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_argv_join( osrc->count, ( char ** ) osrc->data_ptr, 0, osrc->delim ) : NULL;
}

static char *
source_load_string_margv( mucs_source_t * osrc )
{
  return osrc && osrc->data_ptr
          && osrc->npos < osrc->count ? mas_argv_join( 0, ( ( char *** ) osrc->data_ptr )[osrc->npos], 0 /* osrc->npos > 0 ? 1 : 0 */ ,
                                                       osrc->delim ) : NULL;
}

static mas_argvc_t
source_load__targ_string( mucs_source_t * osrc, mas_argvc_t targ, const char *string )
{
  if ( osrc && osrc->data_ptr && !osrc->npos )
    mas_add_argvc_args_d( &targ, string, 0, osrc->delims );
  return targ;
}

static mas_argvc_t
source_load_targ_string( mucs_source_t * osrc, mas_argvc_t targ )
{
  return source_load__targ_string( osrc, targ, ( char * ) osrc->data_ptr );
}

static mas_argvc_t
source_load_targ_env( mucs_source_t * osrc, mas_argvc_t targ )
{
  return source_load__targ_string( osrc, targ, getenv( ( char * ) osrc->data_ptr ) );
}

static mas_argvc_t
source_load_targ_argv( mucs_source_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && !osrc->npos )
    mas_add_argvc_argv( &targ, osrc->count, ( char ** ) osrc->data_ptr, 0 );
  return targ;
}

static mas_argvc_t
source_load_targ_margv( mucs_source_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && osrc->npos < osrc->count )
    mas_add_argvc_argv( &targ, 0, ( ( char *** ) osrc->data_ptr )[osrc->npos], 0 /* osrc->npos > 0 ? 1 : 0 */  );
  return targ;
}

static mas_argvc_t
source_load__targ_stream( FILE * fin, mucs_source_t * osrc, mas_argvc_t targ, int *peof )
{
  if ( fin && osrc && !osrc->error )
  {
    char buffer[1024 * 10];
    char *string = NULL;
    const char *ignpref = NULL;

    do
    {
      string = fgets( buffer, sizeof( buffer ), fin );
      if ( peof )
        *peof = !string;

      if ( string )
      {
        string = mas_chomp( string );
        ignpref = osrc->pref_ids[MUCS_VARIANT_IGNORE].string;
      }
    } while ( ( string && !*string ) || ( ignpref && string && 0 == strncmp( ignpref, string, strlen( ignpref ) ) ) );
    if ( string && *string )
      mas_add_argvc_args_d( &targ, string, 0, osrc->delims );
  }
  return targ;
}

static mas_argvc_t
source_load_targ_stream( mucs_source_t * osrc, mas_argvc_t targ )
{
/* file (FILE *) at osrc->data_ptr */
  if ( osrc && !osrc->error && osrc->data_ptr )
  {
    FILE *fin = ( FILE * ) osrc->data_ptr;

    targ = source_load__targ_stream( fin, osrc, targ, NULL );
  }

  return targ;
}

static mas_argvc_t
source_load_targ_file( mucs_source_t * osrc, mas_argvc_t targ )
{
/* file name at osrc->data_ptr */
  if ( osrc && !osrc->error && osrc->data_ptr )
  {
    const char *filename = ( const char * ) osrc->data_ptr;
    int eof = 0;

    if ( !osrc->ptr_internal )
    {
      /* WARN( "OPEN %s", filename ); */
      osrc->ptr_internal = ( void * ) fopen( filename, "r" );
    }
    if ( !osrc->ptr_internal )
    {
      /* WARN( "ERROR %s", filename ); */
      QRGSRC( osrc, -1 );
      osrc->error = 1;
    }
    if ( !osrc->error )
    {
      targ = source_load__targ_stream( ( FILE * ) osrc->ptr_internal, osrc, targ, &eof );
      if ( eof && osrc->ptr_internal )
      {
        /* WARN( "CLOSE %s", filename ); */
        fclose( ( FILE * ) osrc->ptr_internal );
        osrc->ptr_internal = NULL;
      }
    }
  }

  return targ;
}

static mucs_source_t default_sources[] = {
  [MUCS_SOURCE_STRING] = {
                          .type = MUCS_SOURCE_STRING,
                          .count = 0,
                          .data_ptr = NULL,
                          .delim = ':',
                          .delims = ":",
                          .eq = "=",
                          .check_fun = source_check_string,
                          .open_fun = NULL,
                          .close_fun = NULL,
                          .load_string_fun = source_load_string_string,
                          .load_targ_fun = source_load_targ_string,
                          .pref_ids = {
                                       {
                                        .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_LONG,.string = "" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                        },
                                       },
                          },
  [MUCS_SOURCE_ENV] = {
                       .type = MUCS_SOURCE_ENV,
                       .count = 0,
                       .data_ptr = "MAS_TEST_ENV",
                       .delim = ':',
                       .delims = ":",
                       .eq = "=",
                       .check_fun = source_check_env,
                       .open_fun = NULL,
                       .close_fun = NULL,
                       .load_string_fun = source_load_string_env,
                       .load_targ_fun = source_load_targ_env,
                       .pref_ids = {
                                    {
                                     .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                     },
                                    {
                                     .id = MUCS_VARIANT_LONG,.string = "" /* */
                                     },
                                    {
                                     .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                     },
                                    },
                       },
  [MUCS_SOURCE_ARGV] = {
                        .type = MUCS_SOURCE_ARGV,
                        .count = 0,
                        .data_ptr = NULL,
                        .delim = ':',
                        .delims = 0,
                        .eq = "=",
                        .check_fun = NULL,
                        .open_fun = NULL,
                        .close_fun = NULL,
                        .load_string_fun = source_load_string_argv,
                        .load_targ_fun = source_load_targ_argv,
                        .pref_ids = {
                                     {
                                      .id = MUCS_VARIANT_SHORT,.string = "-" /* */
                                      },
                                     {
                                      .id = MUCS_VARIANT_LONG,.string = "--" /* */
                                      },
                                     {
                                      .id = MUCS_VARIANT_NONOPT,.string = NULL /* */
                                      },
                                     },
                        },
  [MUCS_SOURCE_MARGV] = {
                         .type = MUCS_SOURCE_MARGV,
                         .count = 0,
                         .data_ptr = NULL,
                         .delim = ':',
                         .delims = 0,
                         .eq = "=",
                         .check_fun = NULL,
                         .open_fun = NULL,
                         .close_fun = NULL,
                         .load_string_fun = source_load_string_margv,
                         .load_targ_fun = source_load_targ_margv,
                         .pref_ids = {
                                      {
                                       .id = MUCS_VARIANT_SHORT,.string = "-" /* */
                                       },
                                      {
                                       .id = MUCS_VARIANT_LONG,.string = "--" /* */
                                       },
                                      {
                                       .id = MUCS_VARIANT_NONOPT,.string = NULL /* */
                                       },
                                      },
                         },
  [MUCS_SOURCE_STREAM] = {
                          .type = MUCS_SOURCE_STREAM,
                          .count = 0,
                          .data_ptr = NULL,
                          .delim = ':',
                          .delims = ":\r\n",
                          .eq = "=",
                          .check_fun = NULL,
                          .open_fun = NULL,
                          .close_fun = NULL,
                          .load_string_fun = NULL /*source_load_string_stream */ ,
                          .load_targ_fun = source_load_targ_stream,
                          .pref_ids = {
                                       {
                                        .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_LONG,.string = "" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_IGNORE,.string = "#" /* */
                                        },
                                       },
                          },
  [MUCS_SOURCE_STDIN] = {
                         .type = MUCS_SOURCE_STDIN,
                         .count = 0,
                         .data_ptr = NULL,
                         .delim = ':',
                         .delims = ":\r\n",
                         .eq = "=",
                         .check_fun = NULL,
                         .open_fun = NULL,
                         .close_fun = NULL,
                         .load_string_fun = NULL /*source_load_string_stream */ ,
                         .load_targ_fun = source_load_targ_stream,
                         .pref_ids = {
                                      {
                                       .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                       },
                                      {
                                       .id = MUCS_VARIANT_LONG,.string = "" /* */
                                       },
                                      {
                                       .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                       },
                                      {
                                       .id = MUCS_VARIANT_IGNORE,.string = "#" /* */
                                       },
                                      },
                         },
  [MUCS_SOURCE_FILE] = {
                        .type = MUCS_SOURCE_FILE,
                        .count = 0,
                        .data_ptr = NULL,
                        .delim = ':',
                        .delims = ":\n",
                        .eq = "=",
                        .check_fun = NULL,
                        .open_fun = NULL,
                        .close_fun = NULL,
                        .load_string_fun = NULL /*source_load_string_stream */ ,
                        .load_targ_fun = source_load_targ_file,
                        .pref_ids = {
                                     {
                                      .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                      },
                                     {
                                      .id = MUCS_VARIANT_LONG,.string = "" /* */
                                      },
                                     {
                                      .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                      },
                                     {
                                      .id = MUCS_VARIANT_IGNORE,.string = "#" /* */
                                      },
                                     },
                        },
  [MUCS_SOURCE_CONFIG] = {
                          .type = MUCS_SOURCE_CONFIG,
                          .count = 0,
                          .data_ptr = NULL,
                          .delim = ':',
                          .delims = ":\n",
                          .eq = "=",
                          .check_fun = NULL,
                          .open_fun = NULL,
                          .close_fun = NULL,
                          .load_string_fun = NULL /*source_load_string_stream */ ,
                          .load_targ_fun = source_load_targ_file,
                          .pref_ids = {
                                       {
                                        .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_LONG,.string = "" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                        },
                                       {
                                        .id = MUCS_VARIANT_IGNORE,.string = "#" /* */
                                        },
                                       },
                          },
  [MUCS_SOURCE_LIBCONFIG] = {
                             .type = MUCS_SOURCE_LIBCONFIG,
                             .count = 0,
                             .data_ptr = MAS_SYSCONFDIR "/" PACKAGE_NAME ".cfg",
                             .delim = ':',
                             .delims = ":\n",
                             .eq = "=",
                             .check_fun = NULL,
                             .open_fun = NULL,
                             .close_fun = NULL,
                             .load_string_fun = NULL /*source_load_string_stream */ ,
                             .load_targ_fun = source_load_targ_file,
                             .pref_ids = {
                                          {
                                           .id = MUCS_VARIANT_SHORT,.string = "@short@" /* */
                                           },
                                          {
                                           .id = MUCS_VARIANT_LONG,.string = "" /* */
                                           },
                                          {
                                           .id = MUCS_VARIANT_NONOPT,.string = "@@@@" /* */
                                           },
                                          {
                                           .id = MUCS_VARIANT_IGNORE,.string = "#" /* */
                                           },
                                          },
                             },
};

static void constructor_main(  ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main(  )
{
  FILE *f = stdin;

  if ( isatty( fileno( f ) ) )
    f = NULL;
  default_sources[MUCS_SOURCE_STDIN].data_ptr = ( const void * ) f;
}

size_t
mucs_source_defaults_count( void )
{
  return sizeof( default_sources ) / sizeof( default_sources[0] );
}

const mucs_source_t *
mucs_source_defaults( void )
{
  return default_sources;
}

const mucs_source_t *
mucs_source_default( size_t n )
{
  return &default_sources[n];
}
