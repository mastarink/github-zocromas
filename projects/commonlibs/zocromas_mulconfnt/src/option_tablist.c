#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

#include "option.h"
#include "option_base.h"

#include "option_tablist.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

int
mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, const char *arg_nopref, const char *eq, mucs_optscanner_t * optscan )
{
  rDECLBAD;
  optscan->found_topt = NULL;
/* optscan->at_arg = NULL; */
  if ( tablist )
  {
#if 0
    rSETGOOD;
    while ( rGOOD && !optscan->found_topt && tablist )
    {
      rC( mucs_config_option_lookup_option_table( tablist->options, arg_nopref, eq, optscan ) );
      tablist = tablist->next;
    }
#else
    do
    {
      rC( mucs_config_option_lookup_option_table( tablist->options, arg_nopref, eq, optscan ) );
      tablist = tablist->next;
    } while ( rGOOD && !optscan->found_topt && tablist );
#endif
    if ( !optscan->found_topt )
    {
      rSETBAD;
      if ( optscan->at_arg )
        QRGSRCM( osrc, rCODE, "unrecognized option \"%s\" at \"%s\"", optscan->at_arg, optscan->arg );
      else
        QRGSRCM( osrc, rCODE, "unrecognized option at \"%s\"", optscan->arg );
      optscan->at_arg = NULL;
      if ( mucs_global_flag( MUCS_FLAG_CONTINUE_ON_UNRECOGNIZED ) )
        rSETGOOD;
      WARN( "rCODE:%d", rCODE );
    }
  }
  rRET;
}
