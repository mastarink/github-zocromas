#include "masxfs_defs.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "masxfs_structs.h"

/* #include "masxfs_levinfo_base.h" */
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

/*  */

static int num = 0;
static int _uUu_
fscallback_dir( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_ )
{
/* printf(  "entry directory: '%s'\n   -- %s\n",  ename ? ename : "", epath ? epath : ""); */
  return 0;
}

static int _uUu_
fscallback2( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_ )
{
  num++;
/* EXAM( !epath, TRUE, "%d ? %d" ); */
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", flags );
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t deinode = masxfs_levinfo_deinode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath = masxfs_levinfo_path_ref( li, flags );

  printf( "%s %ld fd:%d D:%ld i:%ld %s; %s\n", prefix ? prefix : "", size, fd, depth, deinode, ename ? ename : "", epath ? epath : "" );

  return 0;
}

static int _uUu_
fscallback( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_ )
{
  num++;
/* printf( "a. %-2d. -- '%s%s'\n", num, ename ? ename : "", epath ? epath : "" ); */
  return 0;
}

static int
testcb( const char *name _uUu_, size_t depth _uUu_, void *li _uUu_, void *udata _uUu_ )
{
  {
    char *real_path = masxfs_pathinfo_pi2path( ( masxfs_pathinfo_t * ) udata );

    fprintf( stderr, "%ld. %s - %s\n", depth, name, real_path );

    mas_free( real_path );
  }

  return 0;
}

int
masxfs_test_0_path( int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_, const char *_path, size_t _maxpath,
                    masxfs_depth_t _depth, masxfs_depth_t _tdepth, char *_tname, char *_lastname )
{
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( _path, _maxpath );

  fprintf( stderr, "A====================================================\n" );
  masxfs_pathinfo_each_depth_cb( pi, testcb, pi );
  fprintf( stderr, "B====================================================\n" );
  {
    EXAM( pi->pidepth, _depth, "pidepth=%ld ? %ld" );
    {
      char *real_path = masxfs_pathinfo_pi2path( pi );

      EXAMS( real_path, _path, "real_path=%s ?\n\t\t\t\t\t\t%s" );
      mas_free( real_path );
    }
    EXAM( pi->levinfo ? 1 : 0, 1, "%d ? %d" );
    EXAMTS( pi->levinfo, pi->levinfo[0].name, "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[0].name, "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[_tdepth].name, _tname, "tdepth '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[pi->pidepth - 1].name, _lastname, "last '%s' ? '%s'" );
    EXAMT( pi->levinfo, pi->levinfo[0].fd, 0, "fd:%d ? %d" );
    EXAMT( pi->levinfo, pi->levinfo[0].pdir, NULL, "fd:%d ? %d" );
    EXAMT( pi->levinfo, pi->levinfo[0].pdir, NULL, "dir:%p ? %p" );
    EXAMT( pi->levinfo, pi->levinfo[0].pde, NULL, "de:%p ? %p" );
    for ( masxfs_depth_t i = 0; i < pi->pidepth; i++ )
    {
      EXAMT( pi->levinfo, masxfs_levinfo_root( pi->levinfo + i ), pi->levinfo, "%p ? %p" );
      EXAMT( pi->levinfo, pi->levinfo[i].lidepth, i, "li->lidepth:%ld ? %ld" );
      if ( pi->levinfo )
      {
        char *path = masxfs_levinfo_li2path( pi->levinfo + i );

        EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
    }
    {
      if ( pi->levinfo )
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo, 0 );

        EXAMS( path, "/", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
      masxfs_depth_t fromdepth = 1;

      if ( _depth > fromdepth && pi->levinfo )
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo + fromdepth, 0 );

        EXAMS( path, "/home", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
    }
    if ( pi->levinfo )
    {
      char *path = masxfs_levinfo_li2path( pi->levinfo );

      EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
      mas_free( path );
      path = masxfs_levinfo_li2path( pi->levinfo + 1 );

      EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
      mas_free( path );
    }
  }
  if ( pi->levinfo )
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

    masxfs_levinfo_open( li );
    EXAM( ( masxfs_depth_t ) ( li - pi->levinfo ), _depth - 1, "masxfs_pathinfo_last_li: %ld ? %ld" );
    for ( masxfs_depth_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].fd, ( int ) i + 3, "%ld ? %ld" );
    }
    masxfs_levinfo_close_all_up( li );
    for ( masxfs_depth_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].fd, ( int ) 0, "%ld ? %ld" );
    }
  }
  masxfs_pathinfo_delete( pi );
  return 0;
}

