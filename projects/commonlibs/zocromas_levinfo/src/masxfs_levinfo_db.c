#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

/* #include <mastar/qstd/qstd_defs.h> */
/* #include <mastar/qstd/qstd_enums.h> */
/* #include <mastar/qstd/qstd_mstmt.h> */
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt_nodes.h>

#include <mastar/qstd/qstd_mstmt_sizes.h>
#include <mastar/qstd/qstd_mstmt_datas.h>
#include <mastar/qstd/qstd_mstmt_props.h>
#include <mastar/qstd/qstd_mstmt_sha1.h>
#include <mastar/qstd/qstd_mstmt_names.h>
#include <mastar/qstd/qstd_mstmt_parents.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"
/* #include "masxfs_levinfo_base.h" */
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_fs.h"
#include "masxfs_levinfo_digest.h"

#include "masxfs_levinfo_db_dir.h"
#include "masxfs_levinfo_db.h"

int
masxfs_levinfo_db_open_at( masxfs_levinfo_t * li _uUu_, int fdparent _uUu_ )
{
  rDECLBAD;
  NIMP( "li:%p", li );
  rSETBAD;
  rRET;
}

int
masxfs_levinfo_db_open( masxfs_levinfo_t * li )
{
  rDECLBAD;
  if ( li )
  {
    rSETGOOD;
    if ( !li->db.node_id )
    {
      masxfs_entry_type_t detype_tmp = li->detype;

      if ( li->lidepth )
      {
        li[-1].detype = MASXFS_ENTRY_DIR_NUM;

        rC( masxfs_levinfo_db_open( li - 1 ) );
//      assert( li[-1].db.node_id );
        assert( !detype_tmp || detype_tmp == MASXFS_ENTRY_UNKNOWN_NUM || detype_tmp == li->detype );
      /* TODO
       * The only unknown type is for starting point, for instance, 'mastest' at:
       *   /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest
       *
       * Why???
       *
       * */
        if ( rGOOD && ( li->detype == MASXFS_ENTRY_DIR_NUM || li->detype == MASXFS_ENTRY_UNKNOWN_NUM ) )
        {
          if ( !li->db.node_id )
            li->db.node_id = mas_qstd_mstmt_selget_node_id( mas_qstd_instance(  ), li[-1].db.node_id, li->name );
#if 0
          if ( !li->db.node_id && ( flags & MASXFS_CB_CAN_UPDATE_DB ) )
          {
            li->db.node_id = masxfs_levinfo_db_store( li, flags );
            WARN( "O li->db.node_id: %lld", li->db.node_id );
          }
#endif
          if ( !li->db.node_id )
          {
            rSETBAD;
            QRLIM( li, rCODE, "can't get node_id for '%s' (D:%d)", li->name, li->lidepth );
          }
        }
      }
      else
      {
        li->db.node_id = 1;
        rSETGOOD;
      }
      assert( !detype_tmp || detype_tmp == MASXFS_ENTRY_UNKNOWN_NUM || detype_tmp == li->detype );
      if ( rGOOD && li->db.node_id )
        li->detype = MASXFS_ENTRY_DIR_NUM;
      assert( !detype_tmp || detype_tmp == MASXFS_ENTRY_UNKNOWN_NUM || detype_tmp == li->detype );
    }
  }
  else
    QRLI( li, rCODE );
/* assert( li->detype != MASXFS_ENTRY_DIR_NUM || li->db.node_id ); */
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
masxfs_levinfo__db_stat( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li->lidepth )
  {
    rC( masxfs_levinfo_db_opendir( li - 1 ) );
    if ( !li->db.stat )
    {
      int has_data = 0;                                              /* or just pass NULL instead of &has_data */

      rC( masxfs_levinfo_db_readdir( li - 1, NULL /* filter */, &has_data ) );
    }
  }
  rRET;
}

int
masxfs_levinfo_db_stat( masxfs_levinfo_t * li )
{
  rDECLBAD;

  rC( masxfs_levinfo__db_stat( li ) );
#if 0
  if ( !li->db.stat )
  {
  /* if ( !li->db.node_id ) */
    {
      if ( ( flags & MASXFS_CB_CAN_UPDATE_DB ) )
      {
        WARN( "li->name: %s", li->name );
        li->db.node_id = masxfs_levinfo_db_store( li, flags );
        WARN( "S li->db.node_id: %lld", li->db.node_id );

      /* FIXME  following NOT WORKING */
        rC( masxfs_levinfo_db_rewinddir( li, flags ) );
        rC( masxfs_levinfo__db_stat( li, flags ) );
        WARN( "SIZE:%ld", masxfs_levinfo_size_ref( li, flags ) );
      }
      else
      {
        rSETBAD;
        QRLIM( li, rCODE, "can't dbstat '%s' D%d", li->name, li->lidepth );
      }
    }
  }
#endif
  rRET;
}

unsigned long long
masxfs_levinfo_db_store( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  masxfs_levinfo_flags_t take_flags = ( flags | MASXFS_CB_MODE_FS ) & ~MASXFS_CB_MODE_DB;

  unsigned long long parent_id = masxfs_levinfo_parent_id( li );

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  unsigned long long node_id = 0;
  unsigned long long data_id = 0;
  const char *sdetype = masxfs_levinfo_detype2s( detype );

  mas_qstd_t *qstd = mas_qstd_instance(  );

  {
    size_t size = masxfs_levinfo_size_ref( li, take_flags );

    size_t thesize _uUu_ = mas_qstd_mstmt_selinsget_sizes_id( qstd, size );

  }
  {
    const masxfs_stat_t *stat = masxfs_levinfo_stat_ref( li, take_flags );
    const masxfs_digests_t *digests = li->digests;                   /* TODO masxfs_levinfo_digests_ref */

    if ( stat || digests )
      data_id = mas_qstd_mstmt_selinsget_datas_id( qstd, stat );
    if ( data_id )
    {
      if ( stat )
      {
        unsigned long long props_id _uUu_ = mas_qstd_mstmt_selinsget_props_id( qstd, data_id, sdetype, stat );
      }
      else
      {
        WARN( "No stat" );
      }
      if ( detype == MASXFS_ENTRY_REG_NUM )
      {
        if ( digests )
        {
          const unsigned char *sha1 = NULL;

        /* TODO mode: calc. digest only if there is no digest by this data_id */
          masxfs_digests_get( digests, MASXFS_DIGEST_SHA1, &sha1 );
          {
            unsigned long long sha1_id = mas_qstd_mstmt_selinsget_sha1_id( qstd, data_id, sha1 );

            mas_qstd_mstmt_selinsget_sha1dref( qstd, data_id, sha1_id );
          }
        }
        else
        {
          WARN( "No digests for %s (%d)", li->name, detype );
        }
      }
      {
        unsigned long long name_id = 0;
        masxfs_depth_t lidepth = masxfs_levinfo_depth_ref( li, take_flags );

        if ( lidepth != 0 )
        {
          assert( parent_id );
          assert( data_id );
          name_id = mas_qstd_mstmt_selinsget_names_id( qstd, ename, parent_id, data_id, sdetype );
        }
        if ( detype == MASXFS_ENTRY_DIR_NUM )
        {
          node_id = mas_qstd_mstmt_selinsget_parents_id( qstd, name_id );
          masxfs_levinfo_set_node_id( li, node_id );
        }
      }
    }
  }

  return node_id;
}
