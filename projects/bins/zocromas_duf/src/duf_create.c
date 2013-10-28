#include <stdio.h>

#include <sqlite3.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_def.h"
#include "duf_create.h"

sqlite3 *pDb = NULL;
char *errmsg = NULL;

static int
check_table_filedatas( void )
{
  int r;
  /* char *errmsg; */

  r = sqlite3_exec( pDb, "CREATE TABLE IF NOT EXISTS "
                    " filedatas (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, size INTEGER, md5id INTEGER, now REAL)",
                    NULL, NULL, &errmsg );;
  if ( r == SQLITE_OK )
    printf( "Create filedatas OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS " " filedatas_md5id ON filedatas (md5id)", NULL, NULL, &errmsg );;
  if ( r == SQLITE_OK )
    printf( "Create filedatas 1 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS " " filedatas_size ON filedatas (size)", NULL, NULL, &errmsg );;
  if ( r == SQLITE_OK )
    printf( "Create filedatas 2 OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS " " filedatas_uniq ON filedatas (dev,inode)", NULL, NULL, &errmsg );;
  if ( r == SQLITE_OK )
    printf( "Create filedatas 3 OK\n" );
  return r;
}

static int
check_table_filenames( void )
{
  int r;
  /* char *errmsg; */

  r = sqlite3_exec( pDb, "CREATE TABLE IF NOT EXISTS "
                    " filenames (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, name TEXT, pathid INTEGER, now REAL)", NULL, NULL,
                    &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create filenames OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS " " filenames_filename ON filenames (name)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create filenames 1 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS " " filenames_filename ON filenames (dataid)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create filenames 2 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS " " filenames_pathid ON filenames (pathid)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create filenames 3 OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS " " filenames_uniq ON filenames (pathid,name)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create filenames 4 OK\n" );
  return r;
}

static int
check_table_paths( void )
{
  int r;
  /* char *errmsg; */

  r = sqlite3_exec( pDb, "CREATE TABLE IF NOT EXISTS "
                    " paths (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, items INTEGER, "
                    " dirname TEXT, mdline TEXT, parentid INTEGER, now REAL)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_dirname ON paths (dirname)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths 1 OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS paths_uniq ON paths (dev,inode)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths 2 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_inode ON paths (inode)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths 3 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_items ON paths (items)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths 4 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_parentid ON paths (parentid)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create paths 5 OK\n" );
  r = sqlite3_exec( pDb, "CREATE INDEX IF NOT EXISTS paths_mdline ON paths (mdline)", NULL, NULL, &errmsg );
  return r;
}

static int
check_table_md5( void )
{
  int r;
  char *errmsg;
  r = sqlite3_exec( pDb, "CREATE TABLE IF NOT EXISTS "
                    " md5 (id INTEGER PRIMARY KEY autoincrement, md5sum1 INTEGER, md5sum2 INTEGER, dupcnt INTEGER, now REAL)", NULL, NULL,
                    &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create md5 OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS md5_md5sum ON md5 (md5sum1,md5sum2)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create md5 1 OK\n" );
  r = sqlite3_exec( pDb, "CREATE TABLE IF NOT EXISTS "
                    " keydata (id INTEGER PRIMARY KEY autoincrement, md5id INTEGER, inode INTEGER, nameid INTEGER, pathid INTEGER, now REAL)",
                    NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create keydata OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS keydata_uniq ON keydata (pathid,nameid,md5id)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create keydata 1 OK\n" );
  return r;
}

int
check_tables( void )
{
  int r;
  /* char *errmsg; */

  check_table_filedatas(  );
  check_table_filenames(  );
  check_table_paths(  );
  check_table_md5(  );
  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " exif (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, datetime INTEGER, d INTEGER, broken_date INTEGER, now REAL)",
                    NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create exif OK\n" );
  r = sqlite3_exec( pDb, "CREATE UNIQUE INDEX IF NOT EXISTS exif_uniq ON exif (dataid)", NULL, NULL, &errmsg );
  if ( r == SQLITE_OK )
    printf( "Create exif 1 OK\n" );
  return r;
}