void
masxfs_test_fd( int variant _uUu_ )
{
  int r;
  int *fds;
  struct rlimit lim = { 103, 100 };

  r = getrlimit( RLIMIT_NOFILE, &lim );
  EXAM( r, 0, "%d ? %d" );
  fds = mas_malloc( lim.rlim_cur * sizeof( int ) );
/* WARN( "RLIMIT_NOFILE: %ld - %ld", lim.rlim_cur, lim.rlim_max ); */
  for ( size_t i = 3; i < lim.rlim_cur; i++ )
    fds[i] = 0;
  for ( size_t i = 3; i < lim.rlim_cur; i++ )
    fds[i] = open( "Makefile.in", O_RDONLY );
  for ( size_t i = 0; i < lim.rlim_cur; i++ )
  {
    int x = fcntl( ( int ) i, F_GETFD );

    EXAM( x, 0, "fdx:%d ? %d" );
  }
  {
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      EXAM( fds[i], ( int ) i, "fds:%d ? %d" );
    }
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
      if ( fds[i] && 0 == close( fds[i] ) )
        fds[i] = 0;
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      EXAM( fds[i], 0, "fds:%d ? %d" );
    }
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      int x _uUu_ = fcntl( ( int ) i, F_GETFD );

      EXAM( x, -1, "fdx:%d ? %d" );
    }
  }
  errno = 0;
  mas_free( fds );
}

int
masxfs_test_0( int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_ )
{
  masxfs_test_fd( __LINE__ );
  if ( 1 )
  {
    masxfs_entry_callback_t callbacks[] = {
    /* {MASXFS_ENTRY_LINK | MASXFS_ENTRY_REG, fscallback}, */
      {MASXFS_ENTRY_REG | MASXFS_ENTRY_DIR, fscallback,
       .flags = 0 | MASXFS_CB_NAME | MASXFS_CB_PATH | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_FD | MASXFS_CB_SKIP}
      , {MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, fscallback2,
         .flags = 0 | MASXFS_CB_NAME | /* MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT | MASXFS_CB_FD}
      , {0, NULL}
    };
    WARN( "callbacks: %p", callbacks );
#if 0
  /* ftw */
    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
    num = 0;
    masxfs_scanpath_real( "/", callbacks, MASXFS_CB_RECURSIVE | MASXFS_CB_MULTIPLE_CBS, 10 );
    fprintf( stderr, "NNNNNNNNNN %d\n", num );

    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
    num = 0;
    masxfs_scanpath_real( NULL, callbacks, MASXFS_CB_RECURSIVE, 10000 );
    fprintf( stderr, "NNNNNNNNNN %d\n", num );
#endif
    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
    num = 0;
    masxfs_scanpath_real2( NULL, callbacks, MASXFS_CB_RECURSIVE, 10000 );
    fprintf( stderr, "NNNNNNNNNN %d\n", num );

    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
    num = 0;
    masxfs_scanpath_real( "./mastest", callbacks, MASXFS_CB_RECURSIVE | MASXFS_CB_MULTIPLE_CBS, 10000 );
    fprintf( stderr, "NNNNNNNNNN %d\n", num );

    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
    num = 0;
    masxfs_scanpath_real( "./mastest/tree/Makefile.in", callbacks, MASXFS_CB_RECURSIVE | MASXFS_CB_MULTIPLE_CBS, 10000 );
    fprintf( stderr, "NNNNNNNNNN %d\n", num );

    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-\n" );
  /* EXAM( num, 1292 * 2, "num:%d ? %d" );                            // MASXFS_ENTRY_REG|MASXFS_ENTRY_DIR and MASXFS_ENTRY_REG|MASXFS_ENTRY_DIR */
  /* EXAM( num, 1241 * 2, "num:%d ? %d" ); // MASXFS_ENTRY_REG and MASXFS_ENTRY_REG */
  }
  masxfs_test_fd( __LINE__ );
  masxfs_test_fd( __LINE__ );

  if ( 1 )
  {
    masexam_next_group(  );
    fprintf( stderr, "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#- TESTS:\n" );
    {
      masxfs_pathinfo_t *pi =
              masxfs_pathinfo_create_setup( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest",
                                            128 );

      masxfs_pathinfo_delete( pi );
    }
    char *tpath = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    char *tpathe _uUu_ = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastes";

    EXAM( 1, 1, "%d ? %d" );
    EXAM( PATH_MAX, 4096, "%d ? %d" );
    EXAM( pathconf( tpath, _PC_PATH_MAX ), 4096, "%d ? %d" );
    EXAM( pathconf( tpath, _PC_NAME_MAX ), 255, "%d ? %d" );
    EXAM( _POSIX_PATH_MAX, 256, "%d ? %d" );
    {
      masxfs_test_0_path( nseries, series_suffix, do_fprintf, "/", 128, 1, 0, "", "" );
      masxfs_test_0_path( nseries, series_suffix, do_fprintf, "/home", 128, 2, 0, "", "home" );
      masxfs_test_0_path( nseries, series_suffix, do_fprintf, tpath, 128, 14, 0, "", "mastest" );
      masxfs_test_0_path( nseries, series_suffix, do_fprintf, tpath, 128, 14, 0, "", "mastest" );
    }
    {
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( tpathe, 128 );

      EXAM( pi ? pi->error : -1, -1, "should be error: %d ? %d" );
      masxfs_pathinfo_delete( pi );
    }
  }
  masxfs_test_fd( __LINE__ );
  return 0;
}
