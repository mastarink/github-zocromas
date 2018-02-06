#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
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
/* #include <mastar/qstd/qstd_mstmt_digest.h> */
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

/* TODO move to qstd?! */
/* make pmstmt to fetch record(s) by name and parent_id or parent_id only */
static int
masxfs_levinfo_db_prepare_execute_store_xx( mysqlpfs_mstmt_t ** pmstmt, const char *name, unsigned long long parent_id,
                                            masxfs_entry_filter_t * entry_pfilter )
{
  rDECLBAD;
  if ( pmstmt )
  {
    rSETGOOD;
    assert( !( *pmstmt ) );

    if ( !( *pmstmt ) )
    {
      ( *pmstmt ) = mas_qstd_mstmt_init_prepare( mas_qstd_instance(  ), name ? STD_MSTMT_SELECT_EVERYTHINGXX_PN : STD_MSTMT_SELECT_EVERYTHINGXX_P );
      {
        unsigned np = 0;

        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, parent_id, FALSE ) );
        QRG( rCODE );
        if ( name )
        {
          rC( mas_qstd_mstmt_set_param_string( ( *pmstmt ), np++, name ) );
          QRG( rCODE );
        }
      /* TODO filter here ! 20171228.144707 */
#if 0
        assert( np == ( name ? STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_PN : STD_MSTMT_SELECT_EVERYTHINGX_NFIELDS_P ) );
#else
      /* TODO add atim,ctim,mtim,nlink,parent_id (via parent path),uid,gid,dev */
        unsigned long min_nsamesize = ( entry_pfilter && entry_pfilter->min_nsamesize ) ? entry_pfilter->min_nsamesize : 0;
        unsigned long max_nsamesize = ( entry_pfilter && entry_pfilter->max_nsamesize ) ? entry_pfilter->max_nsamesize : LONG_MAX;
        unsigned long min_nsame_digest = ( entry_pfilter && entry_pfilter->min_nsame_digest ) ? entry_pfilter->min_nsame_digest : 0;
        unsigned long max_nsame_digest = ( entry_pfilter && entry_pfilter->max_nsame_digest ) ? entry_pfilter->max_nsame_digest : LONG_MAX;
        unsigned long min_size = ( entry_pfilter && entry_pfilter->min_size ) ? entry_pfilter->min_size : 0;
        unsigned long max_size = ( entry_pfilter && entry_pfilter->max_size ) ? entry_pfilter->max_size : LONG_MAX;
        unsigned long min_inode = ( entry_pfilter && entry_pfilter->min_inode ) ? entry_pfilter->min_inode : 0;
        unsigned long max_inode = ( entry_pfilter && entry_pfilter->max_inode ) ? entry_pfilter->max_inode : LONG_MAX;
        unsigned long min_digestid = ( entry_pfilter && entry_pfilter->min_digid ) ? entry_pfilter->min_digid : 0;
        unsigned long max_digestid = ( entry_pfilter && entry_pfilter->max_digid ) ? entry_pfilter->max_digid : LONG_MAX;

        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, min_nsamesize, FALSE ) ); /* min_nsamesize */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, max_nsamesize, FALSE ) ); /* max_nsamesize */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, min_nsame_digest, FALSE ) ); /* min_nsamedigest */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, max_nsame_digest, FALSE ) ); /* max_nsamedigest */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, min_size, FALSE ) ); /* min_size */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, max_size, FALSE ) ); /* max_size */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, min_inode, FALSE ) ); /* min_inode */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, max_inode, FALSE ) ); /* max_inode */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, min_digestid, FALSE ) ); /* min_digestid */
        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, max_digestid, FALSE ) ); /* max_digestid */
        rC( mas_qstd_mstmt_set_param_string( ( *pmstmt ), np++, "" /* "^.*\\.(sh|conf)$" */  ) ); /* regexp */
        assert( np == ( name ? STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_PN : STD_MSTMT_SELECT_EVERYTHINGXX_NFIELDS_P ) );
#endif
      /* assert( np == numpar ); */
        rC( mas_qstd_mstmt_execute_store( ( *pmstmt ) ) );
        QRG( rCODE );
      }
    }
  }
  QRG( rCODE );
  rRET;
}

