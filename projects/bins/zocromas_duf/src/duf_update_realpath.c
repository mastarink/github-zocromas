#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_path.h"

#include "duf_dbg.h"

#include "duf_group.h"

/* ###################################################################### */
#include "duf_update_realpath.h"
/* ###################################################################### */


/* insert full path into db; return id 
 * /a/b/c/d/e/f
 * '/a/b/c/d/e/f' -> db
 * */
unsigned long long
duf_update_realpath_self_up( const char *real_path, const char *group, int up, int need_id, int *pr )
{
  int r=0;
  unsigned long long pathid = 0;
  char *dir_name;
  const char *base_name = NULL;
  char *rpath = mas_strdup( real_path );

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( action, 0, "!!" );

  base_name = basename( rpath );
  dir_name = dirname( rpath );
  if ( !base_name || !*base_name )
  {
    base_name = real_path + 1;
    dir_name = NULL;
  }
  if ( base_name && *base_name )
  {
    struct stat st_dir;

    /* stat needless here ? */
    r = stat( real_path, &st_dir );
    if ( r )
    {
/* no such entry */
      DUF_ERROR( "No such entry %s", real_path );
      r = DUF_ERROR_STAT;
    }
    else
    {
      unsigned long long parentid = 0;

      /* 
       * insert path into db (recursively); return id 
       * /a/b/c/d/e/f
       * '/a/b/c/d/e' -> db
       * */
      if ( ( dir_name && *dir_name ) && up )
        parentid = duf_update_realpath_self_up( dir_name, NULL /* group */ , DUF_TRUE /* up */ , 1 /*need_id */ , &r );

      /* 
       * insert name into db; return id 
       * /a/b/c/d/e/f
       * 'f' -> db
       * */
      if ( r >= 0 )
        pathid = duf_insert_path( base_name, &st_dir, parentid, 1 /*need_id */ , &r );

      if ( r >= 0 && group )
        duf_pathid_group( group, pathid, +1 );
    }
  }
  mas_free( rpath );
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

/* insert full path into db; return id 
 * /a/b/c/d/e/f
 * '/a/b/c/d/e/f/...' -> db
 * */
/* unsigned long long                                                                                            */
/* duf_update_realpath_down_filter( const char *real_path, unsigned long long parentid, duf_filter_t * pfilter ) */
/* {                                                                                                             */
/*   unsigned long long pathid = 0;                                                                              */
/*                                                                                                               */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                               */
/*   (* fprintf( stderr, "Update real_path UP %s\n", real_path ); *)                                             */
/*                                                                                                               */
/*   if ( !pfilter->u.noself_dir )                                                                               */
/*   {                                                                                                           */
/*     pathid = duf_update_realpath_self_up( real_path, NULL, !pfilter->u.noupper_dirs (* DUF_TRUE * up *)  );   */
/*   }                                                                                                           */
/*   pathid = duf_update_pathid_down_filter( parentid, pfilter, pathid );                                        */
/*                                                                                                               */
/*   duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );                                                      */
/*   return pathid;                                                                                              */
/* }                                                                                                             */

/* unsigned long long                                                                                                                           */
/* duf_update_realpath_down( const char *real_path, unsigned long long parentid, int recursive, int level, int maxdepth, int *pseq, int maxseq, */
/*                           int dofiles )                                                                                                      */
/* {                                                                                                                                            */
/*   duf_filter_t filter = {.level = level,                                                                                                     */
/*     .c = {.pseq = pseq,.dofiles = dofiles},                                                                                                  */
/*     .u = {.recursive = recursive,.maxdepth = maxdepth,.maxseq = maxseq}                                                                      */
/*   };                                                                                                                                         */
/*   return duf_update_realpath_down_filter( real_path, parentid, &filter );                                                                    */
/* }                                                                                                                                            */
