#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

#include "sql_beginning_tables.h"
/* ###################################################################### */
#include "sql_beginning_create.h"
/* ###################################################################### */

/*
crc32
exif
exif_model
filedatas
filefilter
filenames
log
md5
mdpath
mime
path_pairs
paths
sd5
sizes
tags
tagnames
*/

duf_sql_sequence_t sql_beginning_clear = /* */
{
  .name = "clear (drop tables)",
  .done = 0,
  .sql = {
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_CRC32_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_EXIF_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL,
#if 1
          "DROP TABLE IF EXISTS filefilter",
#endif
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_FILENAMES_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_MD5_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SHA1_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_MIME_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_PATHS_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SD5_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SIZES_FULL,
          "DROP TABLE IF EXISTS tags",
          "DROP TABLE IF EXISTS tagnames",
          "DROP TABLE IF EXISTS log",
          "DROP TABLE IF EXISTS path_pairs",
          "DROP TABLE IF EXISTS mdpath",
          NULL}
};

duf_sql_sequence_t sql_beginning_create = {
  .name = "create (main tables)",
  .done = 0,
  .beginend = 1,
  .sql = {
          "CREATE TABLE IF NOT EXISTS " DUF_DBADMPREF "log ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement,"
#endif
          " args  TEXT, restored_args  TEXT, msg  TEXT" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
#if 0
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBADMPREF "filefilter ("
#  ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement," /* */
#  endif
          " type INTEGER"       /* */
          ", minsize INTEGER, maxsize INTEGER" /* */
          ", mindups INTEGER, maxdups INTEGER" /* */
          ", glob_include TEXT" /* */
          ", glob_exclude TEXT" /* */
          ", run REAL"          /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBADMPREF "filefilter_all_uniq ON filefilter " /* */
          " (minsize, maxsize, mindups, maxdups" /* */
          ", glob_include, glob_exclude)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_DBADMPREF "filefilter_lastupdated " /* */
          " AFTER UPDATE OF minsize, maxsize, mindups, maxdups, glob_include, glob_exclude " /* */
          " ON filefilter FOR EACH ROW BEGIN " /* */
          "   UPDATE filefilter SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
#endif

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "mime text NOT NULL, dupmimecnt INTEGER" /* */
          /*", charset text NOT NULL, tail text" *//* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_uniq ON " DUF_SQL_TABLES_MIME " (mime" /* */
          /* ", charset" */ " )",

          "CREATE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_dup ON " DUF_SQL_TABLES_MIME " (dupmimecnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_lastupdated " /* */
          " AFTER UPDATE OF " DUF_SQL_TABLES_MIME /* */
          /* ", charset, tail " *//* */
          " ON " DUF_SQL_TABLES_MIME " FOR EACH ROW BEGIN " /* */
          "   UPDATE " DUF_SQL_TABLES_MIME " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",
	  
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dev INTEGER NOT NULL, rdev INTEGER, inode INTEGER NOT NULL" /* */
          ", mode INTEGER NOT NULL " /* */
          ", nlink INTEGER NOT NULL" /* */
          ", uid INTEGER NOT NULL, gid INTEGER NOT NULL" /* */
          ", blksize INTEGER NOT NULL, blocks INTEGER NOT NULL" /* */
          ", size INTEGER NOT NULL" /* */
          ", atim REAL NOT NULL, atimn INTEGER NOT NULL" /* */
          ", mtim REAL NOT NULL, mtimn INTEGER NOT NULL" /* */
          ", ctim REAL NOT NULL, ctimn INTEGER NOT NULL" /* */
          ", md5id INTEGER"     /* */
          ", sha1id INTEGER"    /* */
          ", sd5id INTEGER"     /* */
          ", crc32id INTEGER"   /* */
          ", mimeid INTEGER"    /* */
          ", noexif INTEGER"    /* */
          ", exifid INTEGER"    /* */
          ", filetype TEXT, filestatus INTEGER" /* */
          ", dupdatacnt INTEGER " /* */
          ", last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          ", FOREIGN KEY(md5id)   REFERENCES " DUF_SQL_TABLES_MD5 "   (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(sha1id)  REFERENCES " DUF_SQL_TABLES_SHA1 "  (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(sd5id)   REFERENCES " DUF_SQL_TABLES_SD5 "   (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(crc32id) REFERENCES " DUF_SQL_TABLES_CRC32 " (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(exifid)  REFERENCES " DUF_SQL_TABLES_EXIF "  (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(mimeid)  REFERENCES " DUF_SQL_TABLES_MIME "  (" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(size)    REFERENCES " DUF_SQL_TABLES_SIZES " ( size ) " /* */
          ")",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_md5id      ON " DUF_SQL_TABLES_FILEDATAS " (md5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sha1id     ON " DUF_SQL_TABLES_FILEDATAS " (sha1id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sha1md5id  ON " DUF_SQL_TABLES_FILEDATAS " (sha1id,md5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sd5id      ON " DUF_SQL_TABLES_FILEDATAS " (sd5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_crc32id    ON " DUF_SQL_TABLES_FILEDATAS " (crc32id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_exifid     ON " DUF_SQL_TABLES_FILEDATAS " (exifid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_mimeid     ON " DUF_SQL_TABLES_FILEDATAS " (mimeid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_size       ON " DUF_SQL_TABLES_FILEDATAS " (size)",

/*        "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_filestatus ON " DUF_SQL_TABLES_FILEDATAS " (filestatus)", */
/*        "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_filetype   ON " DUF_SQL_TABLES_FILEDATAS " (filetype)", */
/*        "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_uid        ON " DUF_SQL_TABLES_FILEDATAS " (uid)", */
/*        "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_gid        ON " DUF_SQL_TABLES_FILEDATAS " (gid)", */
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_dup        ON " DUF_SQL_TABLES_FILEDATAS " (dupdatacnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_uniq       ON " DUF_SQL_TABLES_FILEDATAS " (dev,inode)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_lastupdated " /* */
          " AFTER UPDATE OF dev,inode,mode,nlink,uid,gid,blksize,blocks,size,sd5id,crc32id,md5id" /* */
          ",atim,atimn,mtim,mtimn,ctim,ctimn,filetype,filestatus " /* */
          " ON " DUF_SQL_TABLES_FILEDATAS " FOR EACH ROW BEGIN " /* */
          "   UPDATE " DUF_SQL_TABLES_FILEDATAS " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL " (" /* */
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dataid INTEGER NOT NULL" /* */
          ", name TEXT NOT NULL" /* */
          ", Pathid INTEGER NOT NULL" /* */
          ", last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          ", FOREIGN KEY(dataid) REFERENCES " DUF_SQL_TABLES_FILEDATAS "(" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(Pathid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDNAME ") )",

          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_filename ON " DUF_SQL_TABLES_FILENAMES " (name)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_dataid   ON " DUF_SQL_TABLES_FILENAMES " (dataid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_Pathid   ON " DUF_SQL_TABLES_FILENAMES " (Pathid)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_uniq     ON " DUF_SQL_TABLES_FILENAMES " (Pathid,name)",


          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_lastupdated " /* */
          " AFTER UPDATE OF dataid,name,Pathid ON " DUF_SQL_TABLES_FILENAMES /* */
          " FOR EACH ROW BEGIN " /* */
          "   UPDATE " DUF_SQL_TABLES_FILENAMES " SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
          "  dev INTEGER NOT NULL, inode INTEGER NOT NULL " /* */
          ", rdev INTEGER"      /* */
          ", mode INTEGER, nlink INTEGER" /* */
          ", uid INTEGER, gid INTEGER" /* */
          ", blksize INTEGER, blocks INTEGER" /* */
          ", size INTEGER"      /* */
          ", atim REAL, atimn INTEGER" /* */
          ", mtim REAL, mtimn INTEGER" /* */
          ", ctim REAL, ctimn INTEGER" /* */
          ", dirname TEXT, parentid INTEGER " /* */
          ", last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          /* ", FOREIGN KEY(parentid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDNAME ") " (* *) */
	  " ) "
          ,
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_dirname      ON " DUF_SQL_TABLES_PATHS " (dirname)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_dev_uniq     ON " DUF_SQL_TABLES_PATHS " (dev,inode)",

/*        "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parent1_uniq ON " DUF_SQL_TABLES_PATHS " (parentid," DUF_SQL_IDNAME ")", */

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parent2_uniq ON " DUF_SQL_TABLES_PATHS " (parentid,dirname)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_dev          ON " DUF_SQL_TABLES_PATHS " (dev)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_inode        ON " DUF_SQL_TABLES_PATHS " (inode)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parentid     ON " DUF_SQL_TABLES_PATHS " (parentid)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_lastupdated " /* */
          " AFTER UPDATE OF dev,inode,dirname,parentid  ON " DUF_SQL_TABLES_PATHS /* */
          " FOR EACH ROW BEGIN " /* */
          "   UPDATE " DUF_SQL_TABLES_PATHS " SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL " ("
/* #ifdef DUF_USE_IDCOL                                                          */
/*                         DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " */
/*                         " size INTEGER NOT NULL" (* *)                        */
/* #else                                                                         */
          " size INTEGER PRIMARY KEY NOT NULL" /* */
/* #endif */
          ", dupzcnt INTEGER"   /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

/* #ifdef DUF_USE_IDCOL                                                                                                      */
/*        "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_uniq ON "DUF_SQL_TABLES_SIZES" (size)", */
/* #endif                                                                                                                    */
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_dup  ON " DUF_SQL_TABLES_SIZES " (dupzcnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_lastupdated " /* */
          " AFTER UPDATE OF size, dupzcnt " /* */
          " ON " DUF_SQL_TABLES_SIZES " FOR EACH ROW BEGIN " /* */
          "   UPDATE " DUF_SQL_TABLES_SIZES " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",


/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " crc32sum INTEGER NOT NULL, dup32cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_crc32sum ON " DUF_SQL_TABLES_CRC32 " (crc32sum)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_dup      ON " DUF_SQL_TABLES_CRC32 " (dup32cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_lastupdated " /* */
          " AFTER UPDATE OF crc32sum ON " DUF_SQL_TABLES_CRC32 /* */
          " FOR EACH ROW BEGIN " /* */
          "    UPDATE " DUF_SQL_TABLES_CRC32 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "sd5sum1 INTEGER NOT NULL, sd5sum2 INTEGER NOT NULL, dup2cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_sd5sum ON " DUF_SQL_TABLES_SD5 " (sd5sum1,sd5sum2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_dup    ON " DUF_SQL_TABLES_SD5 " (dup2cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_lastupdated " /* */
          " AFTER UPDATE OF sd5sum1, sd5sum1 ON " DUF_SQL_TABLES_SD5 /* */
          " FOR EACH ROW BEGIN " /* */
          "    UPDATE " DUF_SQL_TABLES_SD5 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "md5sum1 INTEGER NOT NULL, md5sum2 INTEGER NOT NULL, dup5cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_dup    ON " DUF_SQL_TABLES_MD5 " (dup5cnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_md5sum ON " DUF_SQL_TABLES_MD5 " (md5sum1,md5sum2)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_lastupdated " /* */
          " AFTER UPDATE OF md5sum1, md5sum2 ON " DUF_SQL_TABLES_MD5 /* */
          " FOR EACH ROW BEGIN " /* */
          "    UPDATE " DUF_SQL_TABLES_MD5 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "sha1sum1 INTEGER NOT NULL, sha1sum2 INTEGER NOT NULL, sha1sum3 INTEGER NOT NULL, dupsha1cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_dup    ON " DUF_SQL_TABLES_SHA1 " (dupsha1cnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_sha1sum ON " DUF_SQL_TABLES_SHA1 " (sha1sum1,sha1sum2,sha1sum3)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_lastupdated " /* */
          " AFTER UPDATE OF sha1sum1, sha1sum2, sha1sum3 ON " DUF_SQL_TABLES_SHA1 /* */
          " FOR EACH ROW BEGIN " /* */
          "    UPDATE " DUF_SQL_TABLES_SHA1 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "mdpath ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
          "  mdpathsum1 INTEGER NOT NULL, mdpathsum2 INTEGER NOT NULL " /* */
          ", duppcnt INTEGER"   /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_mdpathsum ON mdpath (mdpathsum1,mdpathsum2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_dup       ON mdpath (duppcnt)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " model TEXT, "       /* */
          "  inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          /* " , FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES "DUF_SQL_TABLES_EXIF"(modelid) " */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL "_model ON " DUF_SQL_TABLES_EXIF_MODEL " (model)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " modelid INTEGER , date_time REAL" /* */
          ", dupexifcnt INTEGER" /* */
          ", fixed INTEGER"     /* */
          ", broken_date TEXT " /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          /* ", FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES " DUF_SQL_TABLES_FILEDATAS "(exifid) " */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_m_d  ON " DUF_SQL_TABLES_EXIF " (modelid, date_time)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_m_id ON " DUF_SQL_TABLES_EXIF " (modelid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_dup  ON " DUF_SQL_TABLES_EXIF " (dupexifcnt)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "tagnames " /* */
          "("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " name TEXT NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "tagnames_uniq ON tagnames (name)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "tags ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement,"
#endif
          " tagnameid INTEGER NOT NULL, " /* */
          " itemtype TEXT NOT NULL, " /* */
          " itemid INTEGER NOT NULL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')) " /* */
          /*          " , FOREIGN KEY(itemid)  REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDNAME ") " *//* */
          " , FOREIGN KEY(tagnameid) REFERENCES tagnames(" DUF_SQL_IDNAME ") " /* */
          " , FOREIGN KEY(itemid) REFERENCES " DUF_SQL_TABLES_FILENAMES "(" DUF_SQL_IDNAME ") " /* */
          " , FOREIGN KEY(itemid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDNAME ") " /* */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "tags_uniq ON tags (tagnameid, itemid)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "path_pairs ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " Pathid1 INTEGER NOT NULL " /* */
          ", Pathid2 INTEGER NOT NULL " /* */
          ", samefiles INTEGER, last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')) )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_uniq      ON  path_pairs (Pathid1, Pathid2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid1   ON path_pairs (Pathid1)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid2   ON path_pairs (Pathid2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_samefiles ON path_pairs (samefiles)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE  VIEW  IF NOT EXISTS " DUF_DBPREF "v_selected_filenames AS  SELECT fn.rowid AS rowid, fn.rowid AS nameid  " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd.rowid) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md.rowid=fd.md5id) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (x.rowid=fd.exifid) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm.rowid) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL " AS mi ON( fd.mimeid = mi.rowid )",

          NULL}
};
