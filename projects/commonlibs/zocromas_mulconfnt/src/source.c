#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_arg_tools.h> */
/* #include <mastar/tools/mas_argvc_tools.h> */

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"

/* #include "global.h" */

#include "option_base.h"
/* #include "option.h" */
#include "option_flags.h"
/* #include "option_tablist.h" */

#include "optscanner_base.h"
#include "optscanner.h"

/* #include "source_defaults.h" */
/* #include "source_base.h" */

#include "source.h"

/*
 *
 * mucs_source_...
 *
 * */

void
mucs_source_set_string_ptr( mucs_source_t * osrc, char *string )
{
  if ( osrc )
  {
    if ( osrc->string )
      mas_free( osrc->string );
    osrc->string = NULL;
    if ( osrc )
      osrc->string = string;
  }
}

int
mucs_source_open( mucs_source_t * osrc )
{
  rDECLGOOD;

  if ( osrc )
  {
    if ( !osrc->ptr_internal && osrc->open_fun )
      osrc->ptr_internal = osrc->open_fun( osrc->name );
  }
  else
    rSETBAD;
  rRET;
}

int
mucs_source_close( mucs_source_t * osrc )
{
  rDECLGOOD;

  if ( osrc )
  {
    if ( osrc->ptr_internal && osrc->close_fun )
      osrc->close_fun( osrc->ptr_internal );
    osrc->ptr_internal = NULL;
  }
  else
    rSETBAD;
  rRET;
}

char *
mucs_source_load_string( mucs_source_t * osrc )
{
  if ( osrc && osrc->load_string_fun )
  {
    mucs_source_open( osrc );
    mucs_source_set_string_ptr( osrc, osrc->load_string_fun( osrc ) );
  }
  return osrc ? osrc->string : NULL;
}

int
mucs_source_load_targ( mucs_source_t * osrc )
{
  rDECLBAD;
  if ( osrc )
  {
    rSETGOOD;
    if ( osrc->ngroup == osrc->targ_loaded )
    {
      mas_argvc_delete( &osrc->oldtarg );
      osrc->oldtarg = osrc->targ;
      osrc->targ.argc = 0;
      osrc->targ.argv = NULL;
//    mas_argvc_delete( &osrc->targ );
      if ( osrc && osrc->load_targ_fun )
      {
        /* rC( */ mucs_source_open( osrc ) /* ) */ ;
        if ( rGOOD )
          osrc->targ = osrc->load_targ_fun( osrc, osrc->targ );
      }
      osrc->targ_loaded++;
    }
    else if ( osrc->ngroup != osrc->targ_loaded - 1 )
    {
      mas_argvc_delete( &osrc->oldtarg );
      osrc->oldtarg = osrc->targ;
      osrc->targ.argc = 0;
      osrc->targ.argv = NULL;
//    mas_argvc_delete( &osrc->targ );
    }
  }
/* return osrc ? &osrc->targ : NULL; */
  rRET;
}



char **
mucs_source_argv_no( mucs_source_t * osrc )
{
  return osrc ? osrc->targno.argv : NULL;
}

int
mucs_source_argc_no( mucs_source_t * osrc )
{
  return osrc ? osrc->targno.argc : 0;
}

const char *
mucs_source_arg_no( mucs_source_t * osrc, int i )
{
  return osrc && i >= 0 && i < osrc->targno.argc ? osrc->targno.argv[i] : NULL;
}

int
mucs_source_found_opt( mucs_source_t * osrc, mucs_option_t * opt, void *userdata )
{
  rDECLGOOD;

  if ( opt && osrc )
    opt->d.source = osrc;

  if ( mucs_config_option_flag( opt, MUCS_FLAG_LASTOPT ) )
    osrc->lastoptpos = osrc->curarg;

  if ( rGOOD )
  {
    mucs_option_callback_t cb = NULL;

    opt->d.extra_cb.source = osrc;
    if ( !( mucs_config_option_flag( opt, MUCS_FLAG_NO_COMMON_CB_IF_VALUE ) && opt->d.value_is_set ) && osrc->common_callback
         && !mucs_config_option_flag( opt, MUCS_FLAG_NO_COMMON_CB ) )
    {
      cb = osrc->common_callback;
      if ( cb )
      {
        cb( opt, userdata, opt->s.extra_data );
        osrc->extra_cb.callback_called++;
      }
    }
    if ( !( mucs_config_option_flag( opt, MUCS_FLAG_NO_TYPE_CB_IF_VALUE ) && opt->d.value_is_set ) && osrc->type_callbacks
         && !mucs_config_option_flag( opt, MUCS_FLAG_NO_TYPE_CB ) )
    {
      cb = osrc->type_callbacks[opt->s.restype & ~MUCS_RTYP_FLAG_ALL];
      if ( cb )
      {
        cb( opt, userdata, opt->s.extra_data );
        osrc->extra_cb.callback_called++;
      }
    }
    opt->d.extra_cb.source = NULL;
  }
  if ( opt->d.has_value > 0 )                                        /* no matter for opt->d.has_value == 1 , i.e. may be replaced with "> 1" */
    osrc->curarg += opt->d.has_value - 1;
/* TODO additional actions here !! */
  rRET;
}

