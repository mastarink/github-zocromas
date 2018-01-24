#define R_GOOD(_r) ((_r)>=0)
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

#include <mastar/levinfo/masxfs_levinfo_structs.h>

/* #include <mastar/levinfo/masxfs_levinfo_base.h> */
#include <mastar/levinfo/masxfs_levinfo_io.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"
#include "mastest.h"

/*  */
struct rlimit lim = { 0 };

static int num = 0;
static int _uUu_
fscallback_dir( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *userdata _uUu_, masxfs_depth_t reldepth _uUu_ )
{
/* INFO(  "entry directory: '%s'\n   -- %s",  ename ? ename : "", epath ? epath : ""); */
  return 0;
}

static int _uUu_
fscallback2( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *userdata _uUu_, unsigned long serial _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  num++;
/* EXAM( !epath, TRUE, "%d ? %d" ); */
  const char *prefix = masxfs_levinfo_prefix_ref( li, "    ", "└── ", "│   ", "├── ", flags, 0 );
  size_t size = masxfs_levinfo_size_ref( li, flags );
  int fd = masxfs_levinfo_fd_ref( li, flags );
  masxfs_depth_t depth = masxfs_levinfo_depth_ref( li, flags );
  ino_t inode = masxfs_levinfo_inode_ref( li, flags );
  const char *ename = masxfs_levinfo_name_ref( li, flags );
  const char *epath = masxfs_levinfo_path_ref( li, flags );

  INFO( "%s %ld fd:%d D:%ld i:%ld %s; %s", prefix ? prefix : "", size, fd, ( long ) depth, inode, ename ? ename : "", epath ? epath : "" );

  return 0;
}

static int _uUu_
fscallback( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *userdata _uUu_, unsigned long serial _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  num++;
/* INFO( "a. %-2d. -- '%s%s'", num, ename ? ename : "", epath ? epath : "" ); */
  return 0;
}

static int _uUu_
testcb( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *udata _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  char *real_path = masxfs_pathinfo_pi2path( ( masxfs_pathinfo_t * ) udata );

  INFO( "%ld. %s - %s", ( long ) depth, ename, real_path );

  mas_free( real_path );

  return 0;
}

int
masxfs_test_0_path( int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_, const char *_path, size_t _maxpath,
                    masxfs_depth_t _depth, masxfs_depth_t _tdepth, char *_tname, char *_lastname )
{
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( _path, _maxpath, 0 );

  INFO( "A============================================[%ld]======== #%ld", ( long ) pi->pidepth, masexam_tests_count(  ) );
  {
    EXAM( pi->pidepth, _depth, "pidepth=%ld ? %ld" );
/* +1 */
    {
      char *real_path = masxfs_pathinfo_pi2path( pi );

      EXAMS( real_path, _path, "real_path=%s ?\n\t\t\t\t\t\t%s" );
      mas_free( real_path );
    }
    EXAM( pi->levinfo ? 1 : 0, 1, "%d ? %d" );
/* 1+2 */
    EXAMTS( pi->levinfo, pi->levinfo[0].name, "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[0].name, "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[_tdepth].name, _tname, "tdepth '%s' ? '%s'" );
    EXAMTS( pi->levinfo, pi->levinfo[pi->pidepth - 1].name, _lastname, "last '%s' ? '%s'" );
    EXAMT( pi->levinfo, pi->levinfo[0].fd, 0, "fd:%d ? %d" );
    EXAMT( pi->levinfo, pi->levinfo[0].fs.scan.pdir, NULL, "fd:%d ? %d" );
    EXAMT( pi->levinfo, pi->levinfo[0].fs.scan.pdir, NULL, "dir:%p ? %p" );
/* 1+2+7 */
    EXAMTS( pi->levinfo, masxfs_levinfo_name_val( pi->levinfo, 0 ), "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, masxfs_levinfo_name_val( pi->levinfo, 0 ), "", "root '%s' ? '%s'" );
    EXAMTS( pi->levinfo, masxfs_levinfo_name_val( pi->levinfo, _tdepth ), _tname, "tdepth '%s' ? '%s'" );
    EXAMTS( pi->levinfo, masxfs_levinfo_name_val( pi->levinfo, pi->pidepth - 1 ), _lastname, "last '%s' ? '%s'" );
    EXAMT( pi->levinfo, masxfs_levinfo_fd_val( pi->levinfo, 0 ), 0, "fd:%d ? %d" );
    EXAMT( pi->levinfo, masxfs_levinfo_pdir_val( pi->levinfo, 0, MASXFS_CB_MODE_FS ), NULL, "dir:%d ? %d" );
    EXAMT( pi->levinfo, masxfs_levinfo_pdir_val( pi->levinfo, 0, MASXFS_CB_MODE_FS ), NULL, "dir:%p ? %p" );
/* 1+2+7+7 */
    {
      if ( pi->levinfo )
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo, 0 );

        EXAMS( path, "/", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
/* 1+2+7+7+1 */
      masxfs_depth_t fromdepth = 1;

      if ( _depth > fromdepth && pi->levinfo )
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo + fromdepth, 0 );

        EXAMS( path, "/home", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
/* 1+2+7+7+1+1(for d>1) */
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
/* 1+2+7+7+1+1(for d>1)+2 */

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
/* 1+2+7+7+1+1(for d>1)+2 + 3*depth */
  }
  if ( pi->levinfo )
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

    masxfs_levinfo_open( li, MASXFS_CB_MODE_FS );
    EXAM( ( masxfs_depth_t ) ( li - pi->levinfo ), _depth - 1, "masxfs_pathinfo_last_li: %ld ? %ld" );
/* 1+2+7+7+1+1(for d>1)+2 + 3*depth + 1 */
    for ( masxfs_depth_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].fd, ( int ) i + 3, "%ld ? %ld" );
    }
/* 1+2+7+7+1+1(for d>1)+2 + 3*depth + 1 +1*depth */
    masxfs_levinfo_close_all_up( li, MASXFS_CB_MODE_FS );
    for ( masxfs_depth_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].fd, ( int ) 0, "closed fd: %ld ? %ld" );
    }