/* TODO move to qstd */
/* allocates stat */
static int
masxfs_levinfo_db_fetch_xx( mysqlpfs_mstmt_t * mstmt, const char **pname, masxfs_stat_t * stat, masxfs_xstatc_t * xstatc,
                            unsigned long long *pnode_id, int *phas_data )
{
  rDECLBAD;
  int has_data = 0;

  if ( mstmt )
  {
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
    unsigned long long nsamedigest = 0;
    unsigned long long digestid = 0;
    unsigned long long parentid = 0;
    unsigned long long nameid = 0;
    unsigned long long dataid = 0;

    const char *hex_digest = NULL;

    rC( mas_qstd_mstmt_fetch( mstmt, &has_data ) );
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;
      unsigned nr = 0;

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
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nsamedigest, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &digestid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &parentid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nameid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &dataid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_string_na( mstmt, nr++, &hex_digest ) );
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
      if ( rGOOD && xstatc )
      {
        xstatc->dg.nsamesize = nsamesize;
        xstatc->dg.nsamedigest = nsamedigest;
        xstatc->dg.digestid = digestid;

        xstatc->chex_digest = hex_digest;

        xstatc->id.parentid = parentid;
        xstatc->id.nameid = nameid;
        xstatc->id.dataid = dataid;
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
  if ( phas_data )
    *phas_data = has_data;
  rRET;
}

static int
masxfs_levinfo_db_fetch_n( mysqlpfs_mstmt_t * mstmt, const char **pname, masxfs_stat_t * stat, masxfs_xstatc_t * xstatc,
                           unsigned long long *pnode_id, int *phas_data )
{
  rDECLBAD;
  int has_data = 0;

  if ( mstmt )
  {
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

    unsigned long long parentid = 0;
    unsigned long long nameid = 0;
    unsigned long long dataid = 0;

    rC( mas_qstd_mstmt_fetch( mstmt, &has_data ) );
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;
      unsigned nr = 0;

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

      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &parentid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &nameid, &is_null ) );
      rC( mas_qstd_mstmt_get_result_longlong( mstmt, nr++, &dataid, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_EVERYTHINGNM_NRESULTS );
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
      if ( rGOOD && xstatc )
      {
        xstatc->id.parentid = parentid;
        xstatc->id.nameid = nameid;
        xstatc->id.dataid = dataid;
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
  if ( phas_data )
    *phas_data = has_data;
  rRET;
}

static int
masxfs_levinfo_db_prepare_execute_store_1( mysqlpfs_mstmt_t ** pmstmt, unsigned long long any_id, mas_qstd_id_t opid, mas_qstd_nfields_t nfid )
{
  rDECLBAD;
  if ( pmstmt )
  {
    rSETGOOD;
    assert( !( *pmstmt ) );

    if ( !( *pmstmt ) )
    {
      ( *pmstmt ) = mas_qstd_mstmt_init_prepare( mas_qstd_instance(  ), opid );
      {
        unsigned np = 0;

        rC( mas_qstd_mstmt_set_param_longlong( ( *pmstmt ), np++, any_id, FALSE ) );
        QRG( rCODE );
        assert( np == nfid );
      /* assert( np == numpar ); */
        rC( mas_qstd_mstmt_execute_store( ( *pmstmt ) ) );
        QRG( rCODE );
      }
    }
  }
  QRG( rCODE );
  rRET;
}

static int
masxfs_levinfo_db_prepare_execute_store_nm( mysqlpfs_mstmt_t ** pmstmt, unsigned long long name_id )
{
  rDECLBAD;
  rC( masxfs_levinfo_db_prepare_execute_store_1( pmstmt, name_id, STD_MSTMT_SELECT_EVERYTHINGNM, STD_MSTMT_SELECT_EVERYTHINGNM_NFIELDS ) );
  QRG( rCODE );
  rRET;
}

static int
masxfs_levinfo_db_prepare_execute_store_nd( mysqlpfs_mstmt_t ** pmstmt, unsigned long long node_id )
{
  rDECLBAD;
  rC( masxfs_levinfo_db_prepare_execute_store_1( pmstmt, node_id, STD_MSTMT_SELECT_EVERYTHINGND, STD_MSTMT_SELECT_EVERYTHINGND_NFIELDS ) );
  QRG( rCODE );
  rRET;
}

static int
masxfs_levinfo_db_closemstmt( mysqlpfs_mstmt_t * mstmt )
{
  rDECLBAD;
  if ( mstmt )
  {
    rSETGOOD;
    mas_qstd_mstmt_free_result( mstmt );
    mas_qstd_mstmt_delete( mstmt );
  }
  rRET;
}

masxfs_levinfo_t *
masxfs_levinfo_nameid2lia( unsigned long long nameid )
{
  rDECLBAD;
  int has_data = 0;
  unsigned long long cur_node_id = 0;

  {
    const char *dename = NULL;

    masxfs_stat_t stat = { 0 };
    masxfs_xstatc_t xstatc = { 0 };
    masxfs_entry_type_t detype _uUu_ = MASXFS_ENTRY_NONE_NUM;
    mysqlpfs_mstmt_t *mstmt = NULL;
    unsigned long long node_id = 0;

    {
      masxfs_levinfo_db_prepare_execute_store_nm( &mstmt, nameid );
      rC( masxfs_levinfo_db_fetch_n( mstmt, &dename, &stat, &xstatc, &node_id, &has_data ) );
      masxfs_levinfo_db_closemstmt( mstmt );
      mstmt = NULL;
      detype = dename && rGOOD ? masxfs_levinfo_stat2entry( &stat ) : MASXFS_ENTRY_UNKNOWN_NUM;
    }
    cur_node_id = xstatc.id.parentid;
    WARN( "AHA %s : parent:%llu", dename, cur_node_id );
  }
  while ( cur_node_id )
  {
    mysqlpfs_mstmt_t *mstmt = NULL;
    unsigned long long node_id = 0;
    const char *dename = NULL;
    masxfs_stat_t stat = { 0 };
    masxfs_xstatc_t xstatc = { 0 };
    masxfs_entry_type_t detype _uUu_ = MASXFS_ENTRY_NONE_NUM;

    {
      masxfs_levinfo_db_prepare_execute_store_nd( &mstmt, cur_node_id );
      rC( masxfs_levinfo_db_fetch_n( mstmt, &dename, &stat, &xstatc, &node_id, &has_data ) );
      masxfs_levinfo_db_closemstmt( mstmt );
      mstmt = NULL;
      detype = dename && rGOOD ? masxfs_levinfo_stat2entry( &stat ) : MASXFS_ENTRY_UNKNOWN_NUM;
      cur_node_id = xstatc.id.parentid;
    }
    WARN( "OHO %s : parent:%llu", dename, cur_node_id );
  }
  return NULL;
}

int
masxfs_levinfo_db_opened_dir( const masxfs_levinfo_t * li )
{
  return li && li->db.scan.mstmt ? 1 : 0;
}

int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter )
{
  rDECLBAD;
  if ( li )
  {
    rSETGOOD;
    assert( !masxfs_levinfo_db_opened_dir( li ) );
    if ( !li->db.scan.mstmt )
    {
      if ( !masxfs_levinfo_db_opened( li ) )
        rC( masxfs_levinfo_db_open( li ) );
      QRLI( li, rCODE );
      rC( masxfs_levinfo_db_prepare_execute_store_xx( &li->db.scan.mstmt, li[1].name, ( long long ) li->db.node_id, entry_pfilter ) );
      QRLI( li, rCODE );
      assert( rBAD || masxfs_levinfo_db_opened( li ) );
    }
    assert( rBAD || masxfs_levinfo_db_opened_dir( li ) );
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

    masxfs_levinfo_db_closemstmt( li->db.scan.mstmt );
    li->db.scan.mstmt = NULL;
  }
  rRET;
}

int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li )
{
  rDECLBAD;

/* rC( masxfs_levinfo_db_opendir( li, entry_pfilter ) ); */
  if ( li )
    rC( mas_qstd_mstmt_data_seek( li->db.scan.mstmt, 0 ) );
  rRET;
}

