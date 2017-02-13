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
source_load_string_string( config_source_desc_t * desc _uUu_, int pos )
{
  return desc && desc->data_ptr && !pos ? mas_strdup( desc->data_ptr ) : NULL;
}

static mas_argvc_t
source_load_targ_string( config_source_desc_t * desc, mas_argvc_t targ, int pos )
{
  if ( desc && desc->data_ptr && !pos )
    mas_add_argvc_args_d( &targ, ( char * ) desc->data_ptr, 0, desc->delims );
  return targ;
}

static char *
source_load_string_env( config_source_desc_t * desc, int pos )
{
  return desc && desc->data_ptr && !pos ? mas_strdup( getenv( ( char * ) desc->data_ptr ) ) : NULL;
}

static char *
source_load_string_argv( config_source_desc_t * desc, int pos )
{
  return desc && desc->data_ptr && !pos ? mas_argv_join( desc->count, ( char ** ) desc->data_ptr, 0, desc->delim ) : NULL;
}

static char *
source_load_string_margv( config_source_desc_t * desc, int pos )
{
  return desc && desc->data_ptr && pos < desc->count ? mas_argv_join( 0, ( ( char *** ) desc->data_ptr )[pos], 0, desc->delim ) : NULL;
}

static mas_argvc_t
source_load_targ_env( config_source_desc_t * desc, mas_argvc_t targ, int pos )
{
  if ( desc && desc->data_ptr && !pos )
    mas_add_argvc_args_d( &targ, getenv( ( char * ) desc->data_ptr ), 0, desc->delims );

  return targ;
}

static mas_argvc_t
source_load_targ_argv( config_source_desc_t * desc, mas_argvc_t targ, int pos )
{
  if ( desc && desc->data_ptr && !pos )
    mas_add_argvc_argv( &targ, desc->count, ( char ** ) desc->data_ptr, 0 );
  return targ;
}

static mas_argvc_t
source_load_targ_margv( config_source_desc_t * desc, mas_argvc_t targ, int pos )
{
  if ( desc && desc->data_ptr && pos < desc->count )
    mas_add_argvc_argv( &targ, 0, ( ( char *** ) desc->data_ptr )[pos], pos > 0 ? 1 : 0 );

  return targ;
}

static config_source_desc_t default_sources[] =                      /* */
{                                                                    /* */
  [MULCONF_SOURCE_STRING] = {                                        /* */
                             .type = MULCONF_SOURCE_STRING,          /* */
                             .count = 0,                             /* */
                             .data_ptr = NULL,                       /* */
                             .delim = ':',                           /* */
                             .delims = ":",                          /* */
                             .eq = "=",                              /* */
                             .check_fun = source_check_string,       /* */
                             .open_fun = NULL,                       /* */
                             .close_fun = NULL,                      /* */
                             .load_string_fun = source_load_string_string, /* */
                             .load_targ_fun = source_load_targ_string, /* */
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
                                          },                         /* */
                             },                                      /* */
  [MULCONF_SOURCE_ENV] = {                                           /* */
                          .type = MULCONF_SOURCE_ENV,                /* */
                          .count = 0,                                /* */
                          .data_ptr = "MAS_TEST_ENV",                /* */
                          .delim = ':',                              /* */
                          .delims = ":",                             /* */
                          .eq = "=",                                 /* */
                          .check_fun = source_check_env,             /* */
                          .open_fun = NULL,                          /* */
                          .close_fun = NULL,                         /* */
                          .load_string_fun = source_load_string_env, /* */
                          .load_targ_fun = source_load_targ_env,     /* */
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
                                       },                            /* */
                          },                                         /* */
  [MULCONF_SOURCE_ARGV] = {                                          /* */
                           .type = MULCONF_SOURCE_ARGV,              /* */
                           .count = 0,                               /* */
                           .data_ptr = NULL,                         /* */
                           .delim = ':',                             /* */
                           .delims = 0,                              /* */
                           .eq = "=",                                /* */
                           .check_fun = NULL,                        /* */
                           .open_fun = NULL,                         /* */
                           .close_fun = NULL,                        /* */
                           .load_string_fun = source_load_string_argv, /* */
                           .load_targ_fun = source_load_targ_argv,   /* */
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
                                        },                           /* */
                           },                                        /* */
  [MULCONF_SOURCE_MARGV] = {                                         /* */
                            .type = MULCONF_SOURCE_MARGV,            /* */
                            .count = 0,                              /* */
                            .data_ptr = NULL,                        /* */
                            .delim = ':',                            /* */
                            .delims = 0,                             /* */
                            .eq = "=",                               /* */
                            .check_fun = NULL,                       /* */
                            .open_fun = NULL,                        /* */
                            .close_fun = NULL,                       /* */
                            .load_string_fun = source_load_string_margv, /* */
                            .load_targ_fun = source_load_targ_margv, /* */
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
                                         },                          /* */
                            },                                       /* */
  [MULCONF_SOURCE_STREAM] = {                                        /* */
                             .type = MULCONF_SOURCE_STREAM,          /* */
                             .count = 0,                             /* */
                             .data_ptr = NULL,                       /* */
                             .delim = ';',                           /* */
                             .delims = ";\n",                        /* */
                             .eq = "=",                              /* */
                             },                                      /* */
};                                                                   /* */

size_t
mulconfnt_source_defaults_count( void )
{
  return sizeof( default_sources ) / sizeof( default_sources[0] );
}

const config_source_desc_t *
mulconfnt_source_defaults( void )
{
  return default_sources;
}

const config_source_desc_t *
mulconfnt_source_default( size_t n )
{
  return &default_sources[n];
}
