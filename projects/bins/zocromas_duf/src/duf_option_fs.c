#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "duf_maintenance_options.h"


#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_option_fs_types.h"
#include "duf_option_fs.h"
/* ###################################################################### */

static duf_option_fs_mode_t
duf_fs_ask( const char *msg, const char *fn, duf_option_fs_mode_t mode )
{
  duf_option_fs_mode_t doit = DUF_OPTION_FS_MODE_NONE;
  char buf[1024] = "";
  char *s = NULL;

  switch ( mode )
  {
  case DUF_OPTION_FS_MODE_NONE:
    doit = DUF_OPTION_FS_MODE_NONE;
    break;
  case DUF_OPTION_FS_MODE_IGNORE:
    doit = DUF_OPTION_FS_MODE_IGNORE;
    break;
  case DUF_OPTION_FS_MODE_OVERWRITE:
    doit = DUF_OPTION_FS_MODE_OVERWRITE;
    break;
  case DUF_OPTION_FS_MODE_FORCE:
    doit = DUF_OPTION_FS_MODE_FORCE;
    break;
  case DUF_OPTION_FS_MODE_ASK:
    while ( 1 )
    {
      DUF_FPRINTF( 0, stderr, ".%s %s [y,n,yes,no]?", msg, fn );
      s = mas_chomp( fgets( buf, sizeof( buf ), stdin ) );
      if ( 0 == strcasecmp( s, "y" ) || 0 == strcasecmp( s, "yes" ) )
      {
        doit = DUF_OPTION_FS_MODE_FORCE;
        break;
      }
      else if ( 0 == strcasecmp( s, "n" ) || 0 == strcasecmp( s, "no" ) )
      {
        doit = DUF_OPTION_FS_MODE_NONE;
        break;
      }
      else if ( !*s )
      {
        doit = DUF_OPTION_FS_MODE_NONE;
        break;
      }
    }
    break;
  }
  return doit;
}

