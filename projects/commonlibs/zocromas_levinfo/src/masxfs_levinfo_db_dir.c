#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>

/* #include <mastar/qstd/qstd_defs.h> */
#include <mastar/qstd/qstd_enums.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_base.h>
/* #include <mastar/qstd/qstd_mstmt_nodes.h> */

/* #include <mastar/qstd/qstd_mstmt_sizes.h> */
/* #include <mastar/qstd/qstd_mstmt_datas.h> */
/* #include <mastar/qstd/qstd_mstmt_props.h> */
/* #include <mastar/qstd/qstd_mstmt_sha1.h> */
/* #include <mastar/qstd/qstd_mstmt_names.h> */
/* #include <mastar/qstd/qstd_mstmt_parents.h> */

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_ref.h"
/* #include "masxfs_levinfo_fs.h" */
/* #include "masxfs_levinfo_digest.h" */

#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_db_dir.h"

/* TODO move to qstd */
static int
masxfs_levinfo_db_prepare_execute_store( mysqlpfs_mstmt_t ** pmstmt, const char *name, unsigned long long node_id )
{
  rDECLBAD;
  if ( pmstmt )
  {
    rSETGOOD;

    if ( !( *pmstmt ) )
    {
#if 0
      ( *pmstmt ) = mas_qstd_mstmt_init_prepare( mas_qstd_instance(  ), name ? STD_MSTMT_SELECT_EVERYTHINGX_PN : STD_MSTMT_SELECT_EVERYTHINGX_P );
#else
      ( *pmstmt ) = mas_qstd_mstmt_init_prepare( mas_qstd_instance(  ), name ? STD_MSTMT_SELECT_EVERYTHINGXX_PN : STD_MSTMT_SELECT_EVERYTHINGXX_P );
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
      /* TODO filter here ! 20171228.144707 */
#if 0
        assert( np == ( name ? STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_PN : STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_P ) );
#else
	/* TODO add atim,ctim,mtim,nlink,parent_id (via parent path),uid,gid,dev */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0, FALSE ) ); /* min_nsamesize */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0x7fffffffLL, FALSE ) ); /* max_nsamesize */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0, FALSE ) ); /* min_nsamesha1 */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0x7fffffffLL, FALSE ) ); /* max_nsamesha1 */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0, FALSE ) ); /* min_size */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0x7fffffffLL, FALSE ) ); /* max_size */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0, FALSE ) ); /* min_inode */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, 0x7fffffffLL, FALSE ) ); /* max_inode */
        rC( mas_qstd_mstmt_set_param_string( ( *pmstmt ), np++, "" /* "^.*\\.(sh|conf)$" */  ) ); /* regexp */
        assert( np == ( name ? STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_PN : STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_P ) );
#endif
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
masxfs_levinfo_db_fetch( mysqlpfs_mstmt_t * mstmt, const char **pname, masxfs_stat_t * stat, masxfs_xstatc_t * xstat, unsigned long long *pnode_id,
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
    unsigned long long nsamesize = 0;
    unsigned long long nsamesha1 = 0;

    const char *hex_sha1 = NULL;

    rC( mas_qstd_mstmt_fetch( mstmt, &has_data ) );
    if ( phas_data )
      *phas_data = has_data;
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;
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
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nsamesize, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nsamesha1, &is_null ) );
      rC( mas_qstd_mstmt_get_result_string_na( mstmt, nr++, &hex_sha1 ) );
      assert( nr == STD_MSTMT_SELECT_EVERYTHINGX_NRESULTS );

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
      if ( rGOOD && xstat )
      {
        xstat->nsamesize = nsamesize;
        xstat->nsamesha1 = nsamesha1;
        xstat->hex_sha1 = hex_sha1;
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
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li )
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
      rC( masxfs_levinfo_db_open( li ) );
    /* WARN( "(%d) real open 2 '%s' %lld", rCODE, li->name, li->db.node_id ); */
    /* assert( li->detype != MASXFS_ENTRY_DIR_NUM || li->db.node_id ); */
    /* FIXME */
    /* assert( li->db.node_id ); */
    /* WARN("prepare '%s'", li[1].name); */
      rC( masxfs_levinfo_db_prepare_execute_store( &li->db.scan.mstmt, li[1].name, ( long long ) li->db.node_id ) );
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
    if ( li->db.scan.mstmt )
    {
      mas_qstd_mstmt_free_result( li->db.scan.mstmt );
      mas_qstd_mstmt_delete( li->db.scan.mstmt );
      li->db.scan.mstmt = NULL;
    }
  }
  rRET;
}

int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  rC( masxfs_levinfo_db_opendir( li ) );
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
    masxfs_xstatc_t xstat = { 0 };

    rC( masxfs_levinfo_db_fetch( li->db.scan.mstmt, &dename, &stat, &xstat, &node_id, flags, phas_data ) );
    if ( rGOOD )
    {
      if ( *phas_data )
      {
      /* const char *name = NULL; */
        masxfs_entry_type_t detype = masxfs_levinfo_stat2entry( &stat );

        li->db.scan.node_id = node_id;
      /* rC( mas_qstd_mstmt_get_result_string_na( li->db.scan.mstmt, 0, &name ) ); */
        QRLI( li, rCODE );
        assert( !li[1].db.stat );
        masxfs_levinfo_init( li + 1, li->lidepth + 1, dename, detype, &stat, &xstat, node_id );
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
