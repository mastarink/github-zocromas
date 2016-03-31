/* ###################################################################### */
#include "duf_nodetype.h"
/* ###################################################################### */

const char *
duf_nodetype_name( duf_node_type_t nt )
{
  return ( nt == DUF_NODE_LEAF ? "LEAF" : ( nt == DUF_NODE_NODE ? "NODE" : "UNKNOWN" ) );
}
