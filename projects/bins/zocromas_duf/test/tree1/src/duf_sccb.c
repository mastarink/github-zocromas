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

static const duf_sql_set_t *
duf_get_leaf_sql_set( const duf_scan_callbacks_t * sccb )
{
  const duf_sql_set_t *set = NULL;

  assert( sccb );

#if 0

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
#else
  int index;

  index = sccb->use_std_leaf;
  if ( index > 0 )
    set = ( index <= std_leaf_nsets ) ? &std_leaf_sets[index - 1] : NULL;
  else
    set = &sccb->leaf;
  /* DUF_TRACE( temp, 0, "%d : %ld : %s", index, std_leaf_nsets, set->fieldset ); */
#endif
  return set;
}

static const duf_sql_set_t *
duf_get_node_sql_set( const duf_scan_callbacks_t * sccb )
{
  const duf_sql_set_t *set = NULL;

  assert( sccb );

#if 0
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
#else
  int index;

  index = sccb->use_std_node;
  if ( index > 0 )
    set = ( index <= std_node_nsets ) ? &std_node_sets[index - 1] : NULL;
  else
    set = &sccb->node;
  /* DUF_TRACE( temp, 0, "%d : %ld : %s", index, std_node_nsets, set->fieldset ); */
#endif
  return set;
}

const duf_sql_set_t *
duf_get_sql_set( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type )
{
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
    break;
  }
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
