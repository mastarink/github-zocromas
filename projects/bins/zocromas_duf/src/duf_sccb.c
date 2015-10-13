#include <string.h>

#include "duf_maintenance.h"

#include "duf_sccbh_shortcuts.h"
#include "std_mod_sets.h"

/* ###################################################################### */
#include "duf_sccb.h"
/* ###################################################################### */

/* TODO rename _duf_uni_scan_action_title => _duf_sccb_title */
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

/* TODO rename duf_uni_scan_action_title => duf_sccb_title */
const char *
duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb )
{
  static char tbuf[1024];

  snprintf( tbuf, sizeof( tbuf ), "◁ %s ▷", _duf_uni_scan_action_title( sccb ) );
  return tbuf;
}

const duf_action_table_t *
duf_find_sccb_by_evnamen( const char *name, size_t namelen, const duf_action_table_t * table )
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
duf_find_sccb_by_evname( const char *name, const duf_action_table_t * table )
{
  const duf_action_table_t *act;

  act = duf_find_sccb_by_evnamen( name, strlen( name ), table );
  return act;
}

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
  unsigned index;

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
  unsigned index;

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
duf_sccb_get_sql_set( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type )
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