duf_error_code_t
duf_option_fs_each( const char *arg, duf_errc_psvv_func_t fun, const void *pv )
{
  DEBUG_STARTR( r );
  mas_argvc_t targ = { 0 };
  int c = 0;


  c = mas_add_argvc_args( &targ, arg, 0 );
  if ( c > 0 )
    for ( int i = 0; i < targ.argc; i++ )
    {
      DOR( r, ( fun ) ( targ.argv[i], pv ) );
    }
  mas_del_argvc( &targ, 0 );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_fs_each2( const char *arg, duf_errc_ps2vv_func_t fun, const void *pv )
{
  DEBUG_STARTR( r );
  mas_argvc_t targ = { 0 };
  int c = 0;


  c = mas_add_argvc_args( &targ, arg, 0 );
  if ( c > 1 )
    for ( int i = 0; i < targ.argc - 1; i++ )
    {
      DOR( r, ( fun ) ( targ.argv[i], targ.argv[targ.argc - 1], pv ) );
    }
  mas_del_argvc( &targ, 0 );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_fs_ls_file( const char *fn, const void *pv DUF_UNUSED )
{
  DEBUG_STARTR( r );
  int ry = 0;
  struct stat st = { 0 };

  ry = stat( fn, &st );
  if ( ry >= 0 )
  {
/* -rw-r--r-- 1 mastar mastar-msh   12288 Oct 01 2015 19:33:26 .test.cmds.swp */
    DUF_PRINTF( 0, "@@@ls:: %9lu %s", st.st_size, fn );
  }
  else
  {
    DUF_PRINTF( 0, "@@ls:: %9lu %s", st.st_size, fn );
  }

  DEBUG_ENDR( r );
}

int
duf_option_fs_rmfile( const char *fn, const void *pv )
{
  int ry = 0;
  duf_option_fs_mode_t dorm = 0;
  struct stat st = { 0 };

  ry = stat( fn, &st );
  if ( ry >= 0 )
  {
    dorm = duf_fs_ask( "remove", fn, ( duf_option_fs_mode_t ) pv );
    if ( dorm == DUF_OPTION_FS_MODE_FORCE )
    {
      /* T( "@removing %s", fn ); */
      ry = unlink( fn );
      /* if ( ry >= 0 )            */
      /*   T( "@removed %s", fn ); */
    }
  }
  if ( ry < 0 )
  {
    char serr[1024] = "";
    char *ser;

    ser = strerror_r( errno, serr, sizeof( serr ) );
    DUF_SHOW_ERROR( "@Can't remove %s - %s", fn, ser );
  }
  return ry >= 0 ? ( int ) dorm : ry;
}

duf_error_code_t
duf_option_$_fs_rmfile( const char *fn, const void *pv )
{
  DEBUG_STARTR( r );
  int ry = 0;

  ry = duf_option_fs_rmfile( fn, pv );
  if ( ry < 0 )
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );
  DEBUG_ENDR( r );
}

int
duf_option_fs_cpfile2absent( const char *fn, const char *to, const void *pv DUF_UNUSED, struct stat *pstfrom, struct stat *pstto DUF_UNUSED )
{
  int ry = 0;
  int ry1 = 0;
  char *topathf = NULL;
  char *topath = NULL;

  /* char *fname = NULL; */
  struct stat st2 = { 0 };

  topathf = mas_strdup( to );
  /* fname = basename( topathf ); */
  topath = dirname( topathf );
  ry = stat( topath, &st2 );
  if ( ry >= 0 )
  {
    FILE *ffrom, *fto;
    size_t bufsz = 1024 * 1000;
    char *buf = mas_malloc( bufsz );
    size_t nrs = 0, nws = 0;

    /* T( "@@copy to existing dir %s, absent file %s", topath, fname ); */
    fto = fopen( to, "w" );
    ffrom = fopen( fn, "r" );
    if ( ffrom && fto )
    {
      while ( ry >= 0 && !feof( ffrom ) && !ferror( ffrom ) && !ferror( fto ) )
      {
        size_t nr, nw;

        nr = fread( buf, 1, bufsz, ffrom );
        if ( nr > 0 )
          nrs += nr;
        if ( nr > 0 )
        {
          nw = fwrite( buf, 1, nr, fto );
          if ( nw > 0 )
            nws += nw;

          if ( ferror( fto ) )
          {
            DUF_SHOW_ERROR( "@Can' t copy %s %s - write error ", fn, to );
            ry = -1;
            break;
          }
          else if ( nr != nw )
          {
            DUF_SHOW_ERROR( "@Can' t copy %s %s - write error (nr!=nw : impossible?)", fn, to );
            ry = -1;
            break;
          }
        }
        else if ( ferror( ffrom ) )
        {
          DUF_SHOW_ERROR( " @ Can 't copy %s %s - read error", fn, to );
          ry = -1;
          break;
        }
        else
        {
          DUF_SHOW_ERROR( "@Can' t copy %s %s - read error (nr<=0 : impossible?)", fn, to );
          ry = -1;
          break;
        }
      }
      fclose( fto );
      fclose( ffrom );
    }
    else
    {
      DUF_SHOW_ERROR( "@Can' t copy %s(%d) %s(%d) - open error", fn, ffrom ? 1 : 0, to, fto ? 1 : 0 );
      ry = -1;
    }
    if ( ry >= 0 && nrs > 0 && nrs == nws )
    {
#if 0
      {
        struct timeval tt[2];

        tt[0].tv_sec = pstfrom->st_atim.tv_sec;
        tt[0].tv_usec = pstfrom->st_atim.tv_nsec / 1000;
        tt[1].tv_sec = pstfrom->st_mtim.tv_sec;
        tt[1].tv_usec = pstfrom->st_mtim.tv_nsec / 1000;
        ry = utimes( to, tt );
      }
#elif 0
      {
        struct timespec tt[2];
        int dirfd;

        dirfd = open( topath, O_DIRECTORY | O_RDONLY );
        tt[0].tv_sec = pstfrom->st_atim.tv_sec;
        tt[0].tv_nsec = pstfrom->st_atim.tv_nsec;
        tt[1].tv_sec = pstfrom->st_mtim.tv_sec;
        tt[1].tv_nsec = pstfrom->st_mtim.tv_nsec;
        ry = utimensat( dirfd, fname, tt, 0 );
        close( dirfd );
      }
#else
      {
        struct timespec tt[2];

        tt[0].tv_sec = pstfrom->st_atim.tv_sec;
        tt[0].tv_nsec = pstfrom->st_atim.tv_nsec;
        tt[1].tv_sec = pstfrom->st_mtim.tv_sec;
        tt[1].tv_nsec = pstfrom->st_mtim.tv_nsec;

        ry = utimensat( AT_FDCWD, to, tt, 0 );
        if ( 0 )
        {
          /* if i restore access time - i' ll set change time */
          tt[0].tv_sec = pstfrom->st_atim.tv_sec;
          tt[0].tv_nsec = pstfrom->st_atim.tv_nsec;
          tt[1].tv_sec = pstfrom->st_mtim.tv_sec;
          tt[1].tv_nsec = pstfrom->st_mtim.tv_nsec;
          ry1 = utimensat( AT_FDCWD, fn, tt, 0 );
          if ( ry >= 0 )
            ry = ry1;
        }
      }
#endif
    }
    if ( ry >= 0 )
    {
      ry = chmod( to, pstfrom->st_mode );
    }
    if ( ry >= 0 )
    {
      ry = chown( to, pstfrom->st_uid, pstfrom->st_gid );
    }
    mas_free( buf );
  }
  else if ( errno == ENOENT )
  {
    /* ry = 0; */
    DUF_SHOW_ERROR( "@Can't copy %s %s - no dst dir", fn, to );
    ry = -1;
  }
  mas_free( topathf );
  return ry;
}

int
duf_option_fs_cpfile( const char *fn, const char *to, const void *pv )
{
  struct stat stfrom = { 0 };
  struct stat stto = { 0 };
  int ry = 0;
  int dorm = 0;

  ry = stat( fn, &stfrom );
  if ( ry >= 0 )
  {
    if ( S_ISREG( stfrom.st_mode ) )
    {
      ry = stat( to, &stto );
      if ( ry >= 0 && ( S_ISREG( stto.st_mode ) || S_ISDIR( stto.st_mode ) ) )
      {
        dorm = duf_option_fs_rmfile( to, pv );
        ry = stat( to, &stto );
      }
      if ( ( ry < 0 && errno == ENOENT ) || dorm == DUF_OPTION_FS_MODE_OVERWRITE )
        ry = duf_option_fs_cpfile2absent( fn, to, pv, &stfrom, &stto );
      /* else                                                               */
      /*   T( "@@@did'nt copy %s to %s - dst present (%d)", fn, to, dorm ); */
    }
    else
    {
      DUF_SHOW_ERROR( "@Can't copy %s to %s - src is not a file", fn, to );
      ry = -1;
    }
  }

  if ( ry < 0 )
  {
    char serr[1024] = "";
    char *ser;

    ser = strerror_r( errno, serr, sizeof( serr ) );
    DUF_SHOW_ERROR( "@Can't copy %s %s -- %s", fn, to, ser );
  }
  return ry;
}

duf_error_code_t
duf_option_$_fs_cpfile( const char *fn, const char *to, const void *pv )
{
  DEBUG_STARTR( r );
  int ry;

  ry = duf_option_fs_cpfile( fn, to, pv );
  if ( ry == -1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_fs_mvfile( const char *arg DUF_UNUSED, const char *to DUF_UNUSED, const void *pv  DUF_UNUSED)
{
  DEBUG_STARTR( r );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_fs_ls( const char *arg )
{
  DEBUG_STARTR( r );
  long v = 0;

  /* T( "ls %s", arg ); */
  DOR( r, duf_option_fs_each( arg, duf_option_$_fs_ls_file, ( const void * ) v ) );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_fs_rm( const char *arg, long v )
{
  DEBUG_STARTR( r );
  /* T( "rm %s", arg ); */
  DOR( r, duf_option_fs_each( arg, duf_option_$_fs_rmfile, ( const void * ) v ) );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_fs_cp( const char *arg, long v )
{
  DEBUG_STARTR( r );
  /* T( "cp %s", arg ); */
  DOR( r, duf_option_fs_each2( arg, duf_option_$_fs_cpfile, ( const void * ) v ) );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_fs_mv( const char *arg, long v )
{
  DEBUG_STARTR( r );
  /* T( "mv %s", arg ); */
  DOR( r, duf_option_fs_each2( arg, duf_option_fs_mvfile, ( const void * ) v ) );
  DEBUG_ENDR( r );
}
