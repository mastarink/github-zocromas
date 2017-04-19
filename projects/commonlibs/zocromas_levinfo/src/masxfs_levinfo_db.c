#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/qstd/qstd_defs.h>
#include <mastar/qstd/qstd_enums.h>
#include <mastar/qstd/qstd_mstmt.h>
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
#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_fs.h"
#include "masxfs_levinfo_digest.h"

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
masxfs_levinfo_db_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
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

        rC( masxfs_levinfo_db_open( li - 1, flags ) );
        assert( li[-1].db.node_id );
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
masxfs_levinfo__db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  if ( li->lidepth )
  {
    rC( masxfs_levinfo_db_opendir( li - 1, flags ) );
    if ( !li->db.stat )
    {
      int has_data = 0;                                              /* or just pass NULL instead of &has_data */

      rC( masxfs_levinfo_db_readdir( li - 1, flags, &has_data ) );
    }
  }
  rRET;
}

int
masxfs_levinfo_db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  rC( masxfs_levinfo__db_stat( li, flags ) );
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

/* TODO move to qstd */
int
masxfs_levinfo_db_prepare_execute_store( mysqlpfs_mstmt_t ** pmstmt, const char *name, unsigned long long node_id,
                                         masxfs_levinfo_flags_t flags _uUu_ )
{
  rDECLBAD;
  if ( pmstmt )
  {
    rSETGOOD;

    if ( !( *pmstmt ) )
    {
#if 1
      ( *pmstmt ) = mas_qstd_mstmt_init_prepare( mas_qstd_instance(  ), name ? STD_MSTMT_SELECT_EVERYTHING_PN : STD_MSTMT_SELECT_EVERYTHING_P );
#else
      {
        const char *op;

        if ( name )
          op = "SELECT "                                             /* */
                  "  name, inode, node_id "                          /* */
                  ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev " /* */
                  "   FROM " QSTD_VIEW_ALL                           /* */
                  "    WHERE parent_id=? AND name=? "                /* */
                /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                  "   ORDER BY name_id";
        else
          op = "SELECT "                                             /* */
                  "  name, inode, node_id "                          /* */
                  ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev " /* */
                  "   FROM " QSTD_VIEW_ALL                           /* */
                  "    WHERE parent_id=? "                           /* */
                /* "     AND (detype!='REG' OR (nsamesha1>10 AND nsamesize>1)) "              (* *) */
                  "   ORDER BY name_id";
        int numpar = 1 + ( name ? 1 : 0 );
        int numres = 15;                                             /* just fool-proof  */
        int nr = 0;
        int np = 0;

        ( *pmstmt ) = mas_qstd_instance_mstmt_create_setup( numpar, numres, op );
        rC( mas_qstd_mstmt_prepare_param_longlong( ( *pmstmt ), np++ ) ); /* parent_id */
        QRLI( li, rCODE );
        if ( name )
        {
          rC( mas_qstd_mstmt_prepare_param_string( ( *pmstmt ), np++ ) ); /* name */
          QRLI( li, rCODE );
        }
        assert( np == numpar );

        rC( mas_qstd_mstmt_bind_param( ( *pmstmt ) ) );
        rC( mas_qstd_mstmt_prepare_result_string( ( *pmstmt ), nr++ ) ); /* name */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* inode */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* node_id */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* dev */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* mode */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* nlink */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* uid */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* gid */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* size */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* blksize */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* blocks */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* rdev */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* atim */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* mtim */
        rC( mas_qstd_mstmt_prepare_result_longlong( ( *pmstmt ), nr++ ) ); /* ctim */
        QRLI( li, rCODE );
      /* assert( nr == numres ); */
        rC( mas_qstd_mstmt_bind_result( ( *pmstmt ) ) );
        QRLI( li, rCODE );
      }
#endif
      {
        int np = 0;

        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, node_id, FALSE ) );
        QRLI( li, rCODE );
        if ( name )
        {
          rC( mas_qstd_mstmt_set_param_string( ( *pmstmt ), np++, name ) );
          QRLI( li, rCODE );
        }
      /* assert( np == numpar ); */
        rC( mas_qstd_mstmt_execute_store( ( *pmstmt ) ) );
        QRLI( li, rCODE );
      }
    }
  }
  QRLI( li, rCODE );
  rRET;
}

