#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

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

duf_sql_sequence_t sql_beginning_clear = {.done = 0,
  .sql = {
          "DROP TABLE IF EXISTS crc32",
          "DROP TABLE IF EXISTS exif",
          "DROP TABLE IF EXISTS exif_model",
          "DROP TABLE IF EXISTS filedatas",
          "DROP TABLE IF EXISTS filefilter",
          "DROP TABLE IF EXISTS filenames",
          "DROP TABLE IF EXISTS log",
          "DROP TABLE IF EXISTS md5",
          "DROP TABLE IF EXISTS mdpath",
          "DROP TABLE IF EXISTS mime",
          "DROP TABLE IF EXISTS path_pairs",
          "DROP TABLE IF EXISTS paths",
          "DROP TABLE IF EXISTS sd5",
          "DROP TABLE IF EXISTS sizes",
          "DROP TABLE IF EXISTS tags",
          "DROP TABLE IF EXISTS tagnames",
          NULL}
};

duf_sql_sequence_t sql_beginning_create = {.done = 0,
  .sql = {
          "BEGIN" /* */ ,
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBADMPREF "log ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement,"
#endif
          " args  TEXT, restored_args  TEXT, msg  TEXT" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBADMPREF "filefilter ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement," /* */
#endif
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

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "filedatas ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dev INTEGER NOT NULL, rdev INTEGER, inode INTEGER NOT NULL" /* */
          ", mode INTEGER NOT NULL, nlink INTEGER NOT NULL" /* */
          ", uid INTEGER NOT NULL, gid INTEGER NOT NULL" /* */
          ", blksize INTEGER NOT NULL, blocks INTEGER NOT NULL" /* */
          ", size INTEGER NOT NULL" /* */

          ", atim REAL NOT NULL, atimn INTEGER NOT NULL" /* */
          ", mtim REAL NOT NULL, mtimn INTEGER NOT NULL" /* */
          ", ctim REAL NOT NULL, ctimn INTEGER NOT NULL" /* */

          ", md5id INTEGER"     /* */
          ", sd5id INTEGER"     /* */
          ", crc32id INTEGER"   /* */
          ", mimeid INTEGER"    /* */
          ", exifid INTEGER"    /* */
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
          ")",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_exifid ON filedatas (exifid)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_mimeid ON filedatas (mimeid)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_sd5id ON filedatas (sd5id)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_crc32id ON filedatas (crc32id)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_md5id ON filedatas (md5id)",
/*"CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_filestatus ON filedatas (filestatus)", */
/*"CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_filetype ON filedatas (filetype)", */
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_size ON filedatas (size)",
/*"CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_uid ON filedatas (uid)", */
/*"CREATE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_gid ON filedatas (gid)", */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "filedatas_uniq ON filedatas (dev,inode)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "duf_filedatas_lastupdated " /* */
          " AFTER UPDATE OF dev,inode,mode,nlink,uid,gid,blksize,blocks,size,sd5id,crc32id,md5id" /* */
          ",atim,atimn,mtim,mtimn,ctim,ctimn,filetype,filestatus " /* */
          " ON filedatas FOR EACH ROW BEGIN " /* */
          "   UPDATE filedatas SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "filenames (" /* */
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dataid INTEGER NOT NULL" /* */
          ", name TEXT NOT NULL" /* */
          ", Pathid INTEGER NOT NULL" /* */
          ", last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          ", FOREIGN KEY(dataid) REFERENCES filedatas(" DUF_SQL_IDNAME ") " /* */
          ", FOREIGN KEY(Pathid) REFERENCES paths(" DUF_SQL_IDNAME ") )",
          "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_filename ON filenames (name)",
          "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_dataid ON filenames (dataid)",
          "CREATE INDEX IF NOT EXISTS        " DUF_DBPREF "filenames_Pathid ON filenames (Pathid)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "filenames_uniq ON filenames (Pathid,name)",


          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "filenames_lastupdated " /* */
          " AFTER UPDATE OF dataid,name,Pathid ON filenames " /* */
          " FOR EACH ROW BEGIN " /* */
          "   UPDATE filenames SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "paths ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
          "  dev INTEGER NOT NULL, inode INTEGER NOT NULL " /* */
          ", rdev INTEGER"      /* */
          ", mode INTEGER, nlink INTEGER" /* */
          ", uid INTEGER, gid INTEGER" /* */
          ", blksize INTEGER, blocks INTEGER" /* */
          ", size INTEGER" /* */
          
	  ", atim REAL, atimn INTEGER" /* */
          ", mtim REAL, mtimn INTEGER" /* */
          ", ctim REAL, ctimn INTEGER" /* */

          ", dirname TEXT, parentid INTEGER " /* */
          ", last_updated REAL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          ", FOREIGN KEY(parentid) REFERENCES paths(" DUF_SQL_IDNAME ") )",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dirname ON paths (dirname)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dev_uniq ON paths (dev,inode)",

/*"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parent1_uniq ON paths (parentid," DUF_SQL_IDNAME ")", */

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parent2_uniq ON paths (parentid,dirname)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_dev ON paths (dev)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_inode ON paths (inode)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "paths_parentid ON paths (parentid)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "paths_lastupdated " /* */
          " AFTER UPDATE OF dev,inode,dirname,parentid  ON paths " /* */
          " FOR EACH ROW BEGIN " /* */
          "   UPDATE paths SET last_updated=DATETIME()  WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "sizes ("
/* #ifdef DUF_USE_IDCOL                                                          */
/*                         DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " */
/*                         " size INTEGER NOT NULL" (* *)                        */
/* #else                                                                         */
          " size INTEGER PRIMARY KEY NOT NULL" /* */
/* #endif */
          ", dupzcnt INTEGER"   /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

/* #ifdef DUF_USE_IDCOL                                                                                                      */
/*     "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "sizes_uniq ON sizes (size)", */
/* #endif                                                                                                                    */
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "sizes_dup ON sizes (dupzcnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "sizes_lastupdated " /* */
          " AFTER UPDATE OF size, dupzcnt " /* */
          " ON sizes FOR EACH ROW BEGIN " /* */
          "   UPDATE sizes SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

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
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_uniq ON  path_pairs (Pathid1, Pathid2)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid1 ON path_pairs (Pathid1)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid2 ON path_pairs (Pathid2)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_samefiles ON path_pairs (samefiles)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "mime ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "mime text NOT NULL, dupmimecnt INTEGER" /* */
          /*", charset text NOT NULL, tail text" *//* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mime_uniq ON mime (mime" /* */
          /* ", charset" */ " )",

          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "mime_dup ON mime (dupmimecnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "mime_lastupdated " /* */
          " AFTER UPDATE OF mime" /* */
          /* ", charset, tail " *//* */
          " ON mime FOR EACH ROW BEGIN " /* */
          "   UPDATE mime SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "crc32 " /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " crc32sum INTEGER NOT NULL, dup32cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "crc32_crc32sum ON crc32 (crc32sum)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "crc32_dup ON crc32 (dup32cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "crc32_lastupdated " /* */
          " AFTER UPDATE OF crc32sum ON crc32 " /* */
          " FOR EACH ROW BEGIN    UPDATE crc32 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "sd5 " /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "sd5sum1 INTEGER NOT NULL, sd5sum2 INTEGER NOT NULL, dup2cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "sd5_sd5sum ON sd5 (sd5sum1,sd5sum2)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "sd5_dup ON sd5 (dup2cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "sd5_lastupdated " /* */
          " AFTER UPDATE OF sd5sum1, sd5sum1 ON sd5 " /* */
          " FOR EACH ROW BEGIN    UPDATE sd5 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "md5 " /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          "md5sum1 INTEGER NOT NULL, md5sum2 INTEGER NOT NULL, dup5cnt INTEGER" /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "md5_dup ON md5 (dup5cnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "md5_md5sum ON md5 (md5sum1,md5sum2)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_DBPREF "md5_lastupdated " /* */
          " AFTER UPDATE OF md5sum1, md5sum1 ON md5 " /* */
          " FOR EACH ROW BEGIN    UPDATE md5 SET last_updated=DATETIME() WHERE " DUF_SQL_IDNAME "=OLD." DUF_SQL_IDNAME " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "mdpath ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, " /* */
#endif
          "  mdpathsum1 INTEGER NOT NULL, mdpathsum2 INTEGER NOT NULL " /* */
          ", duppcnt INTEGER"   /* */
          ", last_updated REAL, inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_mdpathsum ON mdpath (mdpathsum1,mdpathsum2)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_dup ON mdpath (duppcnt)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "exif_model ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " model TEXT, "       /* */
          "  inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          /* " , FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES exif(modelid) " */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "exif_model_model ON exif_model (model)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " /* */
          DUF_DBPREF "exif ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDNAME " INTEGER PRIMARY KEY autoincrement, "
#endif
          " modelid INTEGER , date_time REAL" /* */
          ", dupexifcnt INTEGER" /* */
          ", fixed INTEGER"     /* */
          ", broken_date TEXT " /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          /* ", FOREIGN KEY(" DUF_SQL_IDNAME ") REFERENCES filedatas(exifid) " */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "exif_m_d ON exif (modelid, date_time)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "exif_m_id ON exif (modelid)",
          "CREATE INDEX IF NOT EXISTS " DUF_DBPREF "exif_dup ON exif (dupexifcnt)",

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
          /*          " , FOREIGN KEY(itemid)  REFERENCES paths(" DUF_SQL_IDNAME ") " *//* */
          " , FOREIGN KEY(tagnameid) REFERENCES tagnames(" DUF_SQL_IDNAME ") "
          " , FOREIGN KEY(itemid) REFERENCES filenames(" DUF_SQL_IDNAME ") "
          " , FOREIGN KEY(itemid) REFERENCES paths(" DUF_SQL_IDNAME ") "
	    " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "tags_uniq ON tags (tagnameid, itemid)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/


          "END" /* */ ,
          NULL}
};
