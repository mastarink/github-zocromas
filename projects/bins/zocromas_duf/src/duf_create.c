/* File 20140902.175935 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_defs.h"           /* */

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_utils.h"          /* */
#include "duf_dbg.h"            /* */

#include "duf_sql_defs.h"       /* */
#include "duf_sql1.h"           /* */

#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_create.h"         /* */
/* ###################################################################### */


/*
 * filedatas
 * filenames
 * fnselected
 * paths
 * md5
 * ...
 * keydata
 * exif
 * */

static int
duf_check_table_log( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBADMPREF "log ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement,"
#endif
                        " args  TEXT, restored_args  TEXT, msg  TEXT" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create log" );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filefilter( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBADMPREF "filefilter ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement," /* */
#endif
                        " type INTEGER" /* */
                        ", minsize INTEGER, maxsize INTEGER" /* */
                        ", mindups INTEGER, maxdups INTEGER" /* */
                        ", glob_include TEXT" /* */
                        ", glob_exclude TEXT" /* */
                        ", run REAL" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create filefilter" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBADMPREF "filefilter_all_uniq ON filefilter " /* */
                          " (minsize, maxsize, mindups, maxdups" /* */
                          ", glob_include, glob_exclude)", "Create filefilter" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBADMPREF "filefilter_lastupdated " /* */
                          " AFTER UPDATE OF minsize, maxsize, mindups, maxdups, glob_include, glob_exclude " /* */
                          " ON filefilter FOR EACH ROW BEGIN " /* */
                          "   UPDATE filefilter SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create filefilter" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filedatas( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "filedatas ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        "dev INTEGER NOT NULL, inode INTEGER NOT NULL" /* */
                        ", mode INTEGER NOT NULL, nlink INTEGER NOT NULL" /* */
                        ", uid INTEGER NOT NULL, gid INTEGER NOT NULL" /* */
                        ", blksize INTEGER NOT NULL, blocks INTEGER NOT NULL" /* */
                        ", size INTEGER NOT NULL" /* */
                        ", md5id INTEGER" /* */
                        ", sd5id INTEGER" /* */
                        ", crc32id INTEGER" /* */
                        ", mimeid INTEGER" /* */
                        ", exifid INTEGER" /* */
                        ", atim REAL NOT NULL, atimn INTEGER NOT NULL" /* */
                        ", mtim REAL NOT NULL, mtimn INTEGER NOT NULL" /* */
                        ", ctim REAL NOT NULL, ctimn INTEGER NOT NULL" /* */
                        ", filetype TEXT, filestatus INTEGER" /* */
                        ", last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        ", FOREIGN KEY(md5id) REFERENCES md5(" DUF_SQL_IDNAME ") " /* */
                        ", FOREIGN KEY(sd5id) REFERENCES sd5(" DUF_SQL_IDNAME ") " /* */
                        ", FOREIGN KEY(crc32id) REFERENCES crc32(" DUF_SQL_IDNAME ") " /* */
                        ", FOREIGN KEY(exifid) REFERENCES exif(" DUF_SQL_IDNAME ") " /* */
                        ", FOREIGN KEY(mimeid) REFERENCES mime(" DUF_SQL_IDNAME ") " /* */
#ifndef DUF_USE_IDCOL
                        ", FOREIGN KEY(size) REFERENCES sizes(size) " /* */
#endif
                        ")", "Create filedatas" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_sd5id ON filedatas (sd5id)", "Create filedatas 1" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_crc32id ON filedatas (crc32id)", "Create filedatas 1" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_md5id ON filedatas (md5id)", "Create filedatas 2" );
  /* if ( r >= 0 )                                                                                                                              */
  /*   r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_filestatus ON filedatas (filestatus)", "Create filedatas 3" ); */
  /* if ( r >= 0 )                                                                                                                              */
  /*   r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_filetype ON filedatas (filetype)", "Create filedatas 4" );     */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_size ON filedatas (size)", "Create filedatas 5" );
  /* if ( r >= 0 )                                                                                                                */
  /*   r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_uid ON filedatas (uid)", "Create filedatas 6" ); */
  /* if ( r >= 0 )                                                                                                                */
  /*   r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_gid ON filedatas (gid)", "Create filedatas 7" ); */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_uniq ON filedatas (dev,inode)", "Create filedatas 8" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "duf_filedatas_lastupdated " /* */
                          " AFTER UPDATE OF dev,inode,mode,nlink,uid,gid,blksize,blocks,size,sd5id,crc32id,md5id" /* */
                          ",atim,atimn,mtim,mtimn,ctim,ctimn,filetype,filestatus " /* */
                          " ON filedatas FOR EACH ROW BEGIN " /* */
                          "   UPDATE filedatas SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create filedatas" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_sizes( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "sizes ("
/* #ifdef DUF_USE_IDCOL                                                          */
/*                         DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " */
/*                         " size INTEGER NOT NULL" (* *)                        */
/* #else                                                                         */
                        " size INTEGER PRIMARY KEY NOT NULL" /* */
/* #endif */
                        ", dupzcnt INTEGER" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create sizes" );

/* #ifdef DUF_USE_IDCOL                                                                                                      */
/*   if ( r >= 0 )                                                                                                           */
/*     r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "sizes_uniq ON sizes (size)", "Create sizes" ); */
/* #endif                                                                                                                    */
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "sizes_dup ON sizes (dupzcnt)", "Create sizes" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "sizes_lastupdated " /* */
                          " AFTER UPDATE OF size, dupzcnt " /* */
                          " ON sizes FOR EACH ROW BEGIN " /* */
                          "   UPDATE sizes SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END", "Create sizes" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_mime( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "mime ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        "mime text NOT NULL, dupmimecnt INTEGER" /* */
                        /*", charset text NOT NULL, tail text" *//* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create mime" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mime_uniq ON mime (mime" /* */
                          /* ", charset" */ " )", "Create mime" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "mime_dup ON mime (dupmimecnt)", "Create mime" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "mime_lastupdated " /* */
                          " AFTER UPDATE OF mime" /* */
                          /* ", charset, tail " *//* */
                          " ON mime FOR EACH ROW BEGIN " /* */
                          "   UPDATE mime SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END", "Create mime" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_filenames( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  /* char *errmsg; */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "filenames (" /* */
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        "dataid INTEGER NOT NULL" /* */
                        ", name TEXT NOT NULL" /* */
                        ", Pathid INTEGER NOT NULL" /* */
                        ", last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        ", FOREIGN KEY(dataid) REFERENCES filedatas(" DUF_SQL_IDNAME ") " /* */
                        ", FOREIGN KEY(Pathid) REFERENCES paths(" DUF_SQL_IDNAME ") )", "Create filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_filename ON filenames (name)", "Create filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_dataid ON filenames (dataid)", "Create filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_Pathid ON filenames (Pathid)", "Create filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "filenames_uniq ON filenames (Pathid,name)", "Create filenames" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "filenames_lastupdated " /* */
                          " AFTER UPDATE OF dataid,name,Pathid ON filenames " /* */
                          " FOR EACH ROW BEGIN " /* */
                          "   UPDATE filenames SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create filenames" );




  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_fnselected( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  /* char *errmsg; */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "fnselected (" /* */
/* #ifdef DUF_USE_IDCOL */
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
/* #endif */
                        "last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
			")" /* */
			, "Create fnselected"
			);


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_paths( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "paths ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
                        "  dev INTEGER NOT NULL, inode INTEGER NOT NULL " /* */
                        ", dirname TEXT, parentid INTEGER " /* */
                        ", last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        ", FOREIGN KEY(parentid) REFERENCES paths(" DUF_SQL_IDNAME ") )", "Create paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dirname ON paths (dirname)", "Create paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dev_uniq ON paths (dev,inode)", "Create paths" );

  /* if ( r >= 0 )                                                                                                                        */
  /*   r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parent1_uniq ON paths (parentid," DUF_SQL_IDNAME ")", */
  /*                         "Create paths" );                                                                                            */

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parent2_uniq ON paths (parentid,dirname)", "Create paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dev ON paths (dev)", "Create paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_inode ON paths (inode)", "Create paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parentid ON paths (parentid)", "Create paths" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "paths_lastupdated " /* */
                          " AFTER UPDATE OF dev,inode,dirname,parentid  ON paths " /* */
                          " FOR EACH ROW BEGIN " /* */
                          "   UPDATE paths SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END", "Create paths" );




  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_pathtot_files( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "pathtot_files ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        " Pathid INTEGER NOT NULL " /* */
                        ", numfiles INTEGER, minsize INTEGER, maxsize INTEGER , last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')), FOREIGN KEY(Pathid) REFERENCES paths(" DUF_SQL_IDNAME ") )",
                        "Create pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "pathtot_files_Pathid ON pathtot_files (Pathid)", "Create pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "pathtot_files_numfiles ON pathtot_files (numfiles)", "Create pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "pathtot_files_minsize ON pathtot_files (minsize)", "Create pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "pathtot_files_maxsize ON pathtot_files (maxsize)", "Create pathtot_files" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "pathtot_files_lastupdated " /* */
                          " AFTER UPDATE OF numfiles,minsize,maxsize ON pathtot_files " /* */
                          " FOR EACH ROW BEGIN " /* */
                          "   UPDATE pathtot_files SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create pathtot_files" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_pathtot_dirs( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "pathtot_dirs ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
                        " Pathid INTEGER NOT NULL " /* */
                        ", numdirs INTEGER " /* */
                        ", mdPathid INTEGER " /* */
                        ", last_updated REAL" /* */
                        ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        ", FOREIGN KEY(Pathid) REFERENCES paths(" DUF_SQL_IDNAME ") )", "Create pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "pathtot_dirs_Pathid ON pathtot_dirs (Pathid)", "Create pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "pathtot_dirs_mdPathid ON pathtot_dirs (mdPathid)", "Create pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "pathtot_dirs_numdirs ON pathtot_dirs (numdirs)", "Create pathtot_dirs" );


  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "pathtot_dirs_lastupdated " /* */
                          " AFTER UPDATE OF Pathid,numdirs,mdPathid ON pathtot_dirs " /* */
                          " FOR EACH ROW BEGIN " /* */
                          "   UPDATE pathtot_dirs SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create pathtot_dirs" );



  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_crc32( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "crc32 " /* */
                        " ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        " crc32sum INTEGER NOT NULL, dup32cnt INTEGER" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create crc32" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "crc32_crc32sum ON crc32 (crc32sum)", "Create crc32" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "crc32_dup ON crc32 (dup32cnt)", "Create crc32" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "crc32_lastupdated " /* */
                          " AFTER UPDATE OF crc32sum ON crc32 " /* */
                          " FOR EACH ROW BEGIN    UPDATE crc32 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create crc32" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_sd5( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "sd5 " /* */
                        " ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        "sd5sum1 INTEGER NOT NULL, sd5sum2 INTEGER NOT NULL, dup2cnt INTEGER" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create sd5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "sd5_sd5sum ON sd5 (sd5sum1,sd5sum2)", "Create sd5" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "sd5_dup ON sd5 (dup2cnt)", "Create crc32" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "sd5_lastupdated " /* */
                          " AFTER UPDATE OF sd5sum1, sd5sum1 ON sd5 " /* */
                          " FOR EACH ROW BEGIN    UPDATE sd5 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create sd5" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_md5( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "md5 " /* */
                        " ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        "md5sum1 INTEGER NOT NULL, md5sum2 INTEGER NOT NULL, dup5cnt INTEGER" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create md5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "md5_dup ON md5 (dup5cnt)", "Create md5" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "md5_md5sum ON md5 (md5sum1,md5sum2)", "Create md5" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "md5_lastupdated " /* */
                          " AFTER UPDATE OF md5sum1, md5sum1 ON md5 " /* */
                          " FOR EACH ROW BEGIN    UPDATE md5 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
                          "Create md5" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* static int                                                                                                                                           */
/* duf_check_table_keydata( void )                                                                                                                      */
/* {                                                                                                                                                    */
/*   int r = DUF_ERROR_CHECK_TABLES;                                                                                                                    */
/*                                                                                                                                                      */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                                                      */
/*   r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " (* *)                                                                                          */
/*                         DUF_DBPREF "keydata (" DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement" (* *)                                                             */
/*                         ", md5id INTEGER NOT NULL" (* *)                                                                                             */
/*                         ", filenameid INTEGER NOT NULL" (* *)                                                                                        */
/*                         ", dataid INTEGER NOT NULL" (* *)                                                                                            */
/*                         ", Pathid INTEGER NOT NULL" (* *)                                                                                            */
/*                         ", last_updated REAL" (* *)                                                                                                  */
/*                         ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" (* *)                                                           */
/*                         ", FOREIGN KEY(dataid)     REFERENCES filedatas(" DUF_SQL_IDNAME ") " (* *)                                                                  */
/*                         ", FOREIGN KEY(filenameid) REFERENCES filenames(" DUF_SQL_IDNAME ") " (* *)                                                                  */
/*                         ", FOREIGN KEY(Pathid)     REFERENCES paths(" DUF_SQL_IDNAME ") " (* *)                                                                      */
/*                         ", FOREIGN KEY(md5id)      REFERENCES md5(" DUF_SQL_IDNAME ") )", "Create keydata" );                                                        */
/*   if ( r >= 0 )                                                                                                                                      */
/*     r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "keydata_uniq   ON keydata (Pathid,filenameid,md5id)", "Create keydata" ); */
/*   if ( r >= 0 )                                                                                                                                      */
/*     r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "keydata_Pathid ON keydata (Pathid)", "Create keydata" );                  */
/*   if ( r >= 0 )                                                                                                                                      */
/*     r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "keydata_nameid ON keydata (filenameid)", "Create keydata" );              */
/*   if ( r >= 0 )                                                                                                                                      */
/*     r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "keydata_dataid ON keydata (dataid)", "Create keydata" );                  */
/*   if ( r >= 0 )                                                                                                                                      */
/*     r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "keydata_md5id  ON keydata (md5id)", "Create keydata" );                   */
/*                                                                                                                                                      */
/*                                                                                                                                                      */
/*   (* if ( r >= 0 )                                                                                                                 *)                */
/*   (*   r = duf_sql_exec_msg( "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "keydata_lastupdated "                                               *)      */
/*   (*                         " AFTER UPDATE OF ... ON keydata "                                                                *)                    */
/*   (*                         " FOR EACH ROW BEGIN "                                                                                *)                */
/*   (*                         "   UPDATE keydata SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END", "Create keydata" ); *)                           */
/*                                                                                                                                                      */
/*                                                                                                                                                      */
/*   duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );                                                                                                    */
/*   return r;                                                                                                                                          */
/* }                                                                                                                                                    */

static int
duf_check_table_mdpath( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "mdpath ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
                        "  mdpathsum1 INTEGER NOT NULL, mdpathsum2 INTEGER NOT NULL " /* */
                        ", duppcnt INTEGER" /* */
                        ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create mdpath" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_mdpathsum ON mdpath (mdpathsum1,mdpathsum2)", "Create mdpath" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_dup ON mdpath (duppcnt)", "Create mdpath" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_exif_model( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "exif_model ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        " model TEXT, " /* */
                        "  inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        /* " , FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES exif(modelid) " */
                        " )", "Create exif_model" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "exif_model_model ON exif_model (model)", "Create exif_model" );


  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_exif( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " /* */
                        DUF_DBPREF "exif ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        " modelid INTEGER , date_time REAL" /* */
                        ", dupexifcnt INTEGER" /* */
                        ", broken_date TEXT, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        /* ", FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES filedatas(exifid) " */
                        " )", "Create exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "exif_m_d ON exif (modelid, date_time)", "Create exif" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "exif_m_id ON exif (modelid)", "Create exif" );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "exif_dup ON exif (dupexifcnt)", "Create exif" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}


static int
duf_check_table_tags( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "tags " /* */
                        "("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
                        " name TEXT NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))", "Create tags" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "tags_uniq ON tags (name)", "Create tags" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

static int
duf_check_table_path_tags( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  r = duf_sql_exec_msg( "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "path_tags ("
#ifdef DUF_USE_IDCOL
                        DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement,"
#endif
                        " tagsid INTEGER NOT NULL, " /* */
                        " Pathid INTEGER NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
                        " , FOREIGN KEY(Pathid)  REFERENCES paths(" DUF_SQL_IDNAME ") "
                        " , FOREIGN KEY(tagsid) REFERENCES tags(" DUF_SQL_IDNAME ") )", "Create path_tags" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "path_tags_uniq ON path_tags (tagsid, Pathid)", "Create path_tags" );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}


int
duf_clear_tables( void )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS log", "Drop log" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS filefilter", "Drop filefilter" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "filedatas", "Drop filedatas" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "sizes", "Drop sizes" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "filenames", "Drop filenames" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "paths", "Drop paths" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "pathtot_dirs", "Drop pathtot_dirs" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "pathtot_files", "Drop pathtot_files" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "sd5", "Drop sd5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "crc32", "Drop crc32" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "md5", "Drop md5" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "mdpath", "Drop mdpath" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "keydata", "Drop keydata" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "mime", "Drop mime" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "exif_model", "Drop exif_model" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "exif", "Drop exif" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "tags", "Drop tags" );
  if ( r >= 0 )
    r = duf_sql_exec_msg( "DROP TABLE IF EXISTS " DUF_DBPREF "path_tags", "Drop path_tags" );
  DUF_TRACE( action, 0, "Drop all tables from DB %s (%d)", r < 0 ? "FAIL" : "OK", r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

typedef int duf_create_table_function( void );
typedef struct
{
  const char *name;
  duf_create_table_function *fun;
} duf_create_table_element;

#define DUF_TABLE_ELEMENT(n) { .name= # n, .fun= duf_check_table_ ## n }
static duf_create_table_element tab_table[] = {
  DUF_TABLE_ELEMENT( log ),
  DUF_TABLE_ELEMENT( paths ),
  DUF_TABLE_ELEMENT( filenames ),
  DUF_TABLE_ELEMENT( fnselected ),
  DUF_TABLE_ELEMENT( filedatas ),
  DUF_TABLE_ELEMENT( sizes ),
  DUF_TABLE_ELEMENT( filefilter ),
  DUF_TABLE_ELEMENT( mime ),
  DUF_TABLE_ELEMENT( crc32 ),
  DUF_TABLE_ELEMENT( sd5 ),
  DUF_TABLE_ELEMENT( md5 ),
  DUF_TABLE_ELEMENT( mdpath ),
  DUF_TABLE_ELEMENT( exif_model ),
  DUF_TABLE_ELEMENT( exif ),
  DUF_TABLE_ELEMENT( tags ),
  DUF_TABLE_ELEMENT( path_tags ),

  /* DUF_TABLE_ELEMENT( keydata ), */
  DUF_TABLE_ELEMENT( pathtot_dirs ),
  DUF_TABLE_ELEMENT( pathtot_files ),
};

int
duf_check_tables( void )
{
  int r = DUF_ERROR_CHECK_TABLES;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  {
    int ntabs;

    ntabs = sizeof( tab_table ) / sizeof( tab_table[0] );
    for ( int itab = 0; itab < ntabs; itab++ )
    {
      if ( DUF_CLI_FLAG( dry_run ) )
      {
        DUF_PRINTF( 0, "%2d of %2d; %s : action '%s' : table '%s'", itab, ntabs, DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME( CREATE_TABLES ),
                    tab_table[itab].name );
        r = 0;
      }
      else
        r = ( tab_table[itab].fun ) (  );
      DUF_TRACE( action, 0, "Created table '%s'\t\t%s (%d)", tab_table[itab].name, r < 0 ? "FAIL" : "OK", r );
      /* DUF_PRINTF( 0, "r:%d", r ); */
      if ( r < 0 )
        break;
    }
  }
  DUF_TRACE( action, 0, "Create all tables for DB %s (%d)", r < 0 ? "FAIL" : "OK", r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
