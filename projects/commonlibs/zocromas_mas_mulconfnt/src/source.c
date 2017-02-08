#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

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
mulconfnt_source_lookup_option_table( config_source_desc_t * osrc __attribute__ ( ( unused ) ), const config_option_t * option_table
                                      __attribute__ ( ( unused ) ), config_variant_t variantid __attribute__ ( ( unused ) ), const char *cutarg
                                      __attribute__ ( ( unused ) ) )
{
  config_option_t *opt = NULL;

  while ( option_table && option_table->name )
  {
    fprintf( stderr, "%s :: %s ? %c\n", cutarg, option_table->name, option_table->shortname );
    option_table++;
  }
  return opt;
}

config_option_t *
mulconfnt_source_lookup_tablist( config_source_desc_t * osrc, const config_option_table_list_t * tablist, config_variant_t variantid,
                                 const char *cutarg )
{
  config_option_t *opt = NULL;

  while ( tablist )
  {
    opt = mulconfnt_source_lookup_option_table( osrc, tablist->options, variantid, cutarg );
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
    static const char *labels[MULCONF_VARIANTS] = { "SHORT", "LONG", "NONOPT" };
    const char *arg = osrc->targ.argv[iarg];

    fprintf( stderr, "LOOKUP %s\n", arg );

    config_variant_t variantid = max_match_id( osrc, arg );

    if ( variantid != MULCONF_VARIANT_BAD )
    {
      fprintf( stderr, "*** LOOKUP %s %s\n", labels[variantid], arg );
      config_option_t *opt __attribute__ ( ( unused ) ) = NULL;

      opt = mulconfnt_source_lookup_tablist( osrc, tablist, variantid, arg + strlen( osrc->pref_ids[variantid].string ) );
    }
  }
}
