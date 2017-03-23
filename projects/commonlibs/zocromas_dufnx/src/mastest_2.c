#define R_GOOD(_r) ((_r)>=0)
#include <mastar/qstd/qstd_defs.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr_defs.h>
#include <mastar/exam/masexam.h>

#include <mastar/masxfs/masxfs_types.h>
#include <mastar/masxfs/masxfs_structs.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/qstd/qstd_structs.h>
#include <mastar/qstd/qstd_mstmt_base.h>
#include <mastar/qstd/qstd_mstmt.h>
#include <mastar/qstd/qstd_mstmt_parents.h>
#include <mastar/qstd/qstd_mstmt_names.h>
#include <mastar/qstd/qstd_mstmt_sizes.h>
#include <mastar/qstd/qstd_mstmt_datas.h>
#include <mastar/qstd/qstd_mstmt_props.h>

static int
testtreefromfscb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *qstdv, masxfs_depth_t reldepth _uUu_ )
{
  mas_qstd_t *qstd = ( mas_qstd_t * ) qstdv;

  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );

  {
    unsigned long long theid = 0;
    unsigned long long parent_id = masxfs_levinfo_parent_id( li, flags );
    masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );
    unsigned long long as_parent_id = 0;
    unsigned long long dataid = 0;

  /* unsigned long long propid _uUu_ = 0; */

    const char *sdetypes[] = {
      [MASXFS_ENTRY_BLK_NUM] = "BLK",
      [MASXFS_ENTRY_CHR_NUM] = "CHR",
      [MASXFS_ENTRY_DIR_NUM] = "DIR",
      [MASXFS_ENTRY_REG_NUM] = "REG",
      [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
      [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
      [MASXFS_ENTRY_LNK_NUM] = "LNK"
    };
  /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
    {
      size_t size = masxfs_levinfo_size_ref( li, flags );
      size_t thesize _uUu_ = mas_qstd_mstmt_insget_sizes_id( qstd, size );

    /* WARN( "SIZE: %ld / %ld", size, thesize ); */
    }
    {
      const masxfs_stat_t *stat = masxfs_levinfo_stat_ref( li, flags );

      if ( stat )
      {
        dataid = mas_qstd_mstmt_selinsget_datas_id( qstd, stat );
      /* if ( detype == MASXFS_ENTRY_REG_NUM ) */
        mas_qstd_mstmt_selinsget_props_id( qstd, dataid, sdetypes[detype], stat );
      }
    }
    if ( masxfs_levinfo_depth_ref( li, flags ) != 0 )
    {
#if 0
      theid = mas_qstd_mstmt_insget_names_id( qstd, ename, parent_id, sdetypes[detype] );
#endif
#if 1
      theid = mas_qstd_mstmt_selinsget_names_id( qstd, ename, parent_id, dataid, sdetypes[detype] );
#endif
#if 0
      theid = mas_qstd_mstmt_insselget_names_id( qstd, ename, parent_id, dataid, sdetypes[detype] );
#endif
    }
    if ( detype == MASXFS_ENTRY_DIR_NUM )
    {
      as_parent_id = mas_qstd_mstmt_selinsget_parents_id( qstd, theid );
      masxfs_levinfo_set_node_id( li, as_parent_id );
    }
    if ( !theid || 0 == strcmp( ename, "home" ) || as_parent_id == 66 || as_parent_id == 1 )
      MARK( "(T6)", " %ld. '%s' ID: %llu => %llu; as_parent_id:%llu", ( long ) depth, ename, ( unsigned long long ) theid,
            ( unsigned long long ) parent_id, ( unsigned long long ) as_parent_id );
  }
  return 0;
}

static int numline_treecb = 0;
static int _uUu_
treecb( masxfs_levinfo_t * li _uUu_, masxfs_levinfo_flags_t flags _uUu_, void *data _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) data;
  static masxfs_depth_t top_depth = 0;

/* EXAM( !epath, TRUE, "%d ? %d" ); */
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );

  if ( !top_depth && depth )
    top_depth = depth - 1;
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth, flags );

  numline_treecb++;
  fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", numline_treecb, prefix ? prefix : "", size, fd, ( long ) depth, inode,
           ename ? ename : "" /*, epath ? epath : "" */  );

  return 0;
}

int
testtreefromfs( const char *path, masxfs_depth_t maxdepth, FILE * fil )
{
  rDECL( 0 );

  masxfs_entry_callback_t callbacks[] = {
    {                                                                /*MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, */
     testtreefromfscb,.flags =
     MASXFS_CB_NAME /* | MASXFS_CB_PATH */  | MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {                                                                /*MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, */
     treecb,.flags = MASXFS_CB_NAME | /* MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {NULL}
  };
  WARN( "******** testtreefromfs *******" );

  {
  /* mas_qstd_t *qstd = */ mas_qstd_instance_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );
  /* if ( qstd->pfs ) */
    {
    /* const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest"; */
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );

      {
        masxfs_levinfo_flags_t flagsfs _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS | MASXFS_CB_SINGLE_CB;
        masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;

        rC( masxfs_pathinfo_scan_cbs
            ( pi, typeflags, &callbacks[1], fil /* data */ , flagsfs /*| MASXFS_CB_UP_ROOT */ | MASXFS_CB_FROM_ROOT /*| MASXFS_CB_SELF_AND_UP */ ,
              maxdepth ) );
      }
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    }
    mas_qstd_instance_delete(  );
  }
  WARN( "******** /testtreefromfs *******" );
  rRET;
}
