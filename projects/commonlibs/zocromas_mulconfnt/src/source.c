#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error.h" */

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
mucs_source_set_string_ptr( mucs_source_han_t * osrc, char *string )
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
mucs_source_load_string( mucs_source_han_t * osrc )
{
  if ( osrc && osrc->load_string_fun )
    mucs_source_set_string_ptr( osrc, osrc->load_string_fun( osrc ) );
  return osrc ? osrc->string : NULL;
}

mas_argvc_t *
mucs_source_load_targ( mucs_source_han_t * osrc )
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

/* determine variant by prefix */
static mucs_variant_t
max_match_id( mucs_source_han_t * osrc, const char *arg )
{
  int maxmatch = -1;
  mucs_variant_t maxmatchid = MUCS_VARIANT_MAX;

  for ( unsigned i = 0; osrc && ( i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ) ); i++ )
  {
    int len = match_arg( osrc->pref_ids[i].string, arg );

    if ( len > maxmatch )
    {
      maxmatch = len;
      maxmatchid = osrc->pref_ids[i].id;
    }
  }
  return maxmatchid;
}

char **
mucs_source_argv_no( mucs_source_han_t * osrc )
{
  return osrc ? osrc->targno.argv : NULL;
}

int
mucs_source_argc_no( mucs_source_han_t * osrc )
{
  return osrc ? osrc->targno.argc : 0;
}

const char *
mucs_source_arg_no( mucs_source_han_t * osrc, int i )
{
  return osrc && i >= 0 && i < osrc->targno.argc ? osrc->targno.argv[i] : NULL;
}

mucs_option_han_t *
mucs_source_dealias_opt( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist, mucs_option_han_t * opt, mucs_variant_t variantid,
                         const char *next_arg )
{
  while ( opt && opt->restype == MUCS_RTYP_ALIAS && opt->argptr )
  {
    mucs_option_han_t *oldopt = opt;

//        oldopt->source = osrc;
    mucs_option_set_source( oldopt, osrc );                          /* mostly for error setting */

    opt = mucs_config_option_tablist_lookup( tablist, variantid, ( char * ) oldopt->argptr, next_arg, osrc->eq, oldopt->string_value, osrc->flags );

    mucs_config_option_delete( oldopt );
  }
  return opt;
}

void
mucs_source_found_opt( mucs_source_han_t * osrc, mucs_option_han_t * opt )
{
  mucs_option_set_source( opt, osrc );                               /* mostly for error setting */
#if 0
  if ( mucs_error_option( opt ) )
  {
    mucs_error_set_at_source_from_option( opt->source, opt );
  }
#endif
  if (  /* ( opt->restype & MUCS_RTYP_FLAG_LASTOPT ) || */ ( opt->flags & MUCS_FLAG_LASTOPT ) )
  {
    osrc->lastoptpos = osrc->curarg;
  }
  {
    mucs_option_callback_t cb = NULL;

    if ( !( ( opt->flags & MUCS_FLAG_NO_COMMON_CB_IF_VALUE ) && opt->value_is_set ) && osrc->common_callback && !( opt->flags & MUCS_FLAG_NO_COMMON_CB ) )
    {
      cb = osrc->common_callback;
      if ( cb )
      {
        cb( opt );
        osrc->callback_called++;
      }
    }
    if ( !( ( opt->flags & MUCS_FLAG_NO_TYPE_CB_IF_VALUE ) && opt->value_is_set ) &&  osrc->type_callbacks && !( opt->flags & MUCS_FLAG_NO_TYPE_CB ) )
    {
      cb = osrc->type_callbacks[opt->restype & ~MUCS_RTYP_FLAG_ALL];
      if ( cb )
      {
        cb( opt );
        osrc->callback_called++;
      }
    }
  }
  if ( opt->has_value > 0 )
    osrc->curarg += opt->has_value - 1;
/* TODO additional actions here !! */
}

