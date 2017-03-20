#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
/* #include <stdio.h> */
#include <string.h>
/* #include <errno.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
/* #include <unistd.h> */

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/qstd/qstd_defs.h>
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
#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_ref.h"
/* #include "masxfs_levinfo_io_dir.h" */
#include "masxfs_levinfo_fs.h"
/* #include "masxfs_levinfo_io.h" */

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
    masxfs_entry_type_t detype_tmp = li->detype;

    if ( li->lidepth )
    {
      li[-1].detype = MASXFS_ENTRY_DIR_NUM;

      rC( masxfs_levinfo_db_open( li - 1, flags ) );
      assert( !detype_tmp || detype_tmp == MASXFS_ENTRY_UNKNOWN_NUM || detype_tmp == li->detype );
    /* TODO
     * The only unknown type is for starting point, for instance, 'mastest' at:
     *   /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest
     *
     * Why???
     *
     * */
    /* WARN( "1 APP TYPE: %d ('%s', %d)", li->detype == MASXFS_ENTRY_DIR_NUM, li->name, li->detype ); */
      if ( rGOOD && ( li->detype == MASXFS_ENTRY_DIR_NUM || li->detype == MASXFS_ENTRY_UNKNOWN_NUM ) )
      {
        if ( !li->db.node_id )
          li->db.node_id = mas_qstd_mstmt_selget_node_id( mas_qstd_instance(  ), li[-1].db.node_id, li->name );
        if ( !li->db.node_id )
          rSETBAD;
      }
    /* WARN( "2 APP TYPE: %d ('%s', %d)", li->detype == MASXFS_ENTRY_DIR_NUM, li->name, li->detype ); */
    /* else        */
    /*   rSETGOOD; */
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
masxfs_levinfo_db_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
/* TODO */
#if 0
  if ( !li->db.stat )
  {
    const char *op _uUu_ = "SELECT name, detype, inode, node_id "    /* */
            ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev FROM " QSTD_VIEW_ALL " WHERE parent_id=? ORDER BY name_id";

    WARN( "li->name:'%s'; li->db.node_id:%lld", li ? li->name : NULL, li && li->lidepth ? li[-1].db.node_id : 0 );
  }
  else
  {
    ADIE( "DB STAT" );
  }
#endif
  rC( masxfs_levinfo_db_open( li, flags ) );

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
    const char *op;

    rSETGOOD;

    if ( name )
      op = "SELECT name, inode, node_id "                            /* */
              ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev FROM " QSTD_VIEW_ALL " WHERE parent_id=? AND name=? ORDER BY name_id";
    else
      op = "SELECT name, inode, node_id "                            /* */
              ", dev, mode, nlink, uid, gid, size, blksize, blocks, rdev FROM " QSTD_VIEW_ALL " WHERE parent_id=? ORDER BY name_id";
    if ( !( *pmstmt ) )
    {
      int numpar = 1 + ( name ? 1 : 0 );
      int numres = 15;
      int nr = 0;
      int np = 0;

      ( *pmstmt ) = mas_qstd_instance_mstmt_create_setup( numpar, numres, op );
      rC( mas_qstd_mstmt_prepare_param_longlong( ( *pmstmt ), np++ ) ); /* parent_id */
      if ( name )
        rC( mas_qstd_mstmt_prepare_param_string( ( *pmstmt ), np++ ) ); /* name */
      assert( np == numpar );

      rC( mas_qstd_mstmt_bind_param( ( *pmstmt ) ) );
      rC( mas_qstd_mstmt_prepare_result_string( ( *pmstmt ), nr++ ) ); /* name */
    /* rC( mas_qstd_mstmt_prepare_result_string( (*pmstmt), nr++ ) ); (* detype *) */
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
      assert( nr == numres );
      rC( mas_qstd_mstmt_bind_result( ( *pmstmt ) ) );

      rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), 0, node_id, FALSE ) );
      if ( name )
        rC( mas_qstd_mstmt_set_param_string( ( *pmstmt ), 0, name ) );
      rC( mas_qstd_mstmt_execute_store( ( *pmstmt ) ) );
    }
  }
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

  /* masxfs_stat_t *stat = NULL; */
    const char *name = NULL;

  /* const char *sdetype = NULL; */
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

  /* if ( pstat )     */
  /*   stat = *pstat; */
    rC( mas_qstd_mstmt_fetch( mstmt, &has_data ) );
    if ( phas_data )
      *phas_data = has_data;
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;
      int numres = 15;
      int nr = 0;

      rC( mas_qstd_mstmt_get_result_string_na( mstmt, nr++, &name ) );
    /* rC( mas_qstd_mstmt_get_result_string_na( mstmt, nr++, &sdetype ) ); (* TODO : needless *) */
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &inode, &is_null ) );
      /* WARN( "A NODE ID: %lld, %d", node_id, is_null ? 1 : 0 ); */
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &node_id, &is_null ) );
      /* WARN( "B NODE ID: %lld, %d", node_id, is_null ? 1 : 0 ); */
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

      if ( rGOOD )
      {
      /* if ( !stat )                                       */
      /*   stat = mas_calloc( 1, sizeof( masxfs_stat_t ) ); */
      /* else                                               */
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
    /* if ( rGOOD && pstat ) */
    /*   *pstat = stat;      */
    /* WARN( "(%d) DATA name: '%s' sdetype:'%s'; inode:%lld", rCODE, name, sdetype, inode ); */
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

  if ( li )
  {
  /* WARN( "'%s' ==> '%s'", li[0].name, li[1].name ); */

  /*
     TODO
   */
#if 0
    if ( li->lidepth && !li[-1].db.scan.mstmt )
      rC( masxfs_levinfo_db_opendir( li - 1, flags ) );
#endif

    if ( li->db.node_id )
    {
      /* WARN( "DB OPENDIR : %lld ; '%s'", ( long long ) li->db.node_id, li[1].name ); */
      rC( masxfs_levinfo_db_prepare_execute_store( &li->db.scan.mstmt, li[1].name, ( long long ) li->db.node_id, flags ) );
    }
    else
    {
      /* WARN( "DB NO OPENDIR" ); */
    }
  /* WARN( "DB OPENDIR %d  '%s' @ %d node_id:%lld", rCODE, li->name, li->lidepth , li->db.node_id); */
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
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags _uUu_, int *phas_data )
{
  rDECLBAD;
/* masxfs_dirent_t *de = NULL; */

  if ( li )
  {
    unsigned long long node_id = 0;
    const char *dename = NULL;
    masxfs_stat_t stat = { 0 };

  /* TODO : directly to li[1]; i.e. init child here (partially??), no need of li->fs.scan.pde, just local pde */
    rC( masxfs_levinfo_db_fetch( li->db.scan.mstmt, &dename, & /*li->db.scan. */ stat, &node_id, flags, phas_data ) );

  /* TODO : directly to li[1]; i.e. init child here (partially??), no need of li->fs.scan.pde, just local pde */

  /* li->db.scan.type = masxfs_levinfo_stat2entry(  (*li->db.scan. *) &stat ); */
    /* li->db.scan.type = masxfs_levinfo_statmode2entry( stat.st_mode ); */

  /* assert( li->db.scan.type == masxfs_levinfo_s2detype( sdetype ) ); */
  /* li->db.scan.inode = inode; */
    li->db.scan.node_id = node_id;

    if ( rGOOD && *phas_data )
    {
#if 1
    /* 20170319.111336 */
      const char *name = NULL;
      masxfs_entry_type_t detype = masxfs_levinfo_stat2entry(  /*li->db.scan. */ &stat );

      rC( mas_qstd_mstmt_get_result_string_na( li->db.scan.mstmt, 0, &name ) );
      /* WARN( "A NID:%lld for '%s'; %lld for '%s' [%lld:'%s':'%s']", li[0].db.node_id, li[0].name, li[1].db.node_id, li[1].name, node_id, dename, name ); */
      assert( !li[1].db.stat );
      masxfs_levinfo_init( li + 1, li->lidepth + 1, name, detype, /*li->db.scan. */ stat.st_ino, node_id, /*li->db.scan. */ NULL );
      /* WARN( "B NID:%lld for '%s'; %lld for '%s' [%lld]", li[0].db.node_id, li[0].name, li[1].db.node_id, li[1].name, node_id ); */
      if ( li[1].db.stat )
        mas_free( li[1].db.stat );
      li[1].db.stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );
      *( li[1].db.stat ) = stat;
      assert( li[1].db.stat );
    /* li->db.scan.stat = NULL; */
#endif
/*
*/

      /* li->db.scan.inode = (*li->db.scan. *) stat.st_ino; */
    }
  /* li->db.pde = &li->db.de; (*???*) */
  /* de.d_name=...;                   */
  /* de.d_type=...;                   */
  /* stat=...;                        */
  }
  else
    QRLI( li, rCODE );
  rRET;
}
