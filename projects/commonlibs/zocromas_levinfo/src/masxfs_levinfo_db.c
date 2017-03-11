#include "masxfs_levinfo_defs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_db.h"

int
masxfs_levinfo_db_nodeid( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );
  if ( li )
  {
    if ( !li->db.node_id )
    {
      const char *op _uUu_ = "SELECT p.id AS Id FROM filenames AS fn LEFT JOIN parents AS p ON (fn.id=p.dir_id) WHERE id=";

      NIMP( "li:%p name:'%s' lidepth:%ld; mstat:%p; node_id:%ld", li, li ? li->name : NULL, ( long ) ( li ? li->lidepth : 0 ),
            li ? li->db.mstmt : NULL, li ? li->db.node_id : 0 );
      rCODE = -1;
    }
  }
  rRET;
}
