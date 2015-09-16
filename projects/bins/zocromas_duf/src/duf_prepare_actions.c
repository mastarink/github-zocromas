#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_names.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_action_table.h"
#include "duf_prepare_actions.h"
/* ###################################################################### */




/*
 * "convert" «duf_config» representation of tasks/actions into «sccb» list (duf_scan_callbacks_t),
 *         return list length (# of items)
 * ********************************************************************************************
 *  psccbs (duf_scan_callbacks_t **) buffer must be inited for enough number of sccb (duf_scan_callbacks_t) pointers
 * */
int
duf_config2sccb_vector( duf_scan_callbacks_t ** psccbs, int max_asteps )
{
  int asteps = 0;

  if ( psccbs )
  {
    psccbs[asteps] = NULL;
    for ( duf_action_table_t * act = duf_action_table(  ); act->sccb && asteps < max_asteps; act++ )
    {
      duf_scan_callbacks_t *sccb = act->sccb;

      if ( act->tovector && ( DUF_CONFIGG( cli.act.v.bit ) & act->on.bit ) == act->on.bit && ( DUF_CONFIGG( cli.act.v.bit ) & act->off.bit ) == 0 )
      {
        if ( sccb )
        {
          psccbs[asteps++] = sccb;
          psccbs[asteps] = NULL;
          DUF_TRACE( action, 0, "action prepared: %s", duf_uni_scan_action_title( sccb ) );
        }
        else
        {
          DUF_TRACE( action, 0, "action not prepared (no sccb): %s : %x on:%x off:%x", duf_uni_scan_action_title( sccb ),
                     DUF_CONFIGG( cli.act.v.bit ), act->on.bit, act->off.bit );
        }
      }
      else
      {
        DUF_TRACE( action, 0, "action not prepared: %s : %x on:%x off:%x", duf_uni_scan_action_title( sccb ),
                   DUF_CONFIGG( cli.act.v.bit ), act->on.bit, act->off.bit );
      }
    }
  }
  return asteps;
}
