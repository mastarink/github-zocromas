#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/qstd/qstd_types.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_base.h>
/* #include <mastar/qstd/qstd_mstmt_sizes.h>   */
/* #include <mastar/qstd/qstd_mstmt_datas.h>   */
/* #include <mastar/qstd/qstd_mstmt_props.h>   */
/* #include <mastar/qstd/qstd_mstmt_names.h>   */
/* #include <mastar/qstd/qstd_mstmt_parents.h> */
#include <mastar/qstd/qstd_mstmt_nodes.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_db.h"

int
masxfs_levinfo_db_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );
  if ( li )
  {
    if ( li->lidepth )
    {
      WARN( "A li:%p(d:%ld) : id:%ld", li, ( long ) li->lidepth, li->db.node_id );
      rC( masxfs_levinfo_db_open( li - 1, flags ) );
      if ( rGOOD && !li->db.node_id )
        li->db.node_id = mas_qstd_mstmt_selget_node_id( mas_qstd_instance(  ), li[-1].db.node_id, li->name );
      WARN( "(%d) C li:%p(d:%ld) : id:%ld", rCODE, li, ( long ) li->lidepth, li->db.node_id );
    }
    else
    {
      li->db.node_id = 1;
      WARN( "O li:%p(d:%ld) : id:%ld", li, ( long ) li->lidepth, li->db.node_id );
    }
    if ( li->db.node_id )
    {
      li->detype = MASXFS_ENTRY_DIR_NUM;
      rSETGOOD;
    }
  }
  rRET;
}

int
masxfs_levinfo_db_close( masxfs_levinfo_t * li )
{
  rDECL( 0 );
  rC( masxfs_levinfo_fs_close( li ) );
  li->db.node_id = 0;
  rRET;
}

int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  const char *op = "SELECT name FROM allfull WHERE parent_id=?";

  if ( li && li->db.node_id )
  {
    if ( !li->db.mstmt )
    {
      li->db.mstmt = mas_qstd_instance_mstmt_create_setup( 1, 1, op );
      rC( mas_qstd_mstmt_prepare_param_longlong( li->db.mstmt, 0 ) );
      rC( mas_qstd_mstmt_bind_param( li->db.mstmt ) );
      rC( mas_qstd_mstmt_prepare_result_string( li->db.mstmt, 0 ) );
      rC( mas_qstd_mstmt_bind_result( li->db.mstmt ) );

      rC( mas_qstd_mstmt_set_param_longlong( li->db.mstmt, 0, ( long long ) li->db.node_id, FALSE ) );
      WARN( "(%d) SET PARAM %lld", rCODE, ( long long ) li->db.node_id );
      rC( mas_qstd_mstmt_execute_store( li->db.mstmt ) );
      WARN( "(%d) EXEC", rCODE );
    }
  }
  rRET;
}

int
masxfs_levinfo_db_closedir( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  if ( li )
    mas_qstd_mstmt_delete( li->db.mstmt );
  rRET;
}

int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li )
{
  rDECL( 0 );

  rC( masxfs_levinfo_db_opendir( li ) );
  WARN( "(%d) A REWIND", rCODE );
  if ( li )
    rC( mas_qstd_mstmt_data_seek( li->db.mstmt, 0 ) );
  WARN( "(%d) B REWIND", rCODE );
  rRET;
}

int
masxfs_levinfo_db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );
  rC( masxfs_levinfo_db_open( li, flags ) );
  WARN( "li:%p name:'%s' lidepth:%ld; mstat:%p; node_id:%ld", li, li ? li->name : NULL, ( long ) ( li ? li->lidepth : 0 ), li ? li->db.mstmt : NULL,
        li ? li->db.node_id : 0 );
  rRET;
}

masxfs_dirent_t *
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  masxfs_dirent_t *de = NULL;

  if ( li )
  {
    const char *name = NULL;
    int has_data = 0;

    rC( mas_qstd_mstmt_fetch( li->db.mstmt, &has_data ) );
    WARN( "(%d) FETCH", rCODE );
    if ( rGOOD && has_data )
    {
      rC( mas_qstd_mstmt_get_result_string_na( li->db.mstmt, 0, &name ) );
      WARN( "(%d) DATA name: '%s'", rCODE, name );
    }
    else
    {
      DIE( "NO DATA" );
    }

  /* li->db.pde = &li->db.de; (*???*) */
  /* de.d_name=...;                   */
  /* de.d_type=...;                   */
  /* stat=...;                        */
  }
  return de;
}