void
mucs_source_lookup_opt( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist, mucs_variant_t variantid, const char *arg_nopref )
{
  mucs_option_han_t *opt = NULL;
  const char *next_arg = NULL;

  if ( osrc->curarg < osrc->targ.argc - 1 )
    next_arg = osrc->targ.argv[osrc->curarg + 1];
  opt = mucs_config_option_tablist_lookup( tablist, variantid, arg_nopref, next_arg, osrc->eq, NULL, osrc->flags );
  opt = mucs_source_dealias_opt( osrc, tablist, opt, variantid, next_arg );

/* do something for found option */
  if ( opt )
  {
    mucs_source_found_opt( osrc, opt );
  }
  else
  {
  /* mucs_error_set_at_source( osrc, __LINE__, __func__, __FILE__, "unrecognized option '%s'", arg_nopref ); */
  /* WARN( "unrecognized option '%s'", arg_nopref ); */
    QRGSRCM( osrc, -1, "unrecognized option \"--%s\"", arg_nopref );
  }
  if ( opt )
    mucs_config_option_delete( opt );
  opt = NULL;
}

void
mucs_source_lookup_arg( mucs_source_han_t * osrc, const char *arg, const mucs_option_table_list_t * tablist )
{

/* max_match_id: determine variant by prefix */
  mucs_variant_t variantid = ( !osrc->lastoptpos || osrc->curarg <= osrc->lastoptpos ) ? max_match_id( osrc, arg ) : MUCS_VARIANT_NONOPT;
  int preflen = osrc->pref_ids[variantid].string ? strlen( osrc->pref_ids[variantid].string ) : 0;

  if ( variantid == MUCS_VARIANT_BAD )
  {
#if 0
    static const char *labels[MUCS_VARIANTS] = { "SHORT", "LONG", "NONOPT", "BAD" };
    WARN( "NO VARIANT [%s] arg='%s';\n", labels[variantid], arg );
#endif
  }
  else if ( variantid == MUCS_VARIANT_NONOPT )
  {
    mas_add_argvc_arg( &osrc->targno, arg + preflen );
  }
  else
  {
    mucs_source_lookup_opt( osrc, tablist, variantid, arg + preflen );
  }
}

int
mucs_source_lookup_seq( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist )
{
  int nargs = 0;

  mucs_source_load_targ( osrc );
  osrc->lastoptpos = 0;

  for ( osrc->curarg = 0; osrc && !masregerrs_count_all_default( NULL, TRUE ) /*mucs_error_source( osrc ) */  && osrc->curarg < osrc->targ.argc;
        osrc->curarg++ )
  {
    nargs++;
    mucs_source_lookup_arg( osrc, osrc->targ.argv[osrc->curarg], tablist );
  /* WARN( "* (%d) %d. %s ne:%d", nargs, osrc->curarg, osrc->targ.argv[osrc->curarg], masregerrs_count_all_default( NULL, TRUE ) ); */
  }
  return -nargs;
}

void
mucs_source_lookup_all( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist )
{
  while ( osrc && mucs_source_lookup_seq( osrc, tablist ) < 0 )
    osrc->npos++;
}

int
mucs_source_lookup( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist )
{
  int r = 0;

  if ( osrc )
  {
    osrc->npos = 0;
    r = mucs_source_lookup_seq( osrc, tablist );
  }
  return r;
}

unsigned long
mucs_source_flags( mucs_source_han_t * osrc )
{
  return osrc ? osrc->flags : 0L;
}

int
mucs_source_flag( mucs_source_han_t * osrc, unsigned long mask )
{
  return osrc && ( osrc->flags & mask ? 1 : 0 );
}

void
mucs_source_set_common_callback( mucs_source_han_t * osrc, mucs_option_callback_t cb )
{
  if ( osrc )
    osrc->common_callback = cb;
}

void
mucs_source_set_type_callback( mucs_source_han_t * osrc, mucs_restype_t restype, mucs_option_callback_t cb )
{
  if ( osrc )
    osrc->type_callbacks[restype] = cb;
}
