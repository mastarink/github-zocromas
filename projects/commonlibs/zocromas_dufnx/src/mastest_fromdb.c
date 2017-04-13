#define R_GOOD(_r) ((_r)>=0)
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr_defs.h>
#include <mastar/exam/masexam.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include <mastar/qstd/qstd_mstmt_base.h>

static int numline_treecb = 0;
static int _uUu_
treecb( masxfs_levinfo_t * li _uUu_, masxfs_levinfo_flags_t flags _uUu_, void *data _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  FILE *fil = ( FILE * ) data;
  static masxfs_depth_t top_depth = 0;

/* EXAM( !epath, TRUE, "%d ? %d" ); */
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd _uUu_ = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath _uUu_ = masxfs_levinfo_path_ref( li, flags );

  if ( !numline_treecb && depth )
    top_depth = depth - 1;
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", top_depth + 1, flags );

  numline_treecb++;
#if 1
/* tree -U --inodes -s -a mastest | nl -ba -nrn -w4 > tree-U--inodes-s-a.tree */
  fprintf( fil, "%4d\t%s[%-10ld %10ld]  %s\n", numline_treecb, prefix ? prefix : "", inode, size, ename ? ename : "" /*, epath ? epath : "" */  );
#else
  fprintf( fil, "%4d. %s %ld fd:%d D:%ld i:%ld '%s'\n", numline_treecb, prefix ? prefix : "", size, fd, ( long ) depth, inode,
           ename ? ename : "" /*, epath ? epath : "" */  );
#endif
  return 0;
}

int
testtreefromdb( const char *path, masxfs_depth_t maxdepth, FILE * fil )
{
  rDECL( 0 );

  masxfs_entry_callback_t callbacks[] = {
    {treecb,.flags = /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_FD */ },
    {NULL}
  };
  WARN( "******** start *******" );

  {
  /* mas_qstd_t *qstd = */ mas_qstd_instance_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );
  /* if ( qstd->pfs ) */
    {
    /* const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest"; */
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 /* depth limit */ , 0 );

      {
        masxfs_levinfo_flags_t walkflags _uUu_ = MASXFS_CB_RECURSIVE | MASXFS_CB_STAT | MASXFS_CB_SINGLE_CB;
        masxfs_type_flags_t typeflags = MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR;
        masxfs_levinfo_flags_t xflags1 _uUu_ = MASXFS_CB_UP_ROOT;

      /* masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF_N_UP; */
        masxfs_levinfo_flags_t xflags2 _uUu_ = MASXFS_CB_FROM_ROOT | MASXFS_CB_SELF;

        walkflags |= MASXFS_CB_MODE_DB;
        numline_treecb = 0;
        rC( masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, fil /* data */ , walkflags | xflags2,
                                      maxdepth ) );
      }
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    }
    mas_qstd_instance_delete(  );
  }
  WARN( "******** end *******" );
  rRET;
}
