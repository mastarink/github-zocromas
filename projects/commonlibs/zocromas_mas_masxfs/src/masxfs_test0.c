#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_structs.h"

#include "masxfs_error.h"

#include "masxfs_exam.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

/*  */

static int num = 0;
static int _uUu_
fscallback_dir( const char *npath, const char *name )
{
  fprintf( stderr, "entry directory: '%s'\n   -- %s\n", name, npath );
  return 0;
}

static int
fscallback_regular( const char *npath, const char *name )
{
  num++;
  fprintf( stderr, "b. %-2d. -- '%s/%s'\n", num, npath, name );
  return 0;
}

static int _uUu_
fscallback( const char *npath, const char *name )
{
  num++;
  fprintf( stderr, "a. %-2d. -- '%s/%s'\n", num, npath, name );
  return 0;
}

int
masxfs_test_0_path( int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_, const char *_path, size_t _maxpath, size_t _depth,
                    size_t _tdepth, char *_tname, char *_lastname )
{
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( _path, _maxpath );

  {

    EXAM( pi->pidepth, _depth, "pidepth=%ld ? %ld" );
    {
      char *real_path = masxfs_pathinfo_pi2path( pi );

      EXAMS( real_path, _path, "real_path=%s ?\n\t\t\t\t\t\t%s" );
      mas_free( real_path );
    }
    EXAMS( pi->levinfo[0].name, "", "root '%s' ? '%s'" );
    EXAMS( pi->levinfo[_tdepth].name, _tname, "tdepth '%s' ? '%s'" );
    EXAMS( pi->levinfo[pi->pidepth - 1].name, _lastname, "last '%s' ? '%s'" );
    EXAM( pi->levinfo[0].dirfd, 0, "dirfd:%d ? %d" );
    EXAM( pi->levinfo[0].dir, NULL, "dirfd:%d ? %d" );
    EXAM( pi->levinfo[0].dir, NULL, "dir:%p ? %p" );
    EXAM( pi->levinfo[0].de, NULL, "de:%p ? %p" );
    for ( size_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( masxfs_levinfo_root( pi->levinfo + i ), pi->levinfo, "%p ? %p" );
      EXAM( pi->levinfo[i].lidepth, i, "li->lidepth:%ld ? %ld" );
      {
        char *path = masxfs_levinfo_li2path( pi->levinfo + i );

        EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
    }
    {
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo );

        EXAMS( path, "/", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
      size_t fromdepth = 1;

      if ( _depth > fromdepth )
      {
        char *path = masxfs_levinfo_li2path_up( pi->levinfo + fromdepth );

        EXAMS( path, "/home", "%s ?\n\t\t\t\t\t\t%s" );
        mas_free( path );
      }
    }
    {
      char *path = masxfs_levinfo_li2path( pi->levinfo );

      EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
      mas_free( path );
      path = masxfs_levinfo_li2path( pi->levinfo + 1 );

      EXAMS( path, _path, "%s ?\n\t\t\t\t\t\t%s" );
      mas_free( path );
    }
  }
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

    masxfs_levinfo_opendirfd( li );
    EXAM( ( size_t ) ( li - pi->levinfo ), _depth - 1, "masxfs_pathinfo_last_li: %ld ? %ld" );
    for ( size_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].dirfd, ( int ) i + 3, "%ld ? %ld" );
    }
    masxfs_levinfo_closedirfd_all_up( li );
    for ( size_t i = 0; i < pi->pidepth; i++ )
    {
      EXAM( pi->levinfo[i].dirfd, ( int ) 0, "%ld ? %ld" );
    }
  }
  masxfs_pathinfo_delete( pi );
  return 0;
}

int
masxfs_test_0( int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_ )
{
  mastest_next_group(  );
  fprintf( stderr, "@@@@@@@@@@@@@@@@@@@@@@@@\n" );
  EXAM( 1, 1, "%d ? %d" );
  {
    masxfs_test_0_path( nseries, series_suffix, do_fprintf, "/", 128, 1, 0, "", "" );
    masxfs_test_0_path( nseries, series_suffix, do_fprintf, "/home", 128, 2, 0, "", "home" );
    masxfs_test_0_path( nseries, series_suffix, do_fprintf,
                        "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_mas_masxfs", 128, 13, 0, "",
                        "zocromas_mas_masxfs" );
    masxfs_test_0_path( nseries, series_suffix, do_fprintf,
                        "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_mas_masxfs", 128, 13, 0, "",
                        "zocromas_mas_masxfs" );
  }
  {
    masxfs_entry_callback_t callbacks[] = {
    /* {MASXFS_ENTRY_BIT_LINK | MASXFS_ENTRY_BIT_REG, fscallback}, */
      {MASXFS_ENTRY_BIT_REG, fscallback}
      , {MASXFS_ENTRY_BIT_REG, fscallback_regular}
      , {0, NULL}
    };
    masxfs_scanpath_real( NULL, callbacks, TRUE );
  }
  return 0;
}
