#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list.h"

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
                                         }
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

void
mulconfnt_list_init( config_source_list_t * list )
{
  memset( list, 0, sizeof( config_source_list_t ) );
}

void
mulconfnt_list_integrate( config_source_list_t * list )
{
  for ( config_source_desc_t * osrc = list->first; osrc; osrc = osrc->next )
  {
    mulconfnt_source_load_targ( osrc );
    mas_add_argvc_argvc( &list->targ, &osrc->targ, 0 );
  }
}

void
mulconfnt_list_close( config_source_list_t * list )
{
  config_source_desc_t *osrc = list->first;

  while ( osrc )
  {
    config_source_desc_t *next = osrc->next;

    mulconfnt_source_close( osrc );
    osrc = next;
  }
  list->first = NULL;
  mas_argvc_delete( &list->targ );
}

static void
mulconfnt_list_add( config_source_list_t * list, config_source_desc_t * osrc )
{
  if ( list )
  {
    if ( list->first )
    {
      config_source_desc_t *os = list->first;

      while ( os && os->next )
      {
        os = os->next;
      }
      os->next = osrc;
    }
    else
      list->first = osrc;
  }
}

config_source_desc_t *
mulconfnt_list_add_source( config_source_list_t * list, config_source_t source_type, int count, void *data_ptr, const char *delims, const char *eq,
                           const config_prefix_encoder_t * pref_ids )
{
  config_source_desc_t *osrc = NULL;

  if ( source_type >= 0 && source_type < sizeof( default_sources ) / sizeof( default_sources[0] ) )
  {
    config_source_desc_t *defsrc = &default_sources[source_type];

    if ( defsrc && source_type == defsrc->type )
    {
      int check = !defsrc->check_fun || defsrc->check_fun( count, data_ptr, delims, eq, pref_ids );

      if ( check )
      {
        osrc = mas_malloc( sizeof( config_source_desc_t ) );
        memset( osrc, 0, sizeof( config_source_desc_t ) );

        osrc->type = source_type;
        osrc->count = count;

        osrc->delim = defsrc->delim;

        if ( delims )
          osrc->delims = mas_strdup( delims );
        else if ( defsrc->delims )
          osrc->delims = mas_strdup( defsrc->delims );

        if ( eq )
          osrc->eq = mas_strdup( eq );
        else if ( defsrc->eq )
          osrc->eq = mas_strdup( defsrc->eq );

        for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
        {
          osrc->pref_ids[i] = pref_ids ? pref_ids[i] : defsrc->pref_ids[i];
          if ( osrc->pref_ids[i].string )
            osrc->pref_ids[i].string = mas_strdup( osrc->pref_ids[i].string );
        }

        osrc->data_ptr = data_ptr ? data_ptr : defsrc->data_ptr;
        osrc->check_fun = defsrc->check_fun;
        osrc->open_fun = defsrc->open_fun;
        osrc->close_fun = defsrc->close_fun;
        osrc->load_string_fun = defsrc->load_string_fun;
        osrc->load_targ_fun = defsrc->load_targ_fun;
        osrc->next = NULL;
      }
    }
  }
  mulconfnt_list_add( list, osrc );
  return osrc;
}
