#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_sccbh_shortcuts.h"
#include "std_mod_sets.h"

/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */

const duf_sql_set_t *
duf_get_leaf_sql_set( const duf_scan_callbacks_t * sccb )
{
  const duf_sql_set_t *set = NULL;

  assert( sccb );
  switch ( sccb->use_std_leaf )
  {
  case 0:
    set = &sccb->leaf;
    break;
  case 1:
    set = &std_leaf_set;
    break;
  case 2:
    set = &std_ns_leaf_set;
    break;
  default:
    break;
  }
  return set;
}

const duf_sql_set_t *
duf_get_node_sql_set( const duf_scan_callbacks_t * sccb )
{
  const duf_sql_set_t *set = NULL;

  assert( sccb );

  switch ( sccb->use_std_node )
  {
  case 0:
    set = &sccb->node;
    break;
  case 1:
    set = &std_node_set;
    break;
  case 2:
    set = &std_ns_node_set;
    break;
  default:
    break;
  }
  return set;
}

const duf_sql_set_t *
duf_get_sql_set( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type, int *pr )
{
  int r = 0;
  const duf_sql_set_t *set = NULL;

  assert( sccb );

  switch ( node_type )
  {
  case DUF_NODE_LEAF:
    set = duf_get_leaf_sql_set( sccb );
    break;
  case DUF_NODE_NODE:
    set = duf_get_node_sql_set( sccb );
    break;
  case DUF_NODE_NONE:
    set = NULL;
    break;
  default:
    set = NULL;
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );
    break;
  }
  if ( pr )
    *pr = r;
  return set;
}

const char *
_duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  const char *stitle;

  DEBUG_START(  );
  if ( sccb && sccb->title )
  {
    stitle = strrchr( sccb->title, '/' );
    if ( stitle )
      stitle++;
    else
      stitle = sccb->title;
  }
  else
  {
    stitle = "";
  }
  DEBUG_ENDCS( stitle );
}

const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", _duf_uni_scan_action_title( sccb ) );
  return tbuf;
}
