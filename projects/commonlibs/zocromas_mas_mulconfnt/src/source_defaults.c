#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source_defaults.h"

static int
source_check_env( int count _uUu_, const void *data_ptr _uUu_, const char *delims
                  _uUu_, const char *eq _uUu_, const config_prefix_encoder_t * pref_ids _uUu_ )
{
  return data_ptr && getenv( ( char * ) data_ptr ) ? 1 : 0;
}

static int
source_check_string( int count _uUu_, const void *data_ptr _uUu_, const char *delims
                     _uUu_, const char *eq _uUu_, const config_prefix_encoder_t * pref_ids _uUu_ )
{
  return 1;
}

static char *
source_load_string_string( config_source_desc_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_strdup( osrc->data_ptr ) : NULL;
}

static mas_argvc_t
source_load_targ_string( config_source_desc_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && !osrc->npos )
    mas_add_argvc_args_d( &targ, ( char * ) osrc->data_ptr, 0, osrc->delims );
  return targ;
}

static char *
source_load_string_env( config_source_desc_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_strdup( getenv( ( char * ) osrc->data_ptr ) ) : NULL;
}

static char *
source_load_string_argv( config_source_desc_t * osrc )
{
  return osrc && osrc->data_ptr && !osrc->npos ? mas_argv_join( osrc->count, ( char ** ) osrc->data_ptr, 0, osrc->delim ) : NULL;
}

static char *
source_load_string_margv( config_source_desc_t * osrc )
{
  return osrc && osrc->data_ptr
          && osrc->npos < osrc->count ? mas_argv_join( 0, ( ( char *** ) osrc->data_ptr )[osrc->npos], 0 /* osrc->npos > 0 ? 1 : 0 */ ,
                                                       osrc->delim ) : NULL;
}

static mas_argvc_t
source_load_targ_env( config_source_desc_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && !osrc->npos )
    mas_add_argvc_args_d( &targ, getenv( ( char * ) osrc->data_ptr ), 0, osrc->delims );
  return targ;
}

static mas_argvc_t
source_load_targ_argv( config_source_desc_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && !osrc->npos )
    mas_add_argvc_argv( &targ, osrc->count, ( char ** ) osrc->data_ptr, 0 );
  return targ;
}

static mas_argvc_t
source_load_targ_margv( config_source_desc_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr && osrc->npos < osrc->count )
    mas_add_argvc_argv( &targ, 0, ( ( char *** ) osrc->data_ptr )[osrc->npos], 0 /* osrc->npos > 0 ? 1 : 0 */  );

  return targ;
}

static mas_argvc_t
source_load_targ_stream( config_source_desc_t * osrc, mas_argvc_t targ )
{
  if ( osrc && osrc->data_ptr )
  {
    FILE *fin = ( FILE * ) osrc->data_ptr;
    char buffer[1024 * 10];
    char *string = NULL;
    const char *ignpref = NULL;

    do
    {
      string = fgets( buffer, sizeof( buffer ), fin );
      string = mas_chomp( string );
      ignpref = osrc->pref_ids[MULCONF_VARIANT_IGNORE].string;
    } while ( ( string && !*string ) || ( ignpref && string && 0 == strncmp( ignpref, string, strlen( ignpref ) ) ) );
    if ( do_fprintf )
      fprintf( stderr, "READ '%s'\n", string );
    if ( string && *string )
      mas_add_argvc_args_d( &targ, string, 0, osrc->delims );
  }

  return targ;
}

static config_source_desc_t default_sources[] = {
  [MULCONF_SOURCE_STRING] = {
                             .type = MULCONF_SOURCE_STRING,
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
                                           .id = MULCONF_VARIANT_SHORT,.string = "@short@" /* */
                                           },
                                          {
                                           .id = MULCONF_VARIANT_LONG,.string = "" /* */
                                           },
                                          {
                                           .id = MULCONF_VARIANT_NONOPT,.string = "@@@@" /* */
                                           },
                                          },
                             },
  [MULCONF_SOURCE_ENV] = {
                          .type = MULCONF_SOURCE_ENV,
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
                                        .id = MULCONF_VARIANT_SHORT,.string = "@short@" /* */
                                        },
                                       {
                                        .id = MULCONF_VARIANT_LONG,.string = "" /* */
                                        },
                                       {
                                        .id = MULCONF_VARIANT_NONOPT,.string = "@@@@" /* */
                                        },
                                       },
                          },
  [MULCONF_SOURCE_ARGV] = {
                           .type = MULCONF_SOURCE_ARGV,
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
                                         .id = MULCONF_VARIANT_SHORT,.string = "-" /* */
                                         },
                                        {
                                         .id = MULCONF_VARIANT_LONG,.string = "--" /* */
                                         },
                                        {
                                         .id = MULCONF_VARIANT_NONOPT,.string = NULL /* */
                                         },
                                        },
                           },
  [MULCONF_SOURCE_MARGV] = {
                            .type = MULCONF_SOURCE_MARGV,
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
                                          .id = MULCONF_VARIANT_SHORT,.string = "-" /* */
                                          },
                                         {
                                          .id = MULCONF_VARIANT_LONG,.string = "--" /* */
                                          },
                                         {
                                          .id = MULCONF_VARIANT_NONOPT,.string = NULL /* */
                                          },
                                         },
                            },
  [MULCONF_SOURCE_FILE] = {
                           .type = MULCONF_SOURCE_FILE,
                           .count = 0,
                           .data_ptr = NULL,
                           .delim = ';',
                           .delims = ";\n",
                           .eq = "=",
                           },
  [MULCONF_SOURCE_STREAM] = {
                             .type = MULCONF_SOURCE_STREAM,
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
                                           .id = MULCONF_VARIANT_SHORT,.string = "@short@" /* */
                                           },
                                          {
                                           .id = MULCONF_VARIANT_LONG,.string = "" /* */
                                           },
                                          {
                                           .id = MULCONF_VARIANT_NONOPT,.string = "@@@@" /* */
                                           },
                                          {
                                           .id = MULCONF_VARIANT_IGNORE,.string = "#" /* */
                                           },
                                          },
                             },
};

size_t
mucs_source_defaults_count( void )
{
  return sizeof( default_sources ) / sizeof( default_sources[0] );
}

const config_source_desc_t *
mucs_source_defaults( void )
{
  return default_sources;
}

const config_source_desc_t *
mucs_source_default( size_t n )
{
  return &default_sources[n];
}
