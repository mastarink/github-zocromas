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
source_check_env( int count __attribute__ ( ( unused ) ), void *data_ptr __attribute__ ( ( unused ) ), const char *delims
                  __attribute__ ( ( unused ) ), const char *eq __attribute__ ( ( unused ) ), const config_prefix_encoder_t * pref_ids
                  __attribute__ ( ( unused ) ) )
{
  return data_ptr && getenv( ( char * ) data_ptr ) ? 1 : 0;
}

static int
source_check_direct( int count __attribute__ ( ( unused ) ), void *data_ptr __attribute__ ( ( unused ) ), const char *delims
                     __attribute__ ( ( unused ) ), const char *eq __attribute__ ( ( unused ) ), const config_prefix_encoder_t * pref_ids
                     __attribute__ ( ( unused ) ) )
{
  return 1;
}

static char *
source_load_string_direct( config_source_desc_t * desc __attribute__ ( ( unused ) ) )
{
  return desc->data_ptr ? mas_strdup( desc->data_ptr ) : NULL;
}

static mas_argvc_t
source_load_targ_direct( config_source_desc_t * desc, mas_argvc_t targ )
{
  if ( desc->data_ptr )
    mas_add_argvc_args_d( &targ, ( char * ) desc->data_ptr, 0, desc->delims );
  return targ;
}

static char *
source_load_string_env( config_source_desc_t * desc )
{
  return desc->data_ptr ? mas_strdup( getenv( ( char * ) desc->data_ptr ) ) : NULL;
}

static char *
source_load_string_argv( config_source_desc_t * desc )
{
  char *s = NULL;

  s = mas_argv_join( desc->count, ( char ** ) desc->data_ptr, 0, desc->delim );
  return s;
}

static mas_argvc_t
source_load_targ_env( config_source_desc_t * desc, mas_argvc_t targ )
{
  if ( desc->data_ptr )
    mas_add_argvc_args_d( &targ, getenv( ( char * ) desc->data_ptr ), 0, desc->delims );
  return targ;
}

static mas_argvc_t
source_load_targ_argv( config_source_desc_t * desc, mas_argvc_t targ )
{
  if ( desc->data_ptr )
    mas_add_argvc_argv( &targ, desc->count, ( char ** ) desc->data_ptr, 0 );
  return targ;
}

static config_source_desc_t default_sources[] =                      /* */
{                                                                    /* */
  [MULCONF_SOURCE_DIRECT] = {                                        /* */
                             .type = MULCONF_SOURCE_DIRECT,          /* */
                             .count = 0,                             /* */
                             .data_ptr = NULL,                       /* */
                             .delim = ':',                           /* */
                             .delims = ":",                          /* */
                             .eq = "=",                              /* */
                             .check_fun = source_check_direct,       /* */
                             .open_fun = NULL,                       /* */
                             .close_fun = NULL,                      /* */
                             .load_string_fun = source_load_string_direct, /* */
                             .load_targ_fun = source_load_targ_direct, /* */
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