int
mucs_source_lookup_seq( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, void *userdata )
{
  rDECLGOOD;
  int nargs = 0;

  rC( mucs_source_load_targ( osrc ) );
  osrc->lastoptpos = 0;

  for ( osrc->curarg = 0; osrc && rGOOD /*!masregerrs_count_all_default( NULL, TRUE ) */  && osrc->curarg < osrc->targ.argc; osrc->curarg++ )
  {
    mucs_optscanner_t optscan = { 0 };
    nargs++;

    mucs_optscanner_init( &optscan, osrc, tablist );
    do
    {
      rC( mucs_optscanner_next_word( &optscan ) );
      rC( mucs_optscanner_lookup_arg( &optscan, userdata ) );
    } while ( rGOOD && optscan.variantid != MUCS_VARIANT_NONOPT && optscan.at_arg && strchr( " \t", *optscan.at_arg ) );
    mucs_optscanner_reset( &optscan );
  }
  if ( rGOOD )
    rCODE = nargs;
/* return -nargs; */
  rRET;
}

int
mucs_source_lookup_all( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, void *userdata )
{
  rDECLBAD;
  if ( osrc )
  {
    osrc->ngroup = osrc->targ_loaded = 0;
    if ( osrc->pass >= osrc->min_pass && ( !osrc->npasses || ( osrc->pass < osrc->min_pass + osrc->npasses ) ) )
    {
      do
      {
        rC( mucs_source_lookup_seq( osrc, tablist, userdata ) );
        if ( rBAD )
          break;
        osrc->ngroup++;
      } while ( rCODE > 0 );
    }
    else
    {
      rSETGOOD;                                                      /* not an error, just skip */
    }
    osrc->pass++;
  }
  rRET;
}

int
mucs_source_set_common_callback( mucs_source_t * osrc, mucs_option_callback_t cb )
{
  rDECLBAD;
  if ( osrc )
  {
    rSETGOOD;
    osrc->common_callback = cb;
  }
  rRET;
}

int
mucs_source_set_type_callback( mucs_source_t * osrc, mucs_restypex_t restype, mucs_option_callback_t cb )
{
  rDECLBAD;
  if ( osrc && /* restype >= 0 && */ restype <= MUCS_RTYP_MAX )
  {
    rSETGOOD;
    osrc->type_callbacks[restype] = cb;
  }
  rRET;
}

char *
mucs_source_wd( const mucs_source_t * osrc )
{
  rDECLBAD;
  char *path = NULL;

  if ( osrc )
  {
    errno = 0;
    switch ( osrc->type )
    {
    case MUCS_SOURCE_NONE:
    case MUCS_SOURCE_STRING:
    case MUCS_SOURCE_ENV:
    case MUCS_SOURCE_ARGV:
    case MUCS_SOURCE_MARGV:
    case MUCS_SOURCE_STREAM:
    case MUCS_SOURCE_STDIN:
    case MUCS_SOURCE_READLINE:
      {
        char *p = get_current_dir_name(  );
        char *p1 = canonicalize_file_name( p );

        if ( errno )
        {
          rSETBAD;
          QRGSRCM( osrc, rCODE, "%s: %s", strerror( errno ), p );
        }
        else
        {
          rSETGOOD;
          path = mas_strdup( p1 );
        }
        if ( p1 )
          free( p1 );
        if ( p )
          free( p );
      }
      break;
    case MUCS_SOURCE_FILE:
    case MUCS_SOURCE_LIBCONFIG:
    case MUCS_SOURCE_CONFIG:
      {
        char *p = canonicalize_file_name( ( char * ) osrc->data_ptr );

        if ( errno )
        {
          rSETBAD;
          QRGSRCM( osrc, rCODE, "%s: %s", strerror( errno ), p );
        }
        else
        {
          rSETGOOD;
          path = mas_strdup( dirname( p ) );
        }
        if ( p )
          free( p );
      }
      break;
    }
  }
  return path;
}

char *
mucs_source_absfile( const mucs_source_t * osrc, const char *fn )
{
  rDECLBAD;
  char *path = NULL;

  if ( osrc && fn )
  {
    char *fname = mas_expand_string( fn );

    errno = 0;

    if ( *fname == '/' )
    {
      char *p = canonicalize_file_name( fname );

      if ( errno )
      {
        rSETBAD;
        QRGSRCM( osrc, rCODE, "%s: %s", strerror( errno ), fname );
      }
      else
      {
        rSETGOOD;
        path = mas_strdup( p );
      }
      free( p );
    }
    else
    {
      path = mucs_source_wd( osrc );
      path = mas_strcat_x( path, "/" );
      path = mas_strcat_x( path, fname );
    }
    mas_free( fname );
  }
  return path;
}
