#include <fnmatch.h>

/* ###################################################################### */
#include "duf_match.h"
/* ###################################################################### */



int
duf_filename_match( duf_filter_globx_t * globx, const char *filename )
{
  int r=0;

  r = 1;
  if ( globx )
  {
    if ( globx->include_fs_files.argc )
    {
      int argc = globx->include_fs_files.argc;
      char *const *argv = globx->include_fs_files.argv;

      r = 0;
      /* DUF_TRACE_C( cfg, match, 2, "MATCH include argc:%d; %s", argc, filename ); */
      for ( int ia = 0; ia < argc; ia++ )
      {
        if ( 0 == fnmatch( argv[ia], filename, FNM_PATHNAME ) )
        {
          r = 1;
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
          r = 0;
          /* DUF_TRACE_C( cfg, match, 1, "EXCLUDE %s : %s", argv[ia], filename ); */
        }
      }
    }
  }
  /* DUF_TRACE_C( cfg, match, 2, "MATCH %s %s", filename, r ? "OK" : "FAIL" ); */
  return r;
}

int
duf_lim_match( duf_limits_t lim, int n )
{
  int r=0;

  r = 1;
  if ( lim.min )
    r = ( n >= lim.min ) ? r : 0;
  if ( lim.max )
    r = ( n <= lim.max ) ? r : 0;
  return r;
}

int
duf_lim_matchll( duf_limitsll_t lim, int n )
{
  int r=0;

  r = 1;
  if ( lim.min )
    r = ( n >= lim.min ) ? r : 0;
  if ( lim.max )
    r = ( n <= lim.max ) ? r : 0;
  return r;
}


/* int                                                                          */
/* duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id ) */
/* {                                                                            */
/*   int r=0;                                                                   */
/*                                                                              */
/*   r = 1;                                                                     */
/*   if ( md5id_filter )                                                        */
/*     r = ( md5id == md5id_filter ) ? r : 0;                                   */
/*   return r;                                                                  */
/* }                                                                            */
