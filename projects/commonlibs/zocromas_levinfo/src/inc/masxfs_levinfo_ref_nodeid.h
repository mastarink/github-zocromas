#ifndef MASXFS_LEVINFO_REF_NODEID_H
# define MASXFS_LEVINFO_REF_NODEID_H

# include "masxfs_levinfo_types.h"

void masxfs_levinfo_set_nodeid( masxfs_levinfo_t * li, unsigned long node_id );

/* unsigned long masxfs_levinfo_node_id( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ); */
unsigned long masxfs_levinfo_nodeid_val( masxfs_levinfo_t * li );
unsigned long masxfs_levinfo_nodeid_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

unsigned long masxfs_levinfo_up_id( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags );

#endif
