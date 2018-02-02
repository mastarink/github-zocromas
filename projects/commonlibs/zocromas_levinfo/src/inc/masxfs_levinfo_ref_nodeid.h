#ifndef MASXFS_LEVINFO_REF_NODEID_H
# define MASXFS_LEVINFO_REF_NODEID_H

# include "masxfs_levinfo_types.h"

void masxfs_levinfo_set_node_id( masxfs_levinfo_t * li, unsigned long node_id );

/* unsigned long masxfs_levinfo_node_id( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags ); */
unsigned long masxfs_levinfo_node_id_val( masxfs_levinfo_t * li );
unsigned long masxfs_levinfo_node_id_ref( masxfs_levinfo_t * li );

int masxfs_levinfo_parent_id( masxfs_levinfo_t * li );

#endif
