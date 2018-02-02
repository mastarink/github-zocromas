#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_format.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_refdef.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_ref_nodeid.h"

void
masxfs_levinfo_set_nodeid( masxfs_levinfo_t * li, unsigned long node_id )
{
  if ( li )
    li->db.node_id = node_id;
}

unsigned long
masxfs_levinfo_nodeid_val( masxfs_levinfo_t * li )
{
  return li ? li->db.node_id : 0;
/* return li ? ( li->lidepth == 0 ? 1 : li->db.node_id ) : 0; */
}

unsigned long
masxfs_levinfo_nodeid_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  unsigned long node_id = 0;

  if ( masxfs_levinfo_stat_ref( li, tflags ) )
    node_id = masxfs_levinfo_nodeid_val( li );
  return node_id;
}

static unsigned long
masxfs_levinfo_nodeid_off( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  unsigned long node_id = 0;

  if ( li )
  {
    masxfs_depth_t lidepth = -offset;

    if ( li->lidepth >= lidepth )
    {
    /* masxfs_levinfo_open( li-offset, tflags ); */
    /* node_id = li[offset].db.node_id; */
      node_id = masxfs_levinfo_nodeid_ref( li + offset, tflags );
    }
  }
  return node_id;
}

unsigned long
masxfs_levinfo_up_id( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  return masxfs_levinfo_nodeid_off( li, -1, tflags );
}
