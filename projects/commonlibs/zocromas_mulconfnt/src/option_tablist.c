#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error.h" */

#include "option.h"
#include "option_base.h"

#include "option_tablist.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

int
mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, const char *arg, const char *eq, mucs_optscanner_t * optscan )
{
  rDECLBAD;
/* const mucs_option_t *found_topt = NULL; */
  optscan->found_topt = NULL;
  if ( tablist )
  {
    rSETGOOD;
    while ( rGOOD && !optscan->found_topt && tablist )
    {
      rC( mucs_config_option_lookup_option_table( tablist->options, arg, eq, optscan ) );
      tablist = tablist->next;
    }
  }
/* return optscan->found_topt; */
  rRET;
}
