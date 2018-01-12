#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "optscanner_base.h"

void
mucs_optscanner_init( mucs_optscanner_t * optscan, mucs_source_t * osrc, const mucs_option_table_list_t * tablist )
{
  if ( optscan )
  {
    memset( optscan, 0, sizeof( mucs_optscanner_t ) );
    optscan->tablist = tablist;
    optscan->source = osrc;
    if ( osrc )
    {
      optscan->arg = osrc->targ.argv[osrc->curarg];
      optscan->pass = osrc->pass;
    /* optscan->found_max = osrc->found_max; */
    }
    optscan->at_arg = optscan->arg;
  /* if ( optscan->found_max )                                                             */
  /*   optscan->found_topts = mas_calloc( optscan->found_max, sizeof( mucs_option_t * ) ); */
  }
}

void
mucs_optscanner_reset( mucs_optscanner_t * optscan _uUu_ )
{
/* if ( optscan->found_topts )         */
/*   mas_free( optscan->found_topts ); */
/* optscan->found_topts = NULL;        */
}
