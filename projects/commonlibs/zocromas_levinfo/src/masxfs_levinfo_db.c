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

#include <mastar/qstd/qstd_types.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_base.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_db.h"

int
masxfs_levinfo_db_closedir( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  if ( li )
  /* mas_mysqlpfs_mstmt_delete( li->db.mstmt ); */
    mas_qstd_mstmt_delete( li->db.mstmt );
  rRET;
}

int
masxfs_levinfo_db_open_mstmt( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  const char *op = "SELECT p.id AS Id FROM filenames AS fn LEFT JOIN parents AS p ON (fn.id=p.dir_id) WHERE p.id=?";

  if ( li->db.node_id )
  {
    li->db.mstmt = mas_qstd_instance_mstmt_create_setup( 1, 1, op );
    rC( mas_qstd_mstmt_prepare_param_longlong( li->db.mstmt, 1 ) );
    rC( mas_qstd_mstmt_set_param_longlong( li->db.mstmt, 0, li->db.node_id, FALSE ) );
  }
  rRET;
}

int
masxfs_levinfo_db_nodeid( masxfs_levinfo_t * li )
{
  rDECL( 0 );
  if ( li )
  {
    if ( !li->db.node_id )
    {
      if ( li->lidepth )
      {
        rC( masxfs_levinfo_db_nodeid( li - 1 ) );
        if ( !rCODE && !li->db.node_id )
        {
          NIMP( "li:%p name:'%s' lidepth:%ld; mstat:%p; node_id:%ld", li, li ? li->name : NULL, ( long ) ( li ? li->lidepth : 0 ),
                li ? li->db.mstmt : NULL, li ? li->db.node_id : 0 );
          rSETBAD;
        }
      }
      else
      {
      /* root: /  --- node_id=1 */
        li->db.node_id = 1;
        masxfs_levinfo_db_open_mstmt( li );
        NIMP( "Is db mstmt open / prepared ?" );
      }
    }
  }
  rRET;
}
