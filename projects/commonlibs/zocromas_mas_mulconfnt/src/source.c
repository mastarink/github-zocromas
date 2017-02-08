#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "option.h"
#include "source.h"

char *
mulconfnt_source_load_string( config_source_desc_t * osrc )
{
  if ( osrc )
  {
    if ( osrc->string )
      mas_free( osrc->string );
    osrc->string = NULL;
    if ( osrc && osrc->load_string_fun )
      osrc->string = osrc->load_string_fun( osrc );
  }
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

void
mulconfnt_source_close( config_source_desc_t * osrc )
{
  if ( osrc )
  {
    if ( osrc->delims )
      mas_free( osrc->delims );
    if ( osrc->eq )
      mas_free( osrc->eq );
    for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
    {
      if ( osrc->pref_ids[i].string )
        mas_free( osrc->pref_ids[i].string );
      osrc->pref_ids[i].string = NULL;
    }
    if ( osrc->string )
      mas_free( osrc->string );
    osrc->string = NULL;
    mas_argvc_delete( &osrc->targ );
    mas_free( osrc );
  }
}

config_option_t *
mulconfnt_source_lookup_option_table( config_source_desc_t * osrc, const config_option_t * option_table, config_variant_t variantid, const char *arg,
                                      const char *nextarg )
{
  config_option_t *opt = NULL;
  const config_option_t *topt = option_table;

  while ( !opt && topt && topt->name )
  {
    int found = 0;
    int has_value = 0;
    const char *string_value = NULL;

    fprintf( stderr, "variantid: %d\n", variantid );
    if ( variantid == MULCONF_VARIANT_SHORT )
    {
      fprintf( stderr, "variantid: SHORT\n" );
      found = ( strlen( arg ) == 1 && *arg == topt->shortname ) ? 1 : 0;
      if ( found )
      {
        string_value = nextarg;
        has_value = 2;
      }
    }
    else if ( variantid == MULCONF_VARIANT_LONG )
    {
      unsigned l = strlen( topt->name );

      fprintf( stderr, "variantid: LONG %s ? %s -- %08x : %08x\n", arg, topt->name, ( topt->restype), ( topt->restype & ~MULCONF_BITWISE_ALL ) );

      if ( strlen( arg ) >= l && 0 == strncmp( arg, topt->name, l ) )
      {
        if ( ( topt->restype & ~MULCONF_BITWISE_ALL ) != MULCONF_RESTYPE_NONE && arg[l] && ( osrc->eq && arg[l] == osrc->eq[0] ) )
        {
          string_value = &arg[l] + 1;
          found = 2;
          has_value = 1;
          fprintf( stderr, "FOUND %d: string_value='%s'\n", found, string_value );
        }
        else if ( arg[l] )
        {
          found = 0;
        }
        else
        {
          found = 1;
          string_value = nextarg;
          has_value = 2;
          fprintf( stderr, "FOUND %d: string_value='%s'\n", found, string_value );
        }
      }
    }
    if ( found )
    {
      opt = mulconfnt_config_option_clone( topt );
      fprintf( stderr, "NEW OPT %s; has_value=%d\n", opt->name, has_value );
      if ( has_value )
      {
        mulconfnt_config_option_set_value( opt, string_value );
        opt->has_value = has_value;
      }
    }
    if ( opt )
      fprintf( stderr, "%s :: %s ? %c -- %s ='%s'\n", arg, option_table->name, option_table->shortname, found ? "OK" : "-", opt->string_value );
    option_table++;
    topt++;
  }
  return opt;
}

config_option_t *
mulconfnt_source_lookup_tablist( config_source_desc_t * osrc, const config_option_table_list_t * tablist, config_variant_t variantid,
                                 const char *arg, const char *nextarg )
{
  config_option_t *opt = NULL;

  while ( !opt && tablist )
  {
    fprintf( stderr, "LOOKUP TABLIST %s\n", tablist->name );
    opt = mulconfnt_source_lookup_option_table( osrc, tablist->options, variantid, arg, nextarg );
    tablist = tablist->next;
  }
  return opt;
}

static unsigned
match_arg( const char *pref, const char *arg )
{
  unsigned i = 0;

  for ( i = 0; i < strlen( pref ); i++ )
    if ( !arg[i] || pref[i] != arg[i] )
      break;
  return i;
}

static config_variant_t
max_match_id( config_source_desc_t * osrc, const char *arg )
{
  int maxmatch = 0;
  config_variant_t maxmatchid = MULCONF_VARIANT_BAD;

  for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
  {
    if ( osrc->pref_ids[i].string )
    {
      int len = match_arg( osrc->pref_ids[i].string, arg );

      if ( len > maxmatch )
      {
        maxmatch = len;
        maxmatchid = osrc->pref_ids[i].id;
      }
    }
  }
  return maxmatchid;
}

void
mulconfnt_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist )
{
  mulconfnt_source_load_targ( osrc );
  for ( int iarg = 0; iarg < osrc->targ.argc; iarg++ )
  {
    static const char *labels[MULCONF_VARIANT_BAD + 1] = { "SHORT", "LONG", "NONOPT", "NONOPT-A", "NONOPT-B" };
    const char *arg = osrc->targ.argv[iarg];
    const char *next_arg = NULL;

    if ( iarg < osrc->targ.argc - 1 )
      next_arg = osrc->targ.argv[iarg + 1];

    fprintf( stderr, "LOOKUP %s\n", arg );

    config_variant_t variantid = max_match_id( osrc, arg );

    if ( variantid != MULCONF_VARIANT_BAD )
    {
      fprintf( stderr, "VARIANT %s\n", labels[variantid] );
      config_option_t *opt __attribute__ ( ( unused ) ) = NULL;

      opt = mulconfnt_source_lookup_tablist( osrc, tablist, variantid, arg + strlen( osrc->pref_ids[variantid].string ), next_arg );
      fprintf( stderr, "OPT: %p\n", opt );
      if ( opt )
      {
        if ( opt->has_value > 0 )
        {
          iarg += opt->has_value - 1;
        }
/* TODO actions here !! */

        }
      fprintf( stderr, "*** LOOKUP [%s] arg='%s'; name='%s'; value='%s'\n", labels[variantid], arg, opt ? opt->name : "<NONE>",
               opt ? opt->string_value : "<NONE>" );
      if ( opt )
        mulconfnt_config_option_delete( opt );
      opt = NULL;
    }
    else
    {
      fprintf( stderr, "NO VARIANT [%s] arg='%s';\n", labels[variantid], arg );
    }
  }
}