/* 1+2+7+7+1+1(for d>1)+2 + 3*depth + 1 +1*depth +1*depth */
  }
/* 1+2+7+7+1+1(for d>1)+2     +1 + 5*depth *//*  */
  masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_FS );
  INFO( "Z==================================================== #%ld", masexam_tests_count(  ) );
  return 0;
}

void
masxfs_test_fd( int variant _uUu_ )
{
  int r;
  int *fds;

  r = getrlimit( RLIMIT_NOFILE, &lim );
  EXAM( r, 0, "%d ? %d" );
/* +1 */
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
/* 1 + rlim*1 */
  {
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      EXAM( fds[i], ( int ) i, "fds:%d ? %d" );
    }
  /* 1 + rlim + rlim-3 */
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
      if ( fds[i] && 0 == close( fds[i] ) )
        fds[i] = 0;
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      EXAM( fds[i], 0, "fds:%d ? %d" );
    }
  /* 1 + rlim + rlim-3 + rlim-3 */
    for ( size_t i = 3; i < lim.rlim_cur; i++ )
    {
      int x _uUu_ = fcntl( ( int ) i, F_GETFD );

      EXAM( x, -1, "fdx:%d ? %d" );
    }
  /* 1 + rlim + rlim-3 + rlim-3 + rlim-3 */
  }
/* 1 + 4*rlim - 3*3 */
  errno = 0;
  mas_free( fds );
}

int
masxfs_test_0( int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  getrlimit( RLIMIT_NOFILE, &lim );
  masxfs_test_fd( __LINE__ );
#define CUR_TESTS (0L +  TEST_FD_EXAMS*1L)
/* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
  EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
  if ( 1 )
  {
    masxfs_entry_callback_t callbacks[] = {
      { /*MASXFS_ENTRY_REG | MASXFS_ENTRY_DIR, */ .cb_type = MASXFS_CBTYPE_SIMPLE,.fun_simple =fscallback,.flags =
       /* MASXFS_CB_OFF_NAME | */ MASXFS_CB_PATH | MASXFS_CB_TRAILINGSLASH /* | MASXFS_CB_NO_FD */ | MASXFS_CB_SKIP}
      , { /*MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, */ .cb_type = MASXFS_CBTYPE_SIMPLE,.fun_simple =fscallback2,.flags =
         /* MASXFS_CB_OFF_NAME | MASXFS_CB_PATH | */ MASXFS_CB_PREFIX | MASXFS_CB_TRAILINGSLASH | MASXFS_CB_STAT /* | MASXFS_CB_NO_FD */}
      , {0}
    };
    WARN( "callbacks: %p", callbacks );
#if 0
  /* ftw */
    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
    num = 0;
    masxfs_scanpath_real( "/", callbacks, MASXFS_CB_RECURSIVE, 10 );
    INFO( "NNNNNNNNNN %d", num );

    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
    num = 0;
    masxfs_scanpath_real( NULL, callbacks, MASXFS_CB_RECURSIVE | MASXFS_CB_SINGLE_CB, 10000 );
    INFO( "NNNNNNNNNN %d", num );
