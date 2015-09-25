#include <fnmatch.h>

/* ###################################################################### */
#include "duf_match.h"
/* ###################################################################### */



int
duf_filename_match( duf_filter_globx_t * globx, const char *filename )
{
  int rx = 0;

  rx = 1;
  if ( globx )
  {
    if ( globx->include_fs_files.argc )
    {
      int argc = globx->include_fs_files.argc;
      char *const *argv = globx->include_fs_files.argv;

      rx = 0;
      /* DUF_TRACE_C( cfg, match, 2, "MATCH include argc:%d; %s", argc, filename ); */
      for ( int ia = 0; ia < argc; ia++ )
      {
        if ( 0 == fnmatch( argv[ia], filename, FNM_PATHNAME ) )
        {
          rx = 1;
          /* DUF_TRACE_C( cfg, match, 1, "INCLUDE %s : %s", argv[ia], filename ); */
        }
      }
    }
    if ( globx->exclude_fs_files.argc )
    {
      int argc = globx->exclude_fs_files.argc;
      char *const *argv = globx->exclude_fs_files.argv;

      /* DUF_TRACE_C( cfg, match, 2, "MATCH exclude argc:%d; %s", argc, filename ); */
      for ( int ia = 0; ia < argc; ia++ )
      {
        if ( 0 == fnmatch( argv[ia], filename, FNM_PATHNAME ) )
        {
          rx = 0;
          /* DUF_TRACE_C( cfg, match, 1, "EXCLUDE %s : %s", argv[ia], filename ); */
        }
      }
    }
  }
  /* DUF_TRACE_C( cfg, match, 2, "MATCH %s %s", filename, rx ? "OK" : "FAIL" ); */
  return rx;
}

int
duf_lim_match( duf_limits_t lim, int n )
{
  int rx = 0;

  rx = 1;
  if ( lim.min )
    rx = ( n >= lim.min ) ? rx : 0;
  if ( lim.max )
    rx = ( n <= lim.max ) ? rx : 0;
  return rx;
}

int
duf_lim_matchll( duf_limitsll_t lim, int n )
{
  int rx = 0;

  rx = 1;
  if ( lim.min )
    rx = ( n >= lim.min ) ? rx : 0;
  if ( lim.max )
    rx = ( n <= lim.max ) ? rx : 0;
  return rx;
}


/* int                                                                          */
/* duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id ) */
/* {                                                                            */
/*   int rx=0;                                                                   */
/*                                                                              */
/*   rx = 1;                                                                     */
/*   if ( md5id_filter )                                                        */
/*     rx = ( md5id == md5id_filter ) ? rx : 0;                                   */
/*   return rx;                                                                  */
/* }                                                                            */
