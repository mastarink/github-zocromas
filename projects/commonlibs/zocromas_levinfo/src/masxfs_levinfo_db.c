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
  rDECLBAD;
  if ( li )
  {
    if ( li->lidepth )
    {
      rC( masxfs_levinfo_db_open( li - 1, flags ) );
      if ( rGOOD && !li->db.node_id )
        li->db.node_id = mas_qstd_mstmt_selget_node_id( mas_qstd_instance(  ), li[-1].db.node_id, li->name );
      if ( !li->db.node_id )
        rSETBAD;
    }
    else
    {
      li->db.node_id = 1;
      rSETGOOD;
    }
    if ( rGOOD && li->db.node_id )
      li->detype = MASXFS_ENTRY_DIR_NUM;
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_db_close( masxfs_levinfo_t * li )
{
  rDECLBAD;
  rC( masxfs_levinfo_fs_close( li ) );
  li->db.node_id = 0;
  rRET;
}

int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li )
{
  rDECLBAD;
  const char *op = "SELECT name, detype, inode, node_id FROM allfull WHERE parent_id=?";

  if ( li )
  {
    if ( li->db.node_id )
    {
      rSETGOOD;
      if ( !li->db.mstmt )
      {
        li->db.mstmt = mas_qstd_instance_mstmt_create_setup( 1, 4, op );
        rC( mas_qstd_mstmt_prepare_param_longlong( li->db.mstmt, 0 ) ); /* parent_id */
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_bind_param( li->db.mstmt ) );
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_prepare_result_string( li->db.mstmt, 0 ) ); /* name */
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_prepare_result_string( li->db.mstmt, 1 ) ); /* detype */
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_prepare_result_longlong( li->db.mstmt, 2 ) ); /* inode */
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_prepare_result_longlong( li->db.mstmt, 3 ) ); /* node_id */
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_bind_result( li->db.mstmt ) );
        WARN( "DB OPENDIR %d", rCODE );

        rC( mas_qstd_mstmt_set_param_longlong( li->db.mstmt, 0, ( long long ) li->db.node_id, FALSE ) );
        WARN( "DB OPENDIR %d", rCODE );
        rC( mas_qstd_mstmt_execute_store( li->db.mstmt ) );
        WARN( "DB OPENDIR %d", rCODE );
      }
      WARN( "DB OPENDIR %d", rCODE );
    }
    WARN( "DB OPENDIR %d  '%s' @ %d node_id:%lld", rCODE, li->name, li->lidepth , li->db.node_id);
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_db_closedir( masxfs_levinfo_t * li )
{
  rDECLBAD;
  if ( li )
  {
    rSETGOOD;
    mas_qstd_mstmt_delete( li->db.mstmt );
  }
  rRET;
}

int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  rC( masxfs_levinfo_db_opendir( li ) );
  WARN( "DB REWIND" );
  if ( li )
    rC( mas_qstd_mstmt_data_seek( li->db.mstmt, 0 ) );
  rRET;
}

int
masxfs_levinfo_db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  rC( masxfs_levinfo_db_open( li, flags ) );
  WARN( "li:%p name:'%s' lidepth:%ld; mstat:%p; node_id:%lld", li, li ? li->name : NULL, ( long ) ( li ? li->lidepth : 0 ), li ? li->db.mstmt : NULL,
        li ? li->db.node_id : 0 );
  rRET;
}

int
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, int *phas_data )
{
  rDECLBAD;
/* masxfs_dirent_t *de = NULL; */

  if ( li )
  {
    const char *name = NULL;
    const char *sdetype = NULL;
    int has_data = 0;
    unsigned long long inode = 0;
    unsigned long long node_id = 0;

    rC( mas_qstd_mstmt_fetch( li->db.mstmt, &has_data ) );
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;

      rC( mas_qstd_mstmt_get_result_string_na( li->db.mstmt, 0, &name ) );
      rC( mas_qstd_mstmt_get_result_string_na( li->db.mstmt, 1, &sdetype ) );
      rC( mas_qstd_mstmt_get_result_longlong( li->db.mstmt, 2, &inode, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( li->db.mstmt, 3, &node_id, &is_null ) );
      li->db.detype = masxfs_levinfo_s2detype( sdetype );
      li->db.deinode = inode;
      li->db.denode_id = node_id;
      WARN( "(%d) DATA name: '%s' sdetype:'%s'; inode:%lld", rCODE, name, sdetype, inode );
    }
    else
    {
      WARN( "NO DATA" );
    }
    if ( phas_data )
      *phas_data = has_data;

  /* li->db.pde = &li->db.de; (*???*) */
  /* de.d_name=...;                   */
  /* de.d_type=...;                   */
  /* stat=...;                        */
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_db_open_at( masxfs_levinfo_t * li _uUu_, int fdparent _uUu_ )
{
  rDECLBAD;
  NIMP( "li:%p", li );
  rSETBAD;
  rRET;
}