#endif
    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
    num = 0;
    masxfs_scanpath_real2( NULL, MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, callbacks, NULL,
                           ( masxfs_cb_flag_bit_t ) MASXFS_CB_RECURSIVE | MASXFS_CB_SINGLE_CB | MASXFS_CB_MODE_FS, 10000 );
    INFO( "NNNNNNNNNN %d", num );

    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
    num = 0;
    masxfs_scanpath_real( "./mastest", MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, callbacks, NULL,
                          ( masxfs_cb_flag_bit_t ) MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS, 10000 );
    INFO( "NNNNNNNNNN %d", num );

    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
    num = 0;
    masxfs_scanpath_real( "./mastest/tree/Makefile.in", MASXFS_ENTRY_REG | MASXFS_ENTRY_LNK | MASXFS_ENTRY_DIR, callbacks, NULL,
                          ( masxfs_cb_flag_bit_t ) MASXFS_CB_RECURSIVE | MASXFS_CB_MODE_FS, 10000 );
    INFO( "NNNNNNNNNN %d", num );

    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-" );
  /* EXAM( num, 1292 * 2, "num:%d ? %d" );                            // MASXFS_ENTRY_REG|MASXFS_ENTRY_DIR and MASXFS_ENTRY_REG|MASXFS_ENTRY_DIR */
  /* EXAM( num, 1241 * 2, "num:%d ? %d" ); // MASXFS_ENTRY_REG and MASXFS_ENTRY_REG */
  }
#define CUR_TESTS (1L +  TEST_FD_EXAMS*1L)
/* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
  EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
  masxfs_test_fd( __LINE__ );
#define CUR_TESTS (2L +  TEST_FD_EXAMS*2L)
/* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
  EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
  masxfs_test_fd( __LINE__ );
#define CUR_TESTS (3L +  TEST_FD_EXAMS*3L)
/* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
  EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS

  if ( 1 )
  {
    masexam_next_group(  );
    INFO( "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#- TESTS:" );
    {
      masxfs_pathinfo_t *pi =
              masxfs_pathinfo_create_setup( "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest",
                                            128, 0 );

      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
    }
    char *tpath = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    char *tpathe _uUu_ = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastes";

    EXAM( 1, 1, "%d ? %d" );
    EXAM( PATH_MAX, 4096, "%d ? %d" );
    EXAM( pathconf( tpath, _PC_PATH_MAX ), 4096, "%d ? %d" );
    EXAM( pathconf( tpath, _PC_NAME_MAX ), 255, "%d ? %d" );
    EXAM( _POSIX_PATH_MAX, 256, "%d ? %d" );
#define CUR_TESTS (4L +  TEST_FD_EXAMS*3L  +5L)
    /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
    EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
    {
      masxfs_test_0_path( nseries, series_suffix, variant, "/", 128, 1, 0, "", "" );
#define CUR_TESTS (5L +  TEST_FD_EXAMS*3L  +5L + TEST_0_PATH_EXAMS(1))
      /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
      EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
      masxfs_test_0_path( nseries, series_suffix, variant, "/home", 128, 2, 0, "", "home" );
#define CUR_TESTS (6L +  TEST_FD_EXAMS*3L  +5L + TEST_0_PATH_EXAMS(1) + TEST_0_PATH_EXAMS(2))
      /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
      EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
      masxfs_test_0_path( nseries, series_suffix, variant, tpath, 128, 14, 0, "", "mastest" );
#define CUR_TESTS (7L +  TEST_FD_EXAMS*3L  +5L + TEST_0_PATH_EXAMS(1) + TEST_0_PATH_EXAMS(2) + TEST_0_PATH_EXAMS(14))
      /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
      EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
      masxfs_test_0_path( nseries, series_suffix, variant, tpath, 128, 14, 0, "", "mastest" );
#define CUR_TESTS (8L +  TEST_FD_EXAMS*3L  +5L + TEST_0_PATH_EXAMS(1) + TEST_0_PATH_EXAMS(2) + TEST_0_PATH_EXAMS(14)*2)
      /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
      EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
    }
    {
      masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( tpathe, 128, 0 );

      EXAM( pi ? pi->error : -1, -1, "should be error: %d ? %d" );
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
    }
  /* +6 +4* */
  }
  masxfs_test_fd( __LINE__ );
#define CUR_TESTS (9L +  TEST_FD_EXAMS*4L  +5L + 1L + TEST_0_PATH_EXAMS(1L) + TEST_0_PATH_EXAMS(2L) + TEST_0_PATH_EXAMS(14)*2L)
  /* masexam_exam( 0, masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS ); */
  EXAMX( masexam_tests_count(  ) == CUR_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), CUR_TESTS );
#undef CUR_TESTS
/* INFO( "%ld TESTS", ( 10L + TEST_FD_EXAMS * 4L + 5L + 1L + TEST_0_PATH_EXAMS( 1L ) + TEST_0_PATH_EXAMS( 2L ) + TEST_0_PATH_EXAMS( 14 ) * 2L ) ); */
  return 0;
}
