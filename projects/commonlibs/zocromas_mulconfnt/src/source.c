#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"

#include "option_base.h"
#include "option.h"
#include "option_tablist.h"

#include "source_defaults.h"
#include "source_base.h"

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

char *
mucs_source_load_string( mucs_source_t * osrc )
{
  if ( osrc && osrc->load_string_fun )
    mucs_source_set_string_ptr( osrc, osrc->load_string_fun( osrc ) );
  return osrc ? osrc->string : NULL;
}

int
mucs_source_load_targ( mucs_source_t * osrc )
{
  rDECLBAD;
  if ( osrc )
  {
    rSETGOOD;
    if ( osrc->npos == osrc->targ_loaded )
    {
      mas_argvc_delete( &osrc->oldtarg );
      osrc->oldtarg = osrc->targ;
      osrc->targ.argc = 0;
      osrc->targ.argv = NULL;
//    mas_argvc_delete( &osrc->targ );
      if ( osrc && osrc->load_targ_fun )
      {
        osrc->targ = osrc->load_targ_fun( osrc, osrc->targ );
      }
      osrc->targ_loaded++;
    }
    else if ( osrc->npos != osrc->targ_loaded - 1 )
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

static int
match_arg( const char *pref, const char *arg )
{
  unsigned i = 0;

  if ( !pref || !*pref )
    return 0;
  for ( i = 0; pref[i] && arg[i]; i++ )
    if ( !arg[i] || pref[i] != arg[i] )
      break;
/*if ( !arg[i] )
    i = 0; */
  return i > 0 ? ( int ) i : -1;
}

/* determine variant by prefix */
static mucs_variant_t
max_match_id( mucs_source_t * osrc, const char *arg )
{
  int maxmatch = -1;
  mucs_variant_t maxmatchid = MUCS_VARIANT_MAX;

  if ( !osrc->lastoptpos || osrc->curarg <= osrc->lastoptpos )
  {
    for ( unsigned i = 0; osrc && ( i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ) ); i++ )
    {
      int len = match_arg( osrc->pref_ids[i].string, arg );

      if ( len > maxmatch )
      {
        maxmatch = len;
        maxmatchid = osrc->pref_ids[i].id;
      }
    }
  }
  else
  {
    maxmatchid = MUCS_VARIANT_NONOPT;
  }
  return maxmatchid;
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
mucs_source_dealias_opt( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, const mucs_option_t * found_topt_ _uUu_,
                         mucs_optscanner_t * optscan )
{
  rDECLGOOD;
  optscan->force_value = optscan->string_value;
  while ( rGOOD && optscan->found_topt && optscan->found_topt->restype == MUCS_RTYP_ALIAS && optscan->found_topt->argptr )
    rC( mucs_config_option_tablist_lookup( tablist, ( char * ) optscan->found_topt->argptr, osrc->eq, optscan ) );
  optscan->force_value = NULL;
  rRET;
}

static int
mucs_source_found_opt( mucs_source_t * osrc, mucs_option_t * opt )
{
  rDECLGOOD;

  if ( opt && osrc )
    opt->source = osrc;

  if ( mucs_config_option_flag( opt, MUCS_FLAG_LASTOPT ) )
    osrc->lastoptpos = osrc->curarg;

  if ( rGOOD )
  {
    mucs_option_callback_t cb = NULL;

    opt->extra_cb.source = osrc;
    if ( !( mucs_config_option_flag( opt, MUCS_FLAG_NO_COMMON_CB_IF_VALUE ) && opt->value_is_set ) && osrc->common_callback
         && !mucs_config_option_flag( opt, MUCS_FLAG_NO_COMMON_CB ) )
    {
      cb = osrc->common_callback;
      if ( cb )
      {
        cb( opt );
        osrc->extra_cb.callback_called++;
      }
    }
    if ( !( mucs_config_option_flag( opt, MUCS_FLAG_NO_TYPE_CB_IF_VALUE ) && opt->value_is_set ) && osrc->type_callbacks
         && !mucs_config_option_flag( opt, MUCS_FLAG_NO_TYPE_CB ) )
    {
      cb = osrc->type_callbacks[opt->restype & ~MUCS_RTYP_FLAG_ALL];
      if ( cb )
      {
        cb( opt );
        osrc->extra_cb.callback_called++;
      }
    }
    opt->extra_cb.source = NULL;
  }
  if ( opt->has_value > 0 )
    osrc->curarg += opt->has_value - 1;
/* TODO additional actions here !! */
  rRET;
}

static int
mucs_source_lookup_opt( mucs_source_t * osrc, const mucs_option_table_list_t * tablist, mucs_optscanner_t * optscan )
{
  rDECLBAD;

  const char *arg_nopref = optscan->arg + optscan->preflen;

  rC( mucs_config_option_tablist_lookup( tablist, arg_nopref, osrc->eq, optscan ) );
  rC( mucs_source_dealias_opt( osrc, tablist, optscan->found_topt, optscan ) );
  if ( rGOOD )
  {
    if ( optscan->found_topt )                                       /* TODO : move to mucs_config_option_tablist_lookup */
    {
      mucs_option_t *opt = NULL;

      rSETBAD;

      opt = mucs_config_option_clone( optscan->found_topt );
      if ( opt )
      {
        rSETGOOD;
        if ( optscan->has_value )
        {
          rC( mucs_config_option_set_value( opt, optscan->string_value ) );
          if ( rGOOD )
          {
            if ( opt->callback && !opt->argptr )
            {
              opt->extra_cb.tablist = tablist;
              opt->extra_cb.source = osrc;
              opt->callback( opt );
              opt->extra_cb.source = NULL;
              opt->extra_cb.tablist = NULL;
              opt->extra_cb.callback_called++;
            }
            opt->has_value = optscan->has_value;
          }
        }

/* do something for found option */
        rC( mucs_source_found_opt( osrc, opt ) );
        mucs_config_option_delete( opt );
        opt = NULL;
      }
    }
  }
  rRET;
}

int
mucs_source_lookup_optscan( mucs_source_t * osrc, mucs_optscanner_t * optscan, const mucs_option_table_list_t * tablist )
{
  rDECLBAD;
  if ( optscan->variantid == MUCS_VARIANT_BAD )
  {
#if 0
    static const char *labels[MUCS_VARIANTS] = { "SHORT", "LONG", "NONOPT", "BAD" };
    WARN( "NO VARIANT [%s] arg='%s';\n", labels[variantid], optscan->arg );
#endif
  }
  else if ( optscan->variantid == MUCS_VARIANT_NONOPT )
  {
    mas_add_argvc_arg( &osrc->targno, optscan->arg + optscan->preflen );
    rSETGOOD;
  }
  else
  {
    if ( osrc->curarg < osrc->targ.argc - 1 )
      optscan->nextarg = osrc->targ.argv[osrc->curarg + 1];
    rC( mucs_source_lookup_opt( osrc, tablist, optscan ) );
  /* TODO if 
   *       1. unrecognized option
   *       2. do not stop on error flag
   *         then -- add unrecognized option as MUCS_VARIANT_NONOPT (or something else !?)
   * */
  }
  rRET;
}

int
mucs_source_lookup_arg( mucs_source_t * osrc, const char *arg, const mucs_option_table_list_t * tablist )
{
  rDECLBAD;
  mucs_variant_t vid = max_match_id( osrc, arg );

/* max_match_id: determine variant by prefix */
  mucs_optscanner_t optscan = {
    .arg = arg,
    .variantid = vid,
    .preflen = osrc->pref_ids[vid].string ? strlen( osrc->pref_ids[vid].string ) : 0,
  };
  rC( mucs_source_lookup_optscan( osrc, &optscan, tablist ) );
  rRET;
}

int
mucs_source_lookup_seq( mucs_source_t * osrc, const mucs_option_table_list_t * tablist )
{
  rDECLGOOD;
  int nargs = 0;

  rC( mucs_source_load_targ( osrc ) );
  osrc->lastoptpos = 0;

  for ( osrc->curarg = 0; osrc && !masregerrs_count_all_default( NULL, TRUE ) && osrc->curarg < osrc->targ.argc; osrc->curarg++ )
  {
    nargs++;
    rC( mucs_source_lookup_arg( osrc, osrc->targ.argv[osrc->curarg], tablist ) );
  }
  if ( rGOOD )
    rCODE = nargs;
/* return -nargs; */
  rRET;
}

int
mucs_source_lookup_all( mucs_source_t * osrc, const mucs_option_table_list_t * tablist )
{
  rDECLBAD;
  if ( osrc )
  {
    do
    {
      rC( mucs_source_lookup_seq( osrc, tablist ) );
      if ( rBAD )
        break;
      osrc->npos++;
    } while ( rCODE > 0 );
  }
  rRET;
}

int
mucs_source_lookup( mucs_source_t * osrc, const mucs_option_table_list_t * tablist )
{
  rDECLBAD;

  if ( osrc )
  {
  /* XXX ??? XXX */
    osrc->npos = 0;
    rC( mucs_source_lookup_seq( osrc, tablist ) );
  }
  rRET;
}

unsigned long
mucs_source_flags( mucs_source_t * osrc )
{
  return osrc ? osrc->flags : 0L;
}

int
mucs_source_flag( mucs_source_t * osrc, unsigned long mask )
{
  return osrc && ( ( osrc->flags & mask ) ? 1 : 0 );
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
mucs_source_set_type_callback( mucs_source_t * osrc, mucs_restype_t restype, mucs_option_callback_t cb )
{
  rDECLBAD;
  if ( osrc && restype >= 0 && restype <= MUCS_RTYP_MAX )
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
      {
        char *p = get_current_dir_name(  );
        char *p1 = canonicalize_file_name( p );

        if ( errno )
        {
          rSETBAD;
        /* WARN( "(%s) path:%s", strerror( errno ), path ); */
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
        /* WARN( "(%s) path:%s", strerror( errno ), path ); */
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
