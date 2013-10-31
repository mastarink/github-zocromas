#include <stdio.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_create.h"

/*
 * filedatas
 * filenames
 * paths
 * md5
 * keydata
 * exif
 * */

static int
duf_check_table_filedatas( void )
{
  int r;

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_filedatas (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER,"
                        " size INTEGER, md5id INTEGER, ucnt INTEGER,  now REAL)", "Create duf_filedatas" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_md5id ON duf_filedatas (md5id)", "Create duf_filedatas 1" );;
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_size ON duf_filedatas (size)", "Create duf_filedatas 2" );;
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " filedatas_uniq ON duf_filedatas (dev,inode)", "Create duf_filedatas 3" );;
  return r;
}

static int
duf_check_table_filenames( void )
{
  int r;

  /* char *errmsg; */

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_filenames (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, name TEXT, pathid INTEGER, ucnt INTEGER, now REAL)",
                        "Create duf_filenames" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_filename ON duf_filenames (name)", "Create duf_filenames" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_filename ON duf_filenames (dataid)", "Create duf_filenames" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_pathid ON duf_filenames (pathid)", "Create duf_filenames" );
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " filenames_uniq ON duf_filenames (pathid,name)", "Create duf_filenames" );
  return r;
}

static int
duf_check_table_paths( void )
{
  int r;

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_paths (id INTEGER PRIMARY KEY autoincrement, dev INTEGER, inode INTEGER, items INTEGER,"
                        " md5dir1 INTEGER, md5dir2 INTEGER, dirname TEXT, parentid INTEGER, added INTEGER, ucnt INTEGER, now REAL)", "Create duf_paths" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_dirname ON duf_paths (dirname)", "Create duf_paths" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_md5sum ON duf_paths (md5dir1,md5dir2)", "Create duf_md5" );
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS paths_uniq ON duf_paths (dev,inode)", "Create duf_paths" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_inode ON duf_paths (inode)", "Create duf_paths" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_items ON duf_paths (items)", "Create duf_paths" );
  r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_parentid ON duf_paths (parentid)", "Create duf_paths" );
  return r;
}

static int
duf_check_table_md5( void )
{
  int r;

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_md5 (id INTEGER PRIMARY KEY autoincrement, md5sum1 INTEGER, md5sum2 INTEGER,"
                        " dupcnt INTEGER, size INTEGER, ucnt INTEGER, now REAL)", "Create duf_md5" );
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS md5_md5sum ON duf_md5 (md5sum1,md5sum2)", "Create duf_md5" );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_keydata (id INTEGER PRIMARY KEY autoincrement, md5id INTEGER,"
                        " inode INTEGER, nameid INTEGER, pathid INTEGER, ucnt INTEGER, now REAL)", "Create duf_keydata" );
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS keydata_uniq ON duf_keydata " "(pathid,nameid,md5id)", "Create duf_keydata" );
  return r;
}

int
duf_clear_tables( void )
{
  int r;

  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_filedatas", "Drop filedatas" );
  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_filenames", "Drop filenames" );
  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_paths", "Drop paths" );
  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_md5", "Drop md5" );
  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_keydata", "Drop keydata" );
  r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_exif", "Drop exif" );
  return r;
}

int
duf_check_tables( void )
{
  int r;

  duf_check_table_filedatas(  );
  duf_check_table_filenames(  );
  duf_check_table_paths(  );
  duf_check_table_md5(  );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_exif (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER, datetime INTEGER,"
                        " d INTEGER, broken_date INTEGER, ucnt INTEGER, now REAL)", "Create duf_exif" );
  r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS exif_uniq ON duf_exif (dataid)", "Create duf_exif" );
  return r;
}