/* TODO move to qstd */
/* allocates stat */
static int
masxfs_levinfo_db_fetch( mysqlpfs_mstmt_t * mstmt, const char **pname, masxfs_stat_t * stat, unsigned long long *pnode_id,
                         masxfs_levinfo_flags_t flags _uUu_, int *phas_data )
{
  rDECLBAD;
  if ( mstmt )
  {
    int has_data = 0;

    const char *name = NULL;

    unsigned long long inode = 0;
    unsigned long long dev = 0;
    unsigned long long rdev = 0;
    unsigned long long node_id = 0;
    unsigned long long mode = 0;
    unsigned long long nlink = 0;
    unsigned long long uid = 0;
    unsigned long long gid = 0;
    unsigned long long size = 0;
    unsigned long long blksize = 0;
    unsigned long long blocks = 0;
    unsigned long long atim_tv_sec = 0;
    unsigned long long mtim_tv_sec = 0;
    unsigned long long ctim_tv_sec = 0;

    rC( mas_qstd_mstmt_fetch( mstmt, &has_data ) );
    if ( phas_data )
      *phas_data = has_data;
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;
      int numres = 15;
      int nr = 0;

      rC( mas_qstd_mstmt_get_result_string_na( mstmt, nr++, &name ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &inode, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &node_id, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &dev, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &mode, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nlink, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &uid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &gid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &size, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &blksize, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &blocks, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &rdev, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &atim_tv_sec, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &mtim_tv_sec, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &ctim_tv_sec, &is_null ) );
      assert( nr == numres );

      if ( rGOOD && stat )
      {
        memset( stat, 0, sizeof( masxfs_stat_t ) );

        stat->st_dev = ( dev_t ) dev;
        stat->st_ino = ( ino_t ) inode;
        stat->st_mode = ( mode_t ) mode;
        stat->st_nlink = ( nlink_t ) nlink;
        stat->st_uid = ( uid_t ) uid;
        stat->st_gid = ( gid_t ) gid;
        stat->st_size = size;
        stat->st_blksize = ( blksize_t ) blksize;
        stat->st_blocks = ( blkcnt_t ) blocks;
        stat->st_rdev = ( dev_t ) rdev;
        stat->st_atim.tv_sec = atim_tv_sec;
        stat->st_mtim.tv_sec = mtim_tv_sec;
        stat->st_ctim.tv_sec = ctim_tv_sec;
      }
      if ( pname )
        *pname = name;
      if ( pnode_id )
        *pnode_id = node_id;
    }
    else
    {
    /* WARN( "NO DATA" ); */
    }
  }
  rRET;
}

int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

/* assert( 0 != strcmp( li->name, ".auxdir" ) ); */
  if ( li )
  {
    if ( li->db.scan.mstmt )
    {
      rSETGOOD;
    }
    else
    {
    /* WARN( "real open 1 '%s' %lld", li->name, li->db.node_id ); */
      rC( masxfs_levinfo_db_open( li, flags ) );
    /* WARN( "(%d) real open 2 '%s' %lld", rCODE, li->name, li->db.node_id ); */
    /* assert( li->detype != MASXFS_ENTRY_DIR_NUM || li->db.node_id ); */
    /* FIXME */
    /* assert( li->db.node_id ); */
    /* WARN("prepare '%s'", li[1].name); */
      rC( masxfs_levinfo_db_prepare_execute_store( &li->db.scan.mstmt, li[1].name, ( long long ) li->db.node_id, flags ) );
    }
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
    mas_qstd_mstmt_delete( li->db.scan.mstmt );
  }
  rRET;
}

int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  rC( masxfs_levinfo_db_opendir( li, flags ) );
  if ( li )
    rC( mas_qstd_mstmt_data_seek( li->db.scan.mstmt, 0 ) );
  rRET;
}

int
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int *phas_data )
{
  rDECLBAD;

  if ( li )
  {
    unsigned long long node_id = 0;
    const char *dename = NULL;
    masxfs_stat_t stat = { 0 };

    rC( masxfs_levinfo_db_fetch( li->db.scan.mstmt, &dename, &stat, &node_id, flags, phas_data ) );
    li->db.scan.node_id = node_id;

    if ( rGOOD )
    {
      if ( *phas_data )
      {
        const char *name = NULL;
        masxfs_entry_type_t detype = masxfs_levinfo_stat2entry( &stat );

        rC( mas_qstd_mstmt_get_result_string_na( li->db.scan.mstmt, 0, &name ) );
        QRLI( li, rCODE );
        assert( !li[1].db.stat );
        masxfs_levinfo_init( li + 1, li->lidepth + 1, name, detype /*, stat.st_ino */ , node_id, &stat );
        assert( li[1].db.stat );
      }
      else if ( dename )
      {
      /* QRLI( li, rCODE ); */
        WARN( "[%p] NO DATA for %s", li->db.scan.mstmt, dename );
      }
      else
      {
      /* WARN( "[%p] NO DATA for %s", li->db.scan.mstmt, dename ); */
      }
    }
  /* TODO #include <fnmatch.h>                                             */
  /* int fnmatch(const char *pattern, const char *string, int flags); */
  }
  else
    QRLI( li, rCODE );
/* WARN( "li '%s' (%p) has:%d", li->name, li->db.stat, *phas_data ); */
  rRET;
}

unsigned long long
masxfs_levinfo_db_store( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  masxfs_levinfo_flags_t take_flags = ( flags | MASXFS_CB_MODE_FS ) & ~MASXFS_CB_MODE_DB;

/* WARN( "ename:%s", ename ); */
  unsigned long long parent_id = masxfs_levinfo_parent_id( li, flags );

/* WARN( "parent_id:%llu", parent_id ); */
  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

/* WARN( "detype:%u", detype ); */
  unsigned long long node_id = 0;
  unsigned long long data_id = 0;
  const char *sdetype = masxfs_levinfo_detype2s( detype );

/* WARN( "sdetype:%s", sdetype ); */
  mas_qstd_t *qstd = mas_qstd_instance(  );

  {
    size_t size = masxfs_levinfo_size_ref( li, take_flags );

  /* WARN( "size:%ld", size ); */
    size_t thesize _uUu_ = mas_qstd_mstmt_selinsget_sizes_id( qstd, size );

  /* WARN( "thesize:%ld", thesize ); */
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
