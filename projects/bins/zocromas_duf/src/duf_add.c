#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* dirname, basename */
#include <libgen.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_sql_def.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"

#include "duf_dbg.h"
#include "duf_path.h"


/* ###################################################################### */
#include "duf_add.h"
/* ###################################################################### */




/* unsigned long long                                                                                */
/* duf_add_real_path_uni( const char *real_path, int up, int need_id, int *pr )                      */
/* {                                                                                                 */
/*   int r = 0;                                                                                      */
/*   unsigned long long dirid = 0;                                                                   */
/*   char *dir_name;                                                                                 */
/*   const char *base_name = NULL;                                                                   */
/*   char *rpath = mas_strdup( real_path );                                                          */
/*                                                                                                   */
/*   DEBUG_START(  );                                                                                */
/*                                                                                                   */
/*                                                                                                   */
/*   base_name = basename( rpath );                                                                  */
/*   dir_name = dirname( rpath );                                                                    */
/*   if ( !base_name || !*base_name )                                                                */
/*   {                                                                                               */
/*     DUF_TRACE( current, 0, "real_path:[%s] / [%s]", real_path, rpath );                           */
/*     base_name = real_path + 1;                                                                    */
/*     dir_name = NULL;                                                                              */
/*     DUF_TRACE( current, 0, "real_path:[%s] / [%s] => [%s]", real_path, rpath, base_name );        */
/*   }                                                                                               */
/*   if ( base_name && *base_name )                                                                  */
/*   {                                                                                               */
/*     struct stat st_dir;                                                                           */
/*                                                                                                   */
/*     (* stat needless here ? *)                                                                    */
/*     r = stat( real_path, &st_dir );                                                               */
/*     if ( r )                                                                                      */
/*     {                                                                                             */
/* (* no such entry *)                                                                               */
/*       DUF_ERROR( "No such entry %s\n", real_path );                                               */
/*     }                                                                                             */
/*     else                                                                                          */
/*     {                                                                                             */
/*       unsigned long long parentid = 0;                                                            */
/*                                                                                                   */
/*       (*                                                                                          */
/*        * insert path into db (recursively); return id                                             */
/*        * /a/b/c/d/e/f                                                                             */
/*        * '/a/b/c/d/e' -> db                                                                       */
/*        * *)                                                                                       */
/*       if ( ( dir_name && *dir_name ) && up )                                                      */
/*       {                                                                                           */
/*         DUF_TRACE( current, 0, "update parent %s", dir_name );                                    */
/*         parentid = duf_add_real_path_uni( dir_name, DUF_TRUE (* up *) , 1 (*need_id *) , &r );    */
/*       }                                                                                           */
/*       DUF_TRACE( current, 0, "updated parent %s AS %llu", dir_name, parentid );                   */
/*                                                                                                   */
/*       (*                                                                                          */
/*        * insert name into db; return id                                                           */
/*        * /a/b/c/d/e/f                                                                             */
/*        * 'f' -> db                                                                                */
/*        * *)                                                                                       */
/*       if ( r >= 0 )                                                                               */
/*       {                                                                                           */
/*         char *name = mas_strdup( base_name );                                                     */
/*                                                                                                   */
/*         DUF_TRACE( current, 0, "to insert [%s]", base_name );                                     */
/*         dirid = duf_insert_path_uni( name, st_dir.st_dev, st_dir.st_ino, parentid, need_id, &r ); */
/*         DUF_TRACE( current, 0, "inserted [%s] AS %llu", base_name, dirid );                       */
/*         mas_free( name );                                                                         */
/*       }                                                                                           */
/*       DUF_TRACE( action, 0, "#%llu : added real path %s", dirid, real_path );                     */
/*     }                                                                                             */
/*   }                                                                                               */
/*   mas_free( rpath );                                                                              */
/*   if ( pr )                                                                                       */
/*     *pr = r;                                                                                      */
/*   DEBUG_ENDULL( dirid );                                                                          */
/*   return dirid;                                                                                   */
/* }                                                                                                 */

unsigned long long
duf_add_path_uni( const char *path, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dirid = 0;

  DEBUG_START(  );
  {
    char *real_path = NULL;

    {
      char *rp = realpath( path, NULL );

      if ( rp )
      {
        real_path = mas_strdup( rp );
        free( rp );
      }
    }
    DUF_TRACE( action, 0, "real_path:%s", real_path );
    if (  /* strlen( real_path ) > 1 && */ !( real_path && *real_path == '/' && real_path[1] == 0 ) )
    {
      DUF_TRACE( current, 0, "Update parent %s", real_path );
      /* dirid = duf_add_real_path_uni( real_path, DUF_TRUE, need_id, &r ); */
      dirid = duf_real_path_to_pathid2( ( duf_depthinfo_t * ) NULL, real_path, 1 /* add */ , need_id, &r );
      if ( need_id )
        DUF_TRACE( current, 0, "Updated parent %s AS %llu", real_path, dirid );
    }
    mas_free( real_path );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( dirid );
  return dirid;
}
