#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"
#include "global.h"

#include "source.h"

#include "option_base.h"
#include "option.h"
#include "option_flags.h"
#include "option_tablist.h"

#include "optscanner_base.h"

static int
mucs_optscanner_dealias_opt( mucs_optscanner_t * optscan )
{
  rDECLGOOD;
  optscan->force_value = optscan->string_value;
  while ( rGOOD && optscan->found_topt && optscan->found_topt->s.restype == MUCS_RTYP_ALIAS && optscan->found_topt->s.cust_ptr )
    rC( mucs_config_option_tablist_lookup( optscan->tablist, ( char * ) optscan->found_topt->s.cust_ptr, optscan->source->eq, optscan ) );
  optscan->force_value = NULL;
  rRET;
}

int
mucs_optscanner_lookup_opt( mucs_optscanner_t * optscan, void *userdata )
{
  rDECLBAD;
  rC( mucs_config_option_tablist_lookup( optscan->tablist, optscan->at_arg, optscan->source->eq, optscan ) );
  rC( mucs_optscanner_dealias_opt( optscan ) );
  if ( rGOOD )
  {
    if ( optscan->found_topt )                                       /* TODO : move to mucs_config_option_tablist_lookup */
    {
      if ( !mucs_global_flag( MUCS_FLAG_USE_VPASS ) || ( optscan->found_topt->s.v_pass < 0 || optscan->found_topt->s.v_pass == optscan->pass ) )
      {
        if ( !mucs_config_option_flag( optscan->found_topt, MUCS_FLAG_USE_VPASS )
             || ( optscan->found_topt->s.v_pass < 0 || optscan->found_topt->s.v_pass == optscan->pass ) )
        {
          mucs_option_t *opt = mucs_config_option_clone( optscan->found_topt );

        /* assert( ( optscan->found_topt->s.v_pass < 0 || optscan->found_topt->s.v_pass == optscan->pass ) ); */
          if ( opt )
          {
            if ( mucs_config_option_flag( opt, MUCS_FLAG_USE_VPASS ) )
            {
              WARN( "%s :: %s", opt->s.name, optscan->arg );
            }
            opt->d.source = optscan->source;
            opt->d.npos = optscan->source->curarg;
            opt->d.extra_cb.tablist = optscan->tablist;
            opt->d.extra_cb.source = optscan->source;

            rC( mucs_config_option_evaluate( opt, optscan, userdata ) );

          /* opt->d.extra_cb.source = NULL;  */
          /* opt->d.extra_cb.tablist = NULL; */

/* do something for found option */
            rC( mucs_source_found_opt( optscan->source, opt, userdata ) );
            mucs_config_option_delete( opt );
            opt = NULL;
          }
          else
          {
            rSETBAD;
          }
        }
      }
    }
  }
  rRET;
}

int
mucs_optscanner_lookup( mucs_optscanner_t * optscan, void *userdata )
{
  rDECLBAD;
  if ( optscan->at_arg )
  {
    optscan->at_arg += optscan->preflen;
  /* optscan->source = osrc; */
    if ( optscan->variantid == MUCS_VARIANT_BAD )
    {
#if 0
      static const char *labels[MUCS_VARIANTS] = { "SHORT", "LONG", "NONOPT", "BAD" };
      WARN( "NO VARIANT [%s] arg='%s';\n", labels[variantid], optscan->arg );
#endif
    }
    else if ( optscan->variantid == MUCS_VARIANT_NONOPT )
    {
      mas_add_argvc_arg( &optscan->source->targno, optscan->at_arg );
      rSETGOOD;
    }
/* else */
    {
      if ( optscan->source->curarg < optscan->source->targ.argc - 1 )
        optscan->nextarg = optscan->source->targ.argv[optscan->source->curarg + 1];
      do
      {
        rC( mucs_optscanner_lookup_opt( optscan, userdata ) );
      /* WARN( "(%d) %s", rCODE, optscan && optscan->found_topt ? optscan->found_topt->s.name : "=" ); */
      } while ( optscan->variantid == MUCS_VARIANT_SHORT && optscan->at_arg && !strchr( " \t", *optscan->at_arg ) );
    /* TODO if 
     *       1. unrecognized option
     *       2. do not stop on error flag
     *         then -- add unrecognized option as MUCS_VARIANT_NONOPT (or something else !?)
     * */

    }
  }
  rRET;
}

static int
match_arg( const char *pref, const char *arg )
{
  unsigned i = 0;

  if ( !pref || !*pref || !arg )
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

int
mucs_optscanner_lookup_arg( mucs_optscanner_t * optscan, void *userdata )
{
  rDECLBAD;
  mucs_variant_t vid = max_match_id( optscan->source, optscan->at_arg );

/* max_match_id: determine variant by prefix */

  optscan->variantid = vid;
  optscan->preflen = optscan->source->pref_ids[vid].string ? strlen( optscan->source->pref_ids[vid].string ) : 0;
  rC( mucs_optscanner_lookup( optscan, userdata ) );
  rRET;
}

int
mucs_optscanner_next_word( mucs_optscanner_t * optscan )
{
  rDECLGOOD;
  if ( optscan->at_arg )
    while ( *optscan->at_arg && strchr( " \t", *optscan->at_arg ) )
      optscan->at_arg++;
  rRET;
}
