#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"


/* ###################################################################### */
#include "duf_mdpath.h"
/* ###################################################################### */


/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_mdpath( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                       void *str_cb_udata )
{
  unsigned long long *presmd;

  presmd = ( unsigned long long * ) sel_cb_udata;
  *presmd = strtoll( presult[0], NULL, 10 );
  duf_sql( DUF_TRACE_NO, "UPDATE duf_mdpath SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", *presmd );
  return 0;
}

unsigned long long
duf_insert_mdpath( unsigned long long *md64 )
{
  unsigned long long resmd = -1;
  int r = 0;

  r = duf_sql_c( DUF_TRACE_NO, "INSERT INTO duf_mdpath (mdpathsum1,mdpathsum2,ucnt,now) values ('%lld','%lld',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, md64[1], md64[0] );
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_mdpath, &resmd, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT id FROM duf_mdpath WHERE mdpathsum1='%lld' and mdpathsum2='%lld'", md64[1], md64[0] );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resmd = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_mdpath %d\n", r );

  return resmd;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
static int
duf_sql_update_mdpaths_path( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                             void *str_cb_udata )
{
  MD5_CTX *pctx;
  unsigned long long md5s1, md5s2;

  /* unsigned long long pathid; */

  /* pathid = va_arg( args, unsigned long long );                                                */
  /* fprintf( stderr, "(duf_sql_update_mdpaths_path) parthid:%lld / %s\n", pathid, presult[0] ); */
  pctx = ( MD5_CTX * ) sel_cb_udata;
  if ( presult[5] && presult[6] )
  {
    md5s1 = strtoll( presult[5], NULL, 10 );
    md5s2 = strtoll( presult[6], NULL, 10 );
    /* fprintf( stderr, "%lld:%lld\n", md5s1, md5s2 ); */
    MD5_Update( pctx, &md5s1, sizeof( md5s1 ) );
    MD5_Update( pctx, &md5s2, sizeof( md5s2 ) );
  }

  return 0;
}

int
duf_update_mdpaths_pathid( unsigned long long pathid )
{
  int r = 0;
  MD5_CTX ctx;
  unsigned long long *md64;
  unsigned long long mdpathid;

  MD5_Init( &ctx );
  if ( 0 )
    r = duf_sql_select( duf_sql_update_mdpaths_path, &ctx, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT duf_keydata.pathid, duf_keydata.filenameid, duf_filenames.name, '', '', duf_md5.md5sum1, duf_md5.md5sum2 "
                        " FROM duf_md5 "
                        " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "
                        " LEFT JOIN duf_filenames ON (duf_filenames.id=duf_keydata.filenameid)"
                        /* " LEFT JOIN duf_paths ON (duf_paths.id=duf_filenames.pathid)" */
                        " WHERE duf_keydata.pathid='%llu' " " ORDER by md5sum1, md5sum2 ", ( unsigned long long ) pathid );
  else if ( 1 )
    r = duf_sql_select( duf_sql_update_mdpaths_path, &ctx, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                        "SELECT duf_filenames.pathid, duf_filenames.id, duf_filenames.name, '', '', duf_md5.md5sum1, duf_md5.md5sum2 "
                        " FROM duf_filenames "
                        " LEFT JOIN duf_keydata ON (duf_keydata.filenameid=duf_filenames.id) "
                        " LEFT JOIN duf_md5 ON (duf_md5.id=duf_keydata.md5id)"
                        /* " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)" */
                        " WHERE duf_keydata.pathid='%llu' " " ORDER by duf_filenames.name ", ( unsigned long long ) pathid );

  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    MD5_Final( md, &ctx );
    md64 = ( unsigned long long * ) md;
  }
  mdpathid = duf_insert_mdpath( md64 );
  duf_sql( DUF_TRACE_YES, "UPDATE duf_paths " " SET md5dir1='%lld', md5dir2='%lld', mdpathid='%lld' " " WHERE id='%lu'", md64[1], md64[0],
           mdpathid, pathid );

  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * */
static int
duf_sql_update_mdpaths( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                        void *str_cb_udata )
{
  unsigned long long pathid;

  pathid = strtoll( presult[0], NULL, 10 );
  /* fprintf( stderr, "duf_sql_update_mdpaths %d\n", nrow ); */
  duf_update_mdpaths_pathid( pathid );
  return 0;
}

/* Sampe same(?) dirs                                                                                         */
/* 1439: /mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007                         */
/* 1676: /mnt/new_media/media/photo/Pictures.R.20120207.164339/Photo/unsorted/kodak/Kodak Pictures/08-14-2007 */
int
duf_update_mdpaths( unsigned long long pathid )
{
  int r;

  fprintf( stderr, "Start duf_update_mdpaths\n" );
  if ( pathid )
    r = duf_sql_select( duf_sql_update_mdpaths, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                        "SELECT id, dirname FROM duf_paths " " WHERE id='%lld' " " ORDER BY id", pathid );
  else
    r = duf_sql_select( duf_sql_update_mdpaths, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_YES,
                        "SELECT id, dirname FROM duf_paths " " ORDER BY id" );
  fprintf( stderr, "End duf_update_mdpaths\n" );
  return r;
}
