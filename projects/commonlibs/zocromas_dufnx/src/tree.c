#define R_GOOD(_r) ((_r)>=0)
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_utils_path.h>

#include <mastar/minierr/minierr.h>
/* #include <mastar/regerr/masregerr.h> */
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>
#include <mastar/masxfs/masxfs_pathinfo_ref.h>

#include <mastar/levinfo/masxfs_levinfo_tools.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_ref_xstat.h>

#include <mastar/levinfo/masxfs_levinfo_digest.h>

/* #include <mastar/qstd/qstd_mstmt_base.h> */

#include "structs.h"
#include "mysql.h"

#include "tree.h"


/* of type: masxfs_scan_fun_simple_t */
static int _uUu_
treecb( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, void *userdata, unsigned long serial, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) userdata;
  static masxfs_depth_t top_depth = 0;

/* EXAM( !epath, TRUE, "%d ? %d" ); */
  size_t size = masxfs_levinfo_size_ref( li, flags );

/*int fd _uUu_ = masxfs_levinfo_fd_ref( li, flags );*/
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *sha1 = masxfs_levinfo_hexsha1_ref( li, flags );
  unsigned long nsamesha1 = masxfs_levinfo_nsamesha1_ref( li, flags );

/*const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );*/

  if ( !serial && depth )
    top_depth = depth - 1;
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth + 1, flags );

  {
    /**
    const masxfs_digests_t *digests = li->digests;
    const unsigned char *dg = NULL;

    int sz =                                                         // masxfs_digests_getn( li->digests, 0, NULL ); //
            masxfs_digests_get( digests, MASXFS_DIGEST_SHA1, &dg );

    char sbuffer[512] = { 0 };
    char *psbuf = sbuffer;

    for ( ssize_t i = 0; i < sz; i++ )
    {
      snprintf( psbuf, sizeof( sbuffer ) - ( psbuf - sbuffer ), "%02x", dg[i] );
      psbuf += 2;
    } **/
#if 1
    char hh[32];

    memset( hh, 0, sizeof( hh ) );
    if ( nsamesha1 == 0 )
      hh[0] = 0;
    else if ( nsamesha1 > 31 )
      sprintf( hh, "# %lu", nsamesha1 );
    else
      for ( unsigned long i = 0; i < nsamesha1; i++ )
        hh[i] = '+';
/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
    fprintf( fil, "%4ld\t%s[%-10ld %10ld]  %-30s  \t%-40s \t%s\n", serial, prefix ? prefix : "", inode, size,
             ename ? ename : "" /*, epath ? epath : "" */ , sha1 ? sha1 : "", hh );
#else
    fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", serial, prefix ? prefix : "", size, fd, ( long ) depth, inode,
             ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  }
  return 0;
}

static int _uUu_
treestatcb( const char *ename, struct stat *st, void *userdata, unsigned depth _uUu_, unsigned long serial, const char *prefix, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) userdata;
  static unsigned top_depth _uUu_ = 0;
  size_t size = st->st_size;
  ino_t inode = st->st_ino;


  {
#if 1
/* /usr/bin/tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
    fprintf( fil, "%4ld\t%s[%-10ld %10ld]  %-30s\n", serial, prefix ? prefix : "?", inode, size,
             ename ? ename : "" /*, epath ? epath : "" */  );
#else
    fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", serial, prefix ? prefix : "", size, fd, ( long ) depth, inode,
             ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  }
  return 0;
}

int
dufnx_tree( const char *real_path, masxfs_depth_t maxdepth, FILE * fil, masxfs_levinfo_flags_t inflags, mas_dufnx_mysql_data_t * mysql )
{
  rDECLGOOD;

  masxfs_levinfo_flags_t walkflags = MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB | inflags;

  masxfs_entry_callback_t callbacks[] = {
#if 0
    {.fun_simple = treecb,.flags =
     /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ ,.entry_filter =
     {.maxdepth = 0}},
#else
    {.fun_stat = treestatcb,.flags = MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT,.entry_filter = {.maxdepth = 0}},
#endif
    {NULL}
  };
  dufnx_qstd( mysql );
  masxfs_pathinfo_t *pi = NULL;

#if 0
  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );
  masxfs_pathinfo_open( pi, walkflags );
  WARN( "pidepth:%d", masxfs_pathinfo_pidepth( pi ) );
  for ( masxfs_depth_t i = 0; i < masxfs_pathinfo_pidepth( pi ); i++ )
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_li( pi, i );

    WARN( "node_id:%ld %s", masxfs_levinfo_node_id_val( li ), masxfs_levinfo_detype2s( masxfs_levinfo_detype( li, walkflags ) ) );
  }
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
  pi - NULL;
#endif

  pi = masxfs_pathinfo_create_setup_realpath( real_path, 128 /* depth limit */ , 0 );

/* masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT; */

/* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */
  masxfs_levinfo_flags_t xflags2 = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF;

  WARN( "**<TREE>**" );
#if 0
  masxfs_scanner_t scanner = {
    .entry_pfilter = {
                      .typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR,.maxdepth = maxdepth /* */
                      }
    ,.cbs = cbs,.flags = flags,.maxdepth = maxdepth
  };
  masxfs_pathinfo_scanf_scanner( pi, &scanner, userdata );

#else
  masxfs_entry_filter_t entry_filter = {.typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR,.maxdepth = maxdepth };
  rC( masxfs_pathinfo_scanf_cbs( pi, &entry_filter, callbacks, fil /* userdata */ , walkflags | xflags2,
                                 0 ) );
#endif
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
  rRET;
}
