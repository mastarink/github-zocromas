#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"


#include "duf_insert.h"

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *              int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_insert_filedata( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *presd;

  presd = ( unsigned long long * ) sel_cb_udata;
  *presd = strtol( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_filedata( unsigned long long file_inode, struct stat *pst_dir, struct stat *pst_file )
{
  int r;
  unsigned long long resd;

  /* char *sqli; */

  /* sqli = sqlite3_mprintf( "INSERT INTO duf_filedatas (dev,inode,size,md5id,ucnt,now) values ('%lu','%lu','%lu','%lu',0,datetime())", */
  /*                         pst_dir->st_dev, file_inode, pst_file ? pst_file->st_size : 0, 0 );                                 */
  /* r = sqlite3_exec( pDb, sqli, NULL, NULL, &errmsg );                                                                         */
  r = duf_sql_c( "INSERT INTO duf_filedatas (dev,inode,size,md5id,ucnt,now) values ('%lu','%lu','%lu','%lu',0,datetime())", 1,
                 pst_dir->st_dev, file_inode, pst_file ? pst_file->st_size : 0, 0 );
  /* sqlite3_free( sqli ); */
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_filedata, &resd, NULL, 0, "SELECT id FROM duf_filedatas WHERE dev='%lu' and inode='%lu'",
                        pst_dir->st_dev, file_inode );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resd = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_filedata %d\n", r );
  return resd;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *          int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_insert_path( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *pdir_id;

  /* fprintf( stderr, "Constraint selected %d\n", nrow ); */
  pdir_id = ( unsigned long long * ) sel_cb_udata;
  if ( nrow == 0 )
    *pdir_id = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_path( const char *base_name, struct stat *pst_dir, unsigned long long parentid )
{
  unsigned long long dir_id = 0;
  int r;

  char *qbase_name;

  /* fprintf( stderr, "Insert path %s\n", base_name ); */
  qbase_name = duf_single_quotes_2( base_name );
  r = duf_sql_c
        ( "INSERT INTO duf_paths (dev, inode, dirname, parentid, ucnt, now) values ('%lu','%lu','%s','%lu',0,datetime())", 1,
          pst_dir->st_dev, pst_dir->st_ino, qbase_name ? qbase_name : base_name, parentid );
  mas_free( qbase_name );
  /* sql = NULL; */
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_path, &dir_id, NULL, 0, "SELECT id FROM duf_paths WHERE dev='%lu' and inode='%lu'", pst_dir->st_dev,
                        pst_dir->st_ino );
  }
  else if ( !r /* assume SQLITE_OK */  )
  {
    dir_id = duf_last_insert_rowid(  );
    /* fprintf( stderr, "Inserted dir_id: %lld\n", dir_id ); */
  }
  else
    fprintf( stderr, "error duf_insert_path %d\n", r );
  /* fprintf( stderr, "Insert path %s done\n", base_name ); */
  return dir_id;
}

unsigned long long
duf_insert_filename( const char *fname, unsigned long long dir_id, unsigned long long resd )
{
  unsigned long long resf = -1;
  int r;

  /* char *sql; */
  char *fqname;

  fqname = duf_single_quotes_2( fname );
 r = duf_sql_c( "INSERT INTO duf_filenames (pathid, dataid, name, ucnt, now) values ('%lu','%lu','%s',0,datetime())", 1,
                 dir_id, resd, fqname ? fqname : fname );
  if ( !r /* assume SQLITE_OK */  )
  {
    resf = duf_last_insert_rowid(  );
    fprintf( stderr, "INSERT INTO duf_filenames :: %llu. [%s]\x1b[K\n", resf, fqname ? fqname : fname );
  }
  else if ( r == duf_constraint )
  {
  }
  else
    fprintf( stderr, "error duf_insert_filename %d\n", r );

  if ( fqname )
    mas_free( fqname );
  return resf;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *         int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_insert_md5( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *presmd;

  presmd = ( unsigned long long * ) sel_cb_udata;
  *presmd = strtoll( presult[0], NULL, 10 );
  duf_sql( "UPDATE duf_md5 SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", *presmd );
  return 0;
}

unsigned long long
duf_insert_md5( unsigned long long *md64, size_t fsize )
{
  unsigned long long resmd = -1;

  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_md5 (md5sum1,md5sum2,size,ucnt,now) values ('%lld','%lld','%llu',0,datetime())", 1, md64[1], md64[0],
                 ( unsigned long long ) fsize );

  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_md5, &resmd, NULL, 0, "SELECT id FROM duf_md5 WHERE md5sum1='%lld' and md5sum2='%lld'", md64[1],
                        md64[0] );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resmd = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );

  return resmd;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *           int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_insert_group( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *pid;

  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_group( const char *name )
{
  unsigned long long id = -1;

  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_group (name,now) values ('%s',datetime())", 1, name );

  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_group, &id, NULL, 0, "SELECT id FROM duf_group WHERE name='%s'", name );
  }
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );

  return id;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *           int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_insert_path_group( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *pid;

  pid = ( unsigned long long * ) sel_cb_udata;
  *pid = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_path_group( unsigned long long groupid, unsigned long long pathid )
{
  unsigned long long id = -1;

  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_path_group (groupid,pathid,now) values ('%lld','%lld',datetime())", 1, groupid, pathid );

  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_insert_path_group, &id, NULL, 0, "SELECT id FROM duf_path_group WHERE groupid='%lld' and  pathid='%lld'",
                        groupid, pathid );
  }
  else if ( !r /* assume SQLITE_OK */  )
    id = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_md5 %d\n", r );

  return id;
}
unsigned long long
duf_insert_keydata( unsigned long long pathid, unsigned long long nameid, ino_t inode, unsigned long long resmd )
{
  int r;
  unsigned long long resp = -1;

  r = duf_sql_c( "INSERT INTO duf_keydata (pathid, nameid, inode, md5id,ucnt,now) VALUES ('%llu', '%llu', '%llu', '%llu',0,datetime())", 1,
                 pathid, nameid, inode, resmd );
  if ( r == duf_constraint )
  {
  }
  else if ( !r /* assume SQLITE_OK */  )
    resp = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_keydata %d\n", r );

  return resp;
}
