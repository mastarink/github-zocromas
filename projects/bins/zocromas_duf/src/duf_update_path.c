#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
/* #include "duf_path.h" */
/* #include "duf_insert.h" */
#include "duf_update_realpath.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_update_path.h"
/* ###################################################################### */


/* unsigned long long                                                                                   */
/* duf_update_path_down_filter( const char *path, unsigned long long parentid, duf_filter_t * pfilter ) */
/* {                                                                                                    */
/*   unsigned long long pathid = 0;                                                                     */
/*   char *real_path;                                                                                   */
/*                                                                                                      */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                      */
/*   real_path = mas_malloc( PATH_MAX );                                                                */
/*   if ( real_path )                                                                                   */
/*   {                                                                                                  */
/*     ( void ) realpath( path, real_path );                                                            */
/*     pathid = duf_update_realpath_down_filter( real_path, parentid, pfilter );                        */
/*     mas_free( real_path );                                                                           */
/*   }                                                                                                  */
/*   duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );                                             */
/*   return pathid;                                                                                     */
/* }                                                                                                    */

/* unsigned long long                                                                                                               */
/* duf_update_path( const char *path, unsigned long long parentid, duf_ufilter_t u, int level, int *pseq, int dofiles )             */
/* {                                                                                                                                */
/*   unsigned long long r;                                                                                                          */
/*                                                                                                                                  */
/* (*                                                                              *) duf_dbgfunc( DBG_START, __func__, __LINE__ ); */
/*   r = duf_update_path_down( path, parentid, u, level, pseq, dofiles );                                                           */
/* (*                                                                              *) duf_dbgfunc( DBG_END, __func__, __LINE__ );   */
/*   return r;                                                                                                                      */
/* }                                                                                                                                */

/* unsigned long long                                                                                                        */
/* duf_update_path_down( const char *path, unsigned long long parentid, duf_ufilter_t u, int level, int *pseq, int dofiles ) */
/* {                                                                                                                         */
/*   int seq = 0;                                                                                                            */
/*                                                                                                                           */
/*   (* fprintf( stderr, "=====================-------------- %p\n", ( void * ) pseq ); *)                                   */
/*   duf_filter_t filter = {.level = level,                                                                                  */
/*     .c = {.pseq = ( pseq ? pseq : &seq ),.dofiles = dofiles},                                                             */
/*     .u = u                                                                                                                */
/*   };                                                                                                                      */
/*   return duf_update_path_down_filter( path, parentid, &filter );                                                          */
/* }                                                                                                                         */
