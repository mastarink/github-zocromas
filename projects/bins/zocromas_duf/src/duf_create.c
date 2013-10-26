#include <stdio.h>

#include <sqlite3.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_def.h"
#include "duf_create.h"

sqlite3 *pDb = NULL;
char *errmsg = NULL;

static void
check_table_filedatas( void )
{
  SQL_EXECC( "CREATE TABLE IF NOT EXISTS "
             " filedatas (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, size INTEGER, md5id INTEGER, now REAL)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS " " filedatas_md5id ON filedatas (md5id)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS " " filedatas_size ON filedatas (size)" );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS " " filedatas_uniq ON filedatas (dev,inode)" );
}

static void
check_table_filenames( void )
{
  SQL_EXECC( "CREATE TABLE IF NOT EXISTS "
             " filenames (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, name TEXT, pathid INTEGER, now REAL)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS " " filenames_filename ON filenames (name)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS " " filenames_filename ON filenames (dataid)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS " " filenames_pathid ON filenames (pathid)" );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS " " filenames_uniq ON filenames (pathid,name)" );
}

static void
check_table_paths( void )
{
  SQL_EXECC( "CREATE TABLE IF NOT EXISTS "
             " paths (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, items INTEGER, "
             " dirname TEXT, mdline TEXT, parentid INTEGER, now REAL)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS paths_dirname ON paths (dirname)" );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS paths_uniq ON paths (dev,inode)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS paths_inode ON paths (inode)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS paths_items ON paths (items)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS paths_parentid ON paths (parentid)" );
  SQL_EXECC( "CREATE INDEX IF NOT EXISTS paths_mdline ON paths (mdline)" );
}

static void
check_table_md5( void )
{
  SQL_EXECC( "CREATE TABLE IF NOT EXISTS "
             " md5 (id INTEGER PRIMARY KEY autoincrement, md5sum1 INTEGER, md5sum2 INTEGER, dupcnt INTEGER, now REAL)" );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS md5_md5sum ON md5 (md5sum1,md5sum2)" );
  SQL_EXECC( "CREATE TABLE IF NOT EXISTS "
             " keydata (id INTEGER PRIMARY KEY autoincrement, md5id INTEGER, inode INTEGER, nameid INTEGER, pathid INTEGER, now REAL)" );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS keydata_uniq ON keydata (pathid,nameid,md5id)" );
}

int
check_tables( void )
{
  int r;
  char *errmsg;

  check_table_filedatas(  );
  check_table_filenames(  );
  check_table_paths(  );
  check_table_md5(  );
  r = sqlite3_exec( pDb,
                    "CREATE TABLE IF NOT EXISTS "
                    " exif (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, datetime INTEGER, d INTEGER, broken_date INTEGER, now REAL)", NULL, NULL,
                    &errmsg );
  SQL_EXECC( "CREATE UNIQUE INDEX IF NOT EXISTS exif_uniq ON exif (dataid)" );
  return 0;
}
