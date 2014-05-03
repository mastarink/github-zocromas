#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"
#include "duf_dbg.h"

#include "duf_sql1.h"


/* ###################################################################### */
#include "duf_create.h"
/* ###################################################################### */


/*
 * filedatas
 * filenames
 * paths
 * md5
 * keydata
 * exif
 * */

static int
duf_check_table_log( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS "
                        " duf_log (id INTEGER PRIMARY KEY autoincrement " ", args  TEXT" ", restored_args  TEXT" ", msg  TEXT"
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")", "Create duf_log" );
  /* *INDENT-OFF*  */
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filedatas( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS "
                        " duf_filedatas (id INTEGER PRIMARY KEY autoincrement, dev INTEGER NOT NULL, inode INTEGER NOT NULL"
                        ", mode INTEGER NOT NULL, nlink INTEGER NOT NULL"
			", uid INTEGER NOT NULL, gid INTEGER NOT NULL"
			", blksize INTEGER NOT NULL, blocks INTEGER NOT NULL"
                        ", size INTEGER NOT NULL, md5id INTEGER NOT NULL"
                        ", atim INTEGER NOT NULL, atimn INTEGER NOT NULL"
			", mtim INTEGER NOT NULL, mtimn INTEGER NOT NULL"
			", ctim INTEGER NOT NULL, ctimn INTEGER NOT NULL"
			", filetype TEXT, filestatus INTEGER"
                        ", last_updated REAL"
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(md5id) REFERENCES duf_md5(id) "
			")", "Create duf_filedatas" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_md5id ON duf_filedatas (md5id)", "Create duf_filedatas 1" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_filestatus ON duf_filedatas (filestatus)", "Create duf_filedatas 2" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_filetype ON duf_filedatas (filetype)", "Create duf_filedatas 3" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_size ON duf_filedatas (size)", "Create duf_filedatas 4" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_uid ON duf_filedatas (uid)", "Create duf_filedatas 5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filedatas_gid ON duf_filedatas (gid)", "Create duf_filedatas 6" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " filedatas_uniq ON duf_filedatas (dev,inode)", "Create duf_filedatas 7" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_filedatas_lastupdated "
                          " AFTER UPDATE OF dev,inode,mode,nlink,uid,gid,blksize,blocks,size,md5id,atim,atimn,mtim,mtimn,ctim,ctimn,filetype,filestatus "
                          " ON duf_filedatas FOR EACH ROW BEGIN "
                          "   UPDATE duf_filedatas SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_filedatas" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_sizes( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS "
                        " duf_sizes (id INTEGER PRIMARY KEY autoincrement, size INTEGER NOT NULL, dupcnt INTEGER"
                        ", last_updated REAL"
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")", "Create duf_sizes" );
  /* *INDENT-ON*  */

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " sizes_uniq ON duf_sizes (size)", "Create duf_sizes" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_sizes_lastupdated "
                          " AFTER UPDATE OF size, dupcnt "
                          " ON duf_sizes FOR EACH ROW BEGIN "
                          "   UPDATE duf_sizes SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_sizes" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filenames( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  /* char *errmsg; */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_filenames (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER NOT NULL"
			", name TEXT NOT NULL"
                        ", Pathid INTEGER NOT NULL"
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(dataid) REFERENCES duf_filedatas(id) "
                        ", FOREIGN KEY(Pathid) REFERENCES duf_paths(id) "
			")", "Create duf_filenames" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_filename ON duf_filenames (name)", "Create duf_filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_dataid ON duf_filenames (dataid)", "Create duf_filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS" " filenames_Pathid ON duf_filenames (Pathid)", "Create duf_filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " filenames_uniq ON duf_filenames (Pathid,name)", "Create duf_filenames" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_filenames_lastupdated "
                          " AFTER UPDATE OF dataid,name,Pathid ON duf_filenames "
                          " FOR EACH ROW BEGIN "
                          "   UPDATE duf_filenames SET last_updated=DATETIME() " " WHERE id=OLD.id ;" " END", "Create duf_filenames" );




  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_paths( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_paths (id INTEGER PRIMARY KEY autoincrement"
			", dev INTEGER NOT NULL, inode INTEGER NOT NULL "
                        ", dirname TEXT, parentid INTEGER "
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(parentid) REFERENCES duf_paths(id) "
			")", "Create duf_paths" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_dirname ON duf_paths (dirname)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS paths_dev_uniq ON duf_paths (dev,inode)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS paths_parent1_uniq ON duf_paths (parentid,id)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS paths_parent2_uniq ON duf_paths (parentid,dirname)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_dev ON duf_paths (dev)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_inode ON duf_paths (inode)", "Create duf_paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS paths_parentid ON duf_paths (parentid)", "Create duf_paths" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_paths_lastupdated "
                          " AFTER UPDATE OF dev,inode,dirname,parentid  ON duf_paths "
                          " FOR EACH ROW BEGIN "
                          "   UPDATE duf_paths SET last_updated=DATETIME() " " WHERE id=OLD.id ;" " END", "Create duf_paths" );




  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_pathtot_files( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_pathtot_files (id INTEGER PRIMARY KEY autoincrement, Pathid INTEGER NOT NULL "
                        ", numfiles INTEGER, minsize INTEGER, maxsize INTEGER "
                        ", last_updated REAL"
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(Pathid) REFERENCES duf_paths(id) "
			")", "Create duf_pathtot_files" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS pathtot_files_Pathid ON duf_pathtot_files (Pathid)",
                          "Create duf_pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS pathtot_files_numfiles ON duf_pathtot_files (numfiles)", "Create duf_pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS pathtot_files_minsize ON duf_pathtot_files (minsize)", "Create duf_pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS pathtot_files_maxsize ON duf_pathtot_files (maxsize)", "Create duf_pathtot_files" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_pathtot_files_lastupdated "
                          " AFTER UPDATE OF numfiles,minsize,maxsize ON duf_pathtot_files "
                          " FOR EACH ROW BEGIN "
                          "   UPDATE duf_pathtot_files SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_pathtot_files" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_pathtot_dirs( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_pathtot_dirs (id INTEGER PRIMARY KEY autoincrement"
			", Pathid INTEGER NOT NULL "
                        ", numdirs INTEGER "
			", mdPathid INTEGER "
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(Pathid) REFERENCES duf_paths(id) "
			")", "Create duf_pathtot_dirs" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS pathtot_dirs_Pathid ON duf_pathtot_dirs (Pathid)", "Create duf_pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS pathtot_dirs_mdPathid ON duf_pathtot_dirs (mdPathid)", "Create duf_pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS pathtot_dirs_numdirs ON duf_pathtot_dirs (numdirs)", "Create duf_pathtot_dirs" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_pathtot_dirs_lastupdated "
                          " AFTER UPDATE OF Pathid,numdirs,mdPathid ON duf_pathtot_dirs "
                          " FOR EACH ROW BEGIN "
                          "   UPDATE duf_pathtot_dirs SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_pathtot_dirs" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_md5( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_md5 (id INTEGER PRIMARY KEY autoincrement, md5sum1 INTEGER NOT NULL, md5sum2 INTEGER NOT NULL"
                        ", dupcnt INTEGER"
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")", "Create duf_md5" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS md5_md5sum ON duf_md5 (md5sum1,md5sum2)", "Create duf_md5" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_md5_lastupdated "
                          " AFTER UPDATE OF md5sum1, md5sum1 ON duf_md5 "
                          " FOR EACH ROW BEGIN "
                          "   UPDATE duf_md5 SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_md5" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_keydata( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_keydata (id INTEGER PRIMARY KEY autoincrement"
			", md5id INTEGER NOT NULL"
                        ", filenameid INTEGER NOT NULL"
			", dataid INTEGER NOT NULL"
			", Pathid INTEGER NOT NULL"
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        ", FOREIGN KEY(dataid) REFERENCES duf_filedatas(id) "
                        ", FOREIGN KEY(filenameid) REFERENCES duf_filenames(id) "
                        ", FOREIGN KEY(Pathid) REFERENCES duf_paths(id) "
                        ", FOREIGN KEY(md5id) REFERENCES duf_md5(id) "
			")",
                        "Create duf_keydata" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS keydata_uniq ON duf_keydata " " (Pathid,filenameid,md5id)",
                          "Create duf_keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS keydata_Pathid ON duf_keydata " "(Pathid)", "Create duf_keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS keydata_nameid ON duf_keydata " "(filenameid)", "Create duf_keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS keydata_dataid ON duf_keydata " "(dataid)", "Create duf_keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS keydata_md5id ON duf_keydata " "(md5id)", "Create duf_keydata" );


  /* if ( r >= 0 )                                                                                                                 */
  /*   r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_keydata_lastupdated "                                               */
  /*                         " AFTER UPDATE OF ... ON duf_keydata "                                                                */
  /*                         " FOR EACH ROW BEGIN "                                                                                */
  /*                         "   UPDATE duf_keydata SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_keydata" ); */


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_mdpath( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_mdpath (id INTEGER PRIMARY KEY autoincrement "
			", mdpathsum1 INTEGER NOT NULL, mdpathsum2 INTEGER NOT NULL "
                        ", dupcnt INTEGER"
                        ", last_updated REAL"
			", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")", "Create duf_mdpath" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS mdpath_mdpathsum ON duf_mdpath (mdpathsum1,mdpathsum2)", "Create duf_mdpath" );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_exif( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_exif (id INTEGER PRIMARY KEY autoincrement, dataid INTEGER NOT NULL, model TEXT, datetime INTEGER,"
                        " d INTEGER, broken_date TEXT, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        " , FOREIGN KEY(dataid) REFERENCES duf_filedatas(id) "
			")", "Create duf_exif" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS exif_uniq ON duf_exif (dataid)", "Create duf_exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS exif_model ON duf_exif (model)", "Create duf_exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS exif_broken_date ON duf_exif (broken_date)", "Create duf_exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS exif_datetime ON duf_exif (datetime)", "Create duf_exif" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_group( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS"
                        " duf_group (id INTEGER PRIMARY KEY autoincrement, name TEXT NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")",
                        "Create duf_group" );
  /* *INDENT-ON*  */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS group_uniq ON duf_group (name)", "Create duf_group" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_path_group( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS" " duf_path_group (id INTEGER PRIMARY KEY autoincrement, groupid INTEGER NOT NULL, "
                        " Pathid INTEGER NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
                        " , FOREIGN KEY(Pathid) REFERENCES duf_paths(id) "
                        " , FOREIGN KEY(groupid) REFERENCES duf_group(id) "
                     ")", "Create duf_path_group" );
  /* *INDENT-ON* */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS path_group_uniq ON duf_path_group (groupid, Pathid)",
                          "Create duf_path_group" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filefilter( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* *INDENT-OFF*  */
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS "
                        " duf_filefilter (id INTEGER PRIMARY KEY autoincrement"
			", type INTEGER"
			", minsize INTEGER, maxsize INTEGER"
			", mindups INTEGER, maxdups INTEGER"
			", nameglob TEXT"
                        ", run REAL"
                        ", last_updated REAL"
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))"
			")", "Create duf_filefilter" );
  /* *INDENT-ON*  */

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS" " all_uniq ON duf_filefilter (minsize, maxsize, mindups, maxdups, nameglob)",
                          "Create duf_filefilter" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS duf_sizes_lastupdated "
                          " AFTER UPDATE OF minsize, maxsize, mindups, maxdups, nameglob "
                          " ON duf_filefilter FOR EACH ROW BEGIN "
                          "   UPDATE duf_filefilter SET last_updated=DATETIME() WHERE id=OLD.id ;" " END", "Create duf_filefilter" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}


int
duf_clear_tables( void )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_log", "Drop log" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_filedatas", "Drop filedatas" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_sizes", "Drop sizes" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_filenames", "Drop filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_paths", "Drop paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS pathtot_dirs", "Drop pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS pathtot_files", "Drop pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_md5", "Drop md5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_mdpath", "Drop mdpath" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_keydata", "Drop keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_exif", "Drop exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_group", "Drop group" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_path_group", "Drop path_group" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS duf_filefilter", "Drop filefilter" );
  DUF_TRACE( action, 0, "Drop all tables from DB %s (%d)", r < 0 ? "FAIL" : "OK", r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_check_tables( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_check_table_log(  );
  if ( r >= 0 )
    r = duf_check_table_filedatas(  );
  if ( r >= 0 )
    r = duf_check_table_sizes(  );
  if ( r >= 0 )
    r = duf_check_table_filenames(  );
  if ( r >= 0 )
    r = duf_check_table_paths(  );
  if ( r >= 0 )
    r = duf_check_table_pathtot_dirs(  );
  if ( r >= 0 )
    r = duf_check_table_pathtot_files(  );
  if ( r >= 0 )
    r = duf_check_table_md5(  );
  if ( r >= 0 )
    r = duf_check_table_mdpath(  );
  if ( r >= 0 )
    r = duf_check_table_keydata(  );
  if ( r >= 0 )
    r = duf_check_table_exif(  );
  if ( r >= 0 )
    r = duf_check_table_group(  );
  if ( r >= 0 )
    r = duf_check_table_path_group(  );
  if ( r >= 0 )
    r = duf_check_table_filefilter(  );
  DUF_TRACE( action, 0, "Create all tables for DB %s (%d)", r < 0 ? "FAIL" : "OK", r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
