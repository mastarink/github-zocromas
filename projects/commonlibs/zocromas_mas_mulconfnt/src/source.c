#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "option_base.h"
#include "option.h"
#include "option_tablist.h"

#include "source_defaults.h"
#include "source_base.h"

#include "source.h"

void
mulconfnt_source_set_string_ptr( config_source_desc_t * osrc, char *string )
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
mulconfnt_source_load_string( config_source_desc_t * osrc )
{
  if ( osrc && osrc->load_string_fun )
    mulconfnt_source_set_string_ptr( osrc, osrc->load_string_fun( osrc ) );
  return osrc ? osrc->string : NULL;
}

mas_argvc_t *
mulconfnt_source_load_targ( config_source_desc_t * osrc )
{
  if ( osrc )
  {
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
  return osrc ? &osrc->targ : NULL;
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

static config_variant_t
max_match_id( config_source_desc_t * osrc, const char *arg )
{
  int maxmatch = -1;
  config_variant_t maxmatchid = MULCONF_VARIANT_MAX;

  for ( unsigned i = 0; osrc && ( i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ) ); i++ )
  {
    int len = match_arg( osrc->pref_ids[i].string, arg );

    if ( do_fprintf > 0 )
      fprintf( stderr, "PREF '%s' *** '%s' ===> %d\n", osrc->pref_ids[i].string, arg, len );

    if ( len > maxmatch )
    {
      maxmatch = len;
      maxmatchid = osrc->pref_ids[i].id;
      if ( do_fprintf > 0 )
        fprintf( stderr, "PREF maxmatch:%d; maxmatchid:%d\n", maxmatch, maxmatchid );
    }
  }
  return maxmatchid;
}

char **
mulconfnt_source_argv_no( config_source_desc_t * osrc )
{
  return osrc ? osrc->targno.argv : NULL;
}

int
mulconfnt_source_argc_no( config_source_desc_t * osrc )
{
  return osrc ? osrc->targno.argc : 0;
}

const char *
mulconfnt_source_arg_no( config_source_desc_t * osrc, int i )
{
  return osrc && i >= 0 && i < osrc->targno.argc ? osrc->targno.argv[i] : NULL;
}

int
mulconfnt_source_lookup_seq( config_source_desc_t * osrc, const config_option_table_list_t * tablist )
{
  int nargs = 0;

  mulconfnt_source_load_targ( osrc );
  osrc->lastoptpos = 0;
  for ( int iarg = 0; osrc && !mulconfnt_error_source( osrc ) && iarg < osrc->targ.argc; iarg++ )
  {
    static const char *labels[MULCONF_VARIANTS] = { "SHORT", "LONG", "NONOPT", "BAD" };
    const char *arg = osrc->targ.argv[iarg];
    const char *next_arg = NULL;

    nargs++;
    if ( iarg < osrc->targ.argc - 1 )
      next_arg = osrc->targ.argv[iarg + 1];

    if ( do_fprintf )
      fprintf( stderr, "LOOKUP %s\n", arg );

    config_variant_t variantid = ( !osrc->lastoptpos || iarg <= osrc->lastoptpos ) ? max_match_id( osrc, arg ) : MULCONF_VARIANT_NONOPT;
    int preflen = osrc->pref_ids[variantid].string ? strlen( osrc->pref_ids[variantid].string ) : 0;

    if ( do_fprintf )
      fprintf( stderr, "LAST:%d. '%s' --- %d\n", iarg, arg, osrc->lastoptpos );
    if ( variantid == MULCONF_VARIANT_BAD )
    {
      if ( do_fprintf )
        fprintf( stderr, "NO VARIANT [%s] arg='%s';\n", labels[variantid], arg );
    }
    else if ( variantid == MULCONF_VARIANT_NONOPT )
    {
      if ( do_fprintf )
        fprintf( stderr, "ADD NONOPT %s\n", arg + preflen );
      mas_add_argvc_arg( &osrc->targno, arg + preflen );
    }
    else
    {
      if ( do_fprintf )
        fprintf( stderr, "VARIANT %s\n", labels[variantid] );
      config_option_t *opt = NULL;

      opt = mulconfnt_config_option_lookup_tablist( tablist, variantid, arg + preflen, next_arg, osrc->eq, NULL, osrc->flags );
      if ( do_fprintf )
        fprintf( stderr, "OPT: %p (%s)\n", opt, arg );
      while ( opt && opt->restype == MULCONF_RESTYPE_ALIAS && opt->ptr )
      {
        config_option_t *oldopt = opt;

//        oldopt->source = osrc;
        mulconfnt_option_set_source( oldopt, osrc );                 /* mostly for error setting */
        if ( do_fprintf )
          fprintf( stderr, "ALIAS VAL: %s\n", oldopt->string_value );

        opt = mulconfnt_config_option_lookup_tablist( tablist, variantid, ( char * ) oldopt->ptr, next_arg, osrc->eq, oldopt->string_value,
                                                      osrc->flags );
        if ( do_fprintf )
          fprintf( stderr, "ALIAS (%s) => %s / %s\n", arg + preflen, opt->string_value, opt ? opt->name : "?" );

        mulconfnt_config_option_delete( oldopt );
      }
      if ( opt )
      {
        mulconfnt_option_set_source( opt, osrc );                    /* mostly for error setting */
        if ( mulconfnt_error_option( opt ) )
        {
//          opt->source = osrc;
          mulconfnt_error_set_at_source_from_option( opt->source, opt );
        }
        if ( opt->restype & MULCONF_BITWISE_LASTOPT )
        {
          if ( do_fprintf )
            fprintf( stderr, "SET LAST: %d. '%s'; has_value:%d\n", iarg, arg, opt->has_value );
          osrc->lastoptpos = iarg;
        }
        if ( opt->has_value > 0 )
        {
          iarg += opt->has_value - 1;
        }
/* TODO actions here !! */
      }
      else
      {
        mulconfnt_error_set_at_source( osrc, __LINE__, __func__, __FILE__, "unrecognized option '%s'", arg );
      }
      if ( do_fprintf )
        fprintf( stderr, "*** LOOKUP [%s] arg='%s'; name='%s'; value='%s'\n", labels[variantid], arg, opt ? opt->name : "<NONE>",
                 opt ? opt->string_value : "<NONE>" );
      if ( opt )
      {
        mulconfnt_config_option_delete( opt );
      }
      opt = NULL;
    }
  }
  return -nargs;
}

void
mulconfnt_source_lookup_all( config_source_desc_t * osrc, const config_option_table_list_t * tablist )
{
  while ( osrc && mulconfnt_source_lookup_seq( osrc, tablist ) < 0 )
    osrc->npos++;
}

int
mulconfnt_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist )
{
  int r = 0;

  if ( osrc )
  {
    osrc->npos = 0;
    r = mulconfnt_source_lookup_seq( osrc, tablist );
  }
  return r;
}

unsigned long
mulconfnt_source_flags( config_source_desc_t * osrc )
{
  return osrc ? osrc->flags : 0L;
}

int
mulconfnt_source_flag( config_source_desc_t * osrc, unsigned long mask )
{
  return osrc && ( osrc->flags & mask ? 1 : 0 );
}
