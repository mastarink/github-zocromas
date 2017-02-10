#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "option_base.h"
#include "option.h"
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
    mas_argvc_delete( &osrc->targ );
    if ( osrc && osrc->load_targ_fun )
      osrc->targ = osrc->load_targ_fun( osrc, osrc->targ );
  }
  return osrc ? &osrc->targ : NULL;
}

static int
match_arg( const char *pref, const char *arg )
{
  unsigned i = 0;

  if ( !pref || !*pref )
    return 0;
  for ( i = 0; i < strlen( pref ); i++ )
    if ( !arg[i] || pref[i] != arg[i] )
      break;
  return i > 0 ? ( int ) i : -1;
}

static config_variant_t
max_match_id( config_source_desc_t * osrc, const char *arg )
{
  int maxmatch = -1;
  config_variant_t maxmatchid = MULCONF_VARIANT_MAX;

  for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
  {
//  if ( osrc->pref_ids[i].string )
    {
      int len = match_arg( osrc->pref_ids[i].string, arg );

      if ( do_fprintf > 0 )
        fprintf( stderr, "PREF '%s' *** '%s' ===> %d\n", osrc->pref_ids[i].string, arg, len );

      if ( len > maxmatch )
      {
        maxmatch = len;
        maxmatchid = osrc->pref_ids[i].id;
      }
    }
  }
  return maxmatchid;
}

int
mulconfnt_source_argno_count( config_source_desc_t * osrc )
{
  return osrc->targno.argc;
}

char **
mulconfnt_source_argsno( config_source_desc_t * osrc )
{
  return osrc->targno.argv;
}

const char *
mulconfnt_source_argno( config_source_desc_t * osrc, int i )
{
  return osrc && i >= 0 && i < osrc->targno.argc ? osrc->targno.argv[i] : NULL;
}

void
mulconfnt_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist )
{
  mulconfnt_source_load_targ( osrc );
  for ( int iarg = 0; iarg < osrc->targ.argc; iarg++ )
  {
    static const char *labels[MULCONF_VARIANTS] = { "SHORT", "LONG", "NONOPT", "BAD" };
    const char *arg = osrc->targ.argv[iarg];
    const char *next_arg = NULL;

    if ( iarg < osrc->targ.argc - 1 )
      next_arg = osrc->targ.argv[iarg + 1];

    if ( do_fprintf )
      fprintf( stderr, "LOOKUP %s\n", arg );

    config_variant_t variantid = max_match_id( osrc, arg );
    int preflen = osrc->pref_ids[variantid].string ? strlen( osrc->pref_ids[variantid].string ) : 0;

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

      opt = mulconfnt_config_option_lookup_tablist( tablist, variantid, arg + preflen, next_arg, osrc->eq, NULL );
      if ( do_fprintf )
        fprintf( stderr, "OPT: %p (%s)\n", opt, arg );
      while ( opt && opt->restype == MULCONF_RESTYPE_ALIAS && opt->ptr )
      {
        config_option_t *oldopt = opt;

        if ( do_fprintf )
          fprintf( stderr, "ALIAS VAL: %s\n", oldopt->string_value );

        opt = mulconfnt_config_option_lookup_tablist( tablist, variantid, ( char * ) oldopt->ptr, next_arg, osrc->eq, oldopt->string_value );
        if ( do_fprintf )
          fprintf( stderr, "ALIAS (%s) => %s / %s\n", arg + preflen, opt->string_value, opt ? opt->name : "?" );

        mulconfnt_config_option_delete( oldopt );
      }
      if ( opt )
      {
        if ( opt->has_value > 0 )
        {
          iarg += opt->has_value - 1;
        }
/* TODO actions here !! */
      }
      if ( do_fprintf )
        fprintf( stderr, "*** LOOKUP [%s] arg='%s'; name='%s'; value='%s'\n", labels[variantid], arg, opt ? opt->name : "<NONE>",
                 opt ? opt->string_value : "<NONE>" );
      if ( opt )
        mulconfnt_config_option_delete( opt );
      opt = NULL;
    }
  }
}
