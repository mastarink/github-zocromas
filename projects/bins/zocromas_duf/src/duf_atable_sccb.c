#include <string.h>

#include "duf_maintenance.h"

#include "duf_sccbh_shortcuts.h"
#include "std_mod_sets.h"

/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */


const duf_action_table_t *
duf_find_atable_sccb_by_evnamen( const char *name, size_t namelen, const duf_action_table_t * table )
{
  const duf_action_table_t *act = NULL;
  char *n;

  n = mas_strndup( name, namelen );
  for ( act = table; !act->end_of_table; act++ )
  {
    if ( act->sccb && act->in_use && 0 == strcmp( n, act->sccb->name ) )
      break;
  }
  if ( !act || act->end_of_table || !act->sccb || !act->sccb->name )
    act = NULL;
  /* T( "@@@ %s", act && act->sccb ? act->sccb->name : "?" ); */
  mas_free( n );
  return act;
}

const duf_action_table_t *
duf_find_atable_sccb_by_evname( const char *name, const duf_action_table_t * table )
{
  const duf_action_table_t *act;

  act = duf_find_atable_sccb_by_evnamen( name, strlen( name ), table );
  return act;
}
