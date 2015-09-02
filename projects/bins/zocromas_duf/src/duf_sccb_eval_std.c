#define DUF_SQL_PDI_STMT

#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */


#include "duf_maintenance.h"


#include "duf_prepare_actions.h"
#include "duf_sccb_eval.h"
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval_std.h"
/* ###################################################################### */


int
duf_evaluate_sccb_named_list_std( const char *names )
{
  return duf_evaluate_sccb_named_list( names, duf_action_table(  ) );
}

int
duf_evaluate_sccb_namen_std( const char *name, size_t len )
{
  return duf_evaluate_sccb_namen( name, len, duf_action_table(  ) );
}

int
duf_evaluate_sccb_name_std( const char *name )
{
  return duf_evaluate_sccb_name( name, duf_action_table(  ) );
}

int
duf_evaluate_pdi_sccb_namen_std( const char *name, size_t len, duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu )
{
  return duf_evaluate_pdi_sccb_namen( name, len, duf_action_table(  ), pdi, ptarg, pu );
}

int
duf_evaluate_pdi_sccb_name_std( const char *name, duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu )
{
  return duf_evaluate_pdi_sccb_name( name, duf_action_table(  ), pdi, ptarg, pu );
}

int
duf_evaluate_pdi_sccb_name_std_at( const char *name, duf_depthinfo_t * pdi, const char *arg, duf_ufilter_t * pu )
{
  return duf_evaluate_pdi_sccb_name_at( name, duf_action_table(  ), pdi, arg, pu );
}

int
duf_evaluate_pdi_sccb_std( const char *name, duf_depthinfo_t * pdi, duf_ufilter_t * pu )
{
  return duf_evaluate_pdi_sccb_name_at( name, duf_action_table(  ), pdi, NULL, pu );
}