int
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, int *phas_data )
{
  rDECLBAD;

  int has_data = 0;

  if ( li )
  {
    int validx = 1;

    do
    {
      masxfs_entry_type_t detype = MASXFS_ENTRY_NONE_NUM;
      unsigned long long node_id = 0;
      const char *dename = NULL;
      masxfs_stat_t stat = { 0 };
      masxfs_xstatc_t xstatc = { 0 };

      do
      {
        rC( masxfs_levinfo_db_fetch_xx( li->db.scan.mstmt, &dename, &stat, &xstatc, &node_id, &has_data ) );
        detype = dename && rGOOD ? masxfs_levinfo_stat2entry( &stat ) : MASXFS_ENTRY_UNKNOWN_NUM;
      } while ( rGOOD && dename && has_data && !masxfs_levinfo_name_valid( dename, detype, entry_pfilter ) );
      if ( rGOOD )
      {
        validx = 1;
        if ( has_data )
        {
          li->db.scan.node_id = node_id;
          QRLI( li, rCODE );
#if 0
          {
            assert( !li[1].db.stat );
            masxfs_levinfo_init( li + 1, li->lidepth + 1, dename, detype, &stat, &xstatc, node_id );
            assert( li[1].db.stat );
            {
              validx = masxfs_levinfo_stat_valid( li + 1, entry_pfilter, MASXFS_CB_MODE_DB )
                      && masxfs_levinfo_xstat_valid( li + 1, entry_pfilter, MASXFS_CB_MODE_DB );
              if ( !validx )
              {
                masxfs_levinfo_reset( li + 1, MASXFS_CB_MODE_DB );   /* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
                continue;
              }
            }
          }
#else
          validx = masxfs_levinfo_init_valid( li + 1, li->lidepth + 1, entry_pfilter, dename, detype, node_id, &stat, &xstatc );
#endif
        }
      }
    } while ( rGOOD && has_data && !validx );
  }
  else
    QRLI( li, rCODE );
  if ( phas_data )
    *phas_data = has_data;
/* WARN( "li '%s' (%p) has:%d", li->name, li->db.stat, *phas_data ); */
  rRET;
}
