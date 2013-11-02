#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"
#include "duf_path.h"
#include "duf_dirent.h"
#include "duf_insert.h"

#include "duf_update_realpath.h"
#include "duf_update_pathid.h"
#include "duf_update_path.h"
#include "duf_update_pathentries.h"

#include "duf_update.h"




unsigned long long
duf_add_path( const char *path, const char *group )
{
  unsigned long long pathid = 0;
  char *rpath;

  fprintf( stderr, "Add path %s group: %s\n", path, group );

  rpath = mas_malloc( PATH_MAX );
  if ( rpath )
  {
    ( void ) realpath( path, rpath );
    fprintf( stderr, "path:%s; rpath:%s;\n\n", path, rpath );
    if (  /* strlen( rpath ) > 1 && */ !( rpath && *rpath == '/' && rpath[1] == 0 ) )
      pathid = duf_update_realpath_self_up( rpath, group, 1 );
    mas_free( rpath );
  }
  return pathid;
}

/* unsigned long long                                                                                    */
/* duf_update_pathid( unsigned long long pathid, unsigned long long parentid, int recursive, int dofiles ) */
/* {                                                                                                     */
/*   fprintf( stderr, "path:%s; rpath:%s;\n\n", path, rpath );                                           */
/*   if ( strlen( rpath ) > 1 )                                                                          */
/*     pathid = duf_update_pathid_down( parentid, recursive, dofiles, pathid );                             */
/*                                                                                                       */
/*   return pathid;                                                                                      */
/* }                                                                                                     */


/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *                  int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata );
 * */
static int
duf_sql_update_mdpaths_path( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                             void *str_cb_udata )
{
  MD5_CTX *pctx;
  unsigned long long md5s1, md5s2, pathid;
  unsigned long long *md64;

  pathid = va_arg( args, unsigned long long );

  pctx = ( MD5_CTX * ) sel_cb_udata;
  if ( presult[0] && presult[1] )
  {
    md5s1 = strtoll( presult[0], NULL, 10 );
    md5s2 = strtoll( presult[1], NULL, 10 );
    MD5_Update( pctx, &md5s1, sizeof( md5s1 ) );
    MD5_Update( pctx, &md5s2, sizeof( md5s2 ) );
  }

  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    MD5_Final( md, pctx );
    md64 = ( unsigned long long * ) md;
  }
  duf_sql( "UPDATE duf_paths SET md5dir1='%lld', md5dir2='%lld' WHERE id='%lu'", md64[1], md64[0], pathid );
  return 0;
}

int
duf_update_mdpaths_path( unsigned long long pathid )
{
  int r = 0;
  MD5_CTX ctx;

  r = duf_sql_select( duf_sql_update_mdpaths_path, &ctx, STR_CB_DEF, STR_CB_UDATA_DEF, 0,
                      "SELECT md5sum1, md5sum2 FROM duf_md5 LEFT JOIN duf_keydata on (duf_keydata.md5id=duf_md5.id) "
                      " WHERE pathid='%llu' ORDER by md5sum1, md5sum2 ", pathid );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *             int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata );
 * */
static int
duf_sql_update_mdpaths( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  unsigned long long pathid;

  pathid = strtol( presult[0], NULL, 10 );
  fprintf( stderr, "duf_update_mdpaths %d\n", nrow );
  duf_update_mdpaths_path( pathid );
  return 0;
}

int
duf_update_mdpaths( void )
{
  int r;

  fprintf( stderr, "Start duf_update_mdpaths\n" );
  r = duf_sql_select( duf_sql_update_mdpaths, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, 0,
                      "SELECT id, dir FROM duf_paths " /* " WHERE ... IS NULL " */ " ORDER BY id" );
  fprintf( stderr, "End duf_update_mdpaths\n" );
  return r;
}
