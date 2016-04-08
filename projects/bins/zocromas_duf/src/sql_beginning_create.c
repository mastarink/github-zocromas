#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */
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

duf_sql_sequence_t sql_beginning_drop =                              /* */
{
  .name = "clear (drop tables)",
  .done = 0,
  .beginend = 1,
  .sql = {
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_CRC32_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_EXIF_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL,
#if 0
          "DROP TABLE IF EXISTS filefilter",
#endif
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SIZES_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SHA1_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_MIME_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_SD5_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_MD5_FULL,

          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_FILENAMES_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL,
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_PATHS_FULL,

          "DROP TABLE IF EXISTS tags",
          "DROP TABLE IF EXISTS tagnames",
          "DROP TABLE IF EXISTS log",
          "DROP TABLE IF EXISTS path_pairs",
          "DROP TABLE IF EXISTS mdpath",
          NULL}
};

duf_sql_sequence_t sql_beginning_clean =                             /* */
{
  .name = "clean tables",
  .done = 0,
  .beginend = 1,
  .sql = {
          "DELETE FROM " DUF_SQL_TABLES_FILEDATAS_FULL,
          "DELETE FROM " DUF_SQL_TABLES_FILENAMES_FULL,
          "DELETE FROM " DUF_SQL_TABLES_CRC32_FULL,
          "DELETE FROM " DUF_SQL_TABLES_EXIF_FULL,
          "DELETE FROM " DUF_SQL_TABLES_EXIF_MODEL_FULL,
#if 0
          "DELETE FROM filefilter",
#endif
          "DELETE FROM " DUF_SQL_TABLES_SIZES_FULL,
          "DELETE FROM " DUF_SQL_TABLES_SHA1_FULL,
          "DELETE FROM " DUF_SQL_TABLES_MIME_FULL,
          "DELETE FROM " DUF_SQL_TABLES_SD5_FULL,
          "DELETE FROM " DUF_SQL_TABLES_MD5_FULL,

          "DELETE FROM " DUF_SQL_TABLES_PATHS_FULL,

          "DELETE FROM tags",
          "DELETE FROM tagnames",
          "DELETE FROM log",
          "DELETE FROM path_pairs",
          "DELETE FROM mdpath",
          NULL}
};

duf_sql_sequence_t sql_beginning_create_one = {
  .name = "create (main tables 0)",
  .done = 0,
  .beginend = 1,
  .sql = {
          "CREATE TABLE IF NOT EXISTS " DUF_DBADMPREF "log ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement,"
#endif
          " args  TEXT, restored_args  TEXT, msg  TEXT"              /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
#if 0
          "CREATE TABLE IF NOT EXISTS "                              /* */
          DUF_DBADMPREF "filefilter ("
# ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement,"      /* */
# endif
          " type INTEGER"                                            /* */
          ", minsize INTEGER, maxsize INTEGER"                       /* */
          ", mindups INTEGER, maxdups INTEGER"                       /* */
          ", glob_include TEXT"                                      /* */
          ", glob_exclude TEXT"                                      /* */
          ", run " DUF_SQL_DATETIME_TYPE                             /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBADMPREF "filefilter_all_uniq ON filefilter " /* */
          " (minsize, maxsize, mindups, maxdups"                     /* */
          ", glob_include, glob_exclude)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_DBADMPREF "filefilter_lastupdated " /* */
          " AFTER UPDATE OF minsize, maxsize, mindups, maxdups, glob_include, glob_exclude " /* */
          " ON filefilter FOR EACH ROW BEGIN "                       /* */
          "   UPDATE filefilter SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",
#endif

          NULL}
};
/* TODO move to modules respectively ?! */
duf_sql_sequence_t sql_beginning_create_two = {
  .name = "create (main tables)",
  .done = 0,
  .beginend = 1,
  .sql = {
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL " ("
/* #ifdef DUF_USE_IDCOL                                                          */
/*                         DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, " */
/*                         " size INTEGER NOT NULL" (* *)                        */
/* #else                                                                         */
          " size INTEGER PRIMARY KEY NOT NULL"                       /* */
/* #endif */
          ", dupzcnt INTEGER"                                        /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

/* #ifdef DUF_USE_IDCOL                                                                                                      */
/*        "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_uniq ON "DUF_SQL_TABLES_SIZES" (size)", */
/* #endif                                                                                                                    */
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_dup  ON " DUF_SQL_TABLES_SIZES " (dupzcnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SIZES_FULL "_lastupdated " /* */
          " AFTER UPDATE OF size, dupzcnt "                          /* */
          " ON " DUF_SQL_TABLES_SIZES " FOR EACH ROW BEGIN "         /* */
          "   UPDATE " DUF_SQL_TABLES_SIZES " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL      /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "sd5sum1 INTEGER NOT NULL, sd5sum2 INTEGER NOT NULL, dup2cnt INTEGER" /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_sd5sum ON " DUF_SQL_TABLES_SD5 " (sd5sum1,sd5sum2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_dup    ON " DUF_SQL_TABLES_SD5 " (dup2cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SD5_FULL "_lastupdated " /* */
          " AFTER UPDATE OF sd5sum1, sd5sum1 ON " DUF_SQL_TABLES_SD5 /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "    UPDATE " DUF_SQL_TABLES_SD5 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL      /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "md5sum1 INTEGER NOT NULL, md5sum2 INTEGER NOT NULL, dup5cnt INTEGER" /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_dup    ON " DUF_SQL_TABLES_MD5 " (dup5cnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_md5sum ON " DUF_SQL_TABLES_MD5 " (md5sum1,md5sum2)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_MD5_FULL "_lastupdated " /* */
          " AFTER UPDATE OF md5sum1, md5sum2 ON " DUF_SQL_TABLES_MD5 /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "    UPDATE " DUF_SQL_TABLES_MD5 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL     /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "sha1sum1 INTEGER NOT NULL, sha1sum2 INTEGER NOT NULL, sha1sum3 INTEGER NOT NULL, dupsha1cnt INTEGER" /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_dup    ON " DUF_SQL_TABLES_SHA1 " (dupsha1cnt)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_sha1sum ON " DUF_SQL_TABLES_SHA1 " (sha1sum1,sha1sum2,sha1sum3)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_SHA1_FULL "_lastupdated " /* */
          " AFTER UPDATE OF sha1sum1, sha1sum2, sha1sum3 ON " DUF_SQL_TABLES_SHA1 /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "    UPDATE " DUF_SQL_TABLES_SHA1 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL    /* */
          " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " crc32sum INTEGER NOT NULL, dup32cnt INTEGER"             /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_crc32sum ON " DUF_SQL_TABLES_CRC32 " (crc32sum)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_dup      ON " DUF_SQL_TABLES_CRC32 " (dup32cnt)",
          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_CRC32_FULL "_lastupdated " /* */
          " AFTER UPDATE OF crc32sum ON " DUF_SQL_TABLES_CRC32       /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "    UPDATE " DUF_SQL_TABLES_CRC32 " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " model  TEXT, "                                           /* */
          "  inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_MODEL_FULL "_model ON " DUF_SQL_TABLES_EXIF_MODEL " (model)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " modelid INTEGER "                                        /* " REFERENCES " DUF_SQL_TABLES_SIZES " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", date_time " DUF_SQL_DATETIME_TYPE                       /* */
          ", dupexifcnt INTEGER"                                     /* */
          ", fixed INTEGER"                                          /* */
          ", broken_date TEXT "                                      /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
        /* ", FOREIGN KEY(" DUF_SQL_IDFIELD ") REFERENCES " DUF_SQL_TABLES_FILEDATAS "(exifid) " */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_m_d  ON " DUF_SQL_TABLES_EXIF " (modelid, date_time)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_m_id ON " DUF_SQL_TABLES_EXIF " (modelid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_EXIF_FULL "_dup  ON " DUF_SQL_TABLES_EXIF " (dupexifcnt)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "mime TEXT NOT NULL, dupmimecnt INTEGER"                   /* */
/* ", charset text NOT NULL, tail text"  (*   *) */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_uniq ON " DUF_SQL_TABLES_MIME " (mime" /* */
        /* ", charset" */ " )",

          "CREATE INDEX IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_dup ON " DUF_SQL_TABLES_MIME " (dupmimecnt)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_MIME_FULL "_lastupdated " /* */
          " AFTER UPDATE OF " DUF_SQL_TABLES_MIME                    /* */
/*  ", charset, tail "  (*   *) */
          " ON " DUF_SQL_TABLES_MIME " FOR EACH ROW BEGIN "          /* */
          "   UPDATE " DUF_SQL_TABLES_MIME " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

          NULL}
};

duf_sql_sequence_t sql_beginning_create_three = {
  .name = "create (main tables)",
  .done = 0,
  .beginend = 1,
  .sql = {
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "     /* */
#endif
          "  dev INTEGER NOT NULL, inode INTEGER NOT NULL "          /* */
          ", rdev INTEGER"                                           /* */
          ", mode INTEGER, nlink INTEGER"                            /* */
          ", uid INTEGER, gid INTEGER"                               /* */
          ", blksize INTEGER, blocks INTEGER"                        /* */
          ", size INTEGER"                                           /* */
          ", atim " DUF_SQL_DATETIME_TYPE ", atimn INTEGER"          /* */
          ", mtim " DUF_SQL_DATETIME_TYPE ", mtimn INTEGER"          /* */
          ", ctim " DUF_SQL_DATETIME_TYPE ", ctimn INTEGER"          /* */
          ", " DUF_SQL_DIRNAMEFIELD " TEXT"                          /* */
          ", parentid INTEGER "                                      /* " REFERENCES " DUF_SQL_TABLES_PATHS *//* */
          ", priority INTEGER" ", last_updated " DUF_SQL_DATETIME_TYPE /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
        /* ", FOREIGN KEY(parentid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDFIELD ") " (* *) */
          " ) ",
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_" DUF_SQL_DIRNAMEFIELD " ON " DUF_SQL_TABLES_PATHS " (" DUF_SQL_DIRNAMEFIELD ")",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_dev_uniq     ON " DUF_SQL_TABLES_PATHS " (dev,inode)",

/*        "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parent1_uniq ON " DUF_SQL_TABLES_PATHS " (parentid," DUF_SQL_IDFIELD ")", */

          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parent2_uniq ON " DUF_SQL_TABLES_PATHS " (parentid," DUF_SQL_DIRNAMEFIELD
          ")",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_dev          ON " DUF_SQL_TABLES_PATHS " (dev)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_inode        ON " DUF_SQL_TABLES_PATHS " (inode)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_parentid     ON " DUF_SQL_TABLES_PATHS " (parentid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_priority     ON " DUF_SQL_TABLES_PATHS " (priority)",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_PATHS_FULL "_lastupdated " /* */
          " AFTER UPDATE OF dev,inode," DUF_SQL_DIRNAMEFIELD ",parentid  ON " DUF_SQL_TABLES_PATHS /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "   UPDATE " DUF_SQL_TABLES_PATHS " SET last_updated=DATETIME()  WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dev INTEGER NOT NULL, rdev INTEGER, inode INTEGER NOT NULL" /* */
          ", mode INTEGER NOT NULL "                                 /* */
          ", nlink INTEGER NOT NULL"                                 /* */
          ", uid INTEGER NOT NULL, gid INTEGER NOT NULL"             /* */
          ", blksize INTEGER NOT NULL, blocks INTEGER NOT NULL"      /* */
          ", atim " DUF_SQL_DATETIME_TYPE " NOT NULL, atimn INTEGER NOT NULL" /* */
          ", mtim " DUF_SQL_DATETIME_TYPE " NOT NULL, mtimn INTEGER NOT NULL" /* */
          ", ctim " DUF_SQL_DATETIME_TYPE " NOT NULL, ctimn INTEGER NOT NULL" /* */
          ", size    INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_SIZES " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", md5id   INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_MD5 " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", sha1id  INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_SHA1 " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", sd5id   INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_SD5 " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", crc32id INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_CRC32 " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", mimeid  INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_MIME " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", exifid  INTEGER "                                       /*" REFERENCES " DUF_SQL_TABLES_EXIF " DEFERRABLE INITIALLY DEFERRED " *//* */
          ", noexif INTEGER"                                         /* */
          ", filetype TEXT, filestatus INTEGER"                      /* */
          ", dupdatacnt INTEGER "                                    /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE                    /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
        /* ", FOREIGN KEY(md5id)   REFERENCES " DUF_SQL_TABLES_MD5 "   (" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(sha1id)  REFERENCES " DUF_SQL_TABLES_SHA1 "  (" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(sd5id)   REFERENCES " DUF_SQL_TABLES_SD5 "   (" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(crc32id) REFERENCES " DUF_SQL_TABLES_CRC32 " (" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(exifid)  REFERENCES " DUF_SQL_TABLES_EXIF "  (" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(mimeid)  REFERENCES " DUF_SQL_TABLES_MIME "  (" DUF_SQL_IDFIELD ") " (* *) */
          ", FOREIGN KEY(size)    REFERENCES " DUF_SQL_TABLES_SIZES " ( size ) " /* */
          ")",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_md5id      ON " DUF_SQL_TABLES_FILEDATAS " (md5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sha1id     ON " DUF_SQL_TABLES_FILEDATAS " (sha1id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sha1md5id  ON " DUF_SQL_TABLES_FILEDATAS " (sha1id,md5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_sd5id      ON " DUF_SQL_TABLES_FILEDATAS " (sd5id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_crc32id    ON " DUF_SQL_TABLES_FILEDATAS " (crc32id)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_exifid     ON " DUF_SQL_TABLES_FILEDATAS " (exifid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_exifid2    ON " DUF_SQL_TABLES_FILEDATAS " (exifid,noexif)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILEDATAS_FULL "_noexif     ON " DUF_SQL_TABLES_FILEDATAS " (noexif)",
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
          ",atim,atimn,mtim,mtimn,ctim,ctimn,filetype,filestatus "   /* */
          " ON " DUF_SQL_TABLES_FILEDATAS " FOR EACH ROW BEGIN "     /* */
          "   UPDATE " DUF_SQL_TABLES_FILEDATAS " SET last_updated=DATETIME() WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL " (" /* */
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          "dataid   INTEGER "                                        /* " REFERENCES " DUF_SQL_TABLES_FILEDATAS " DEFERRABLE INITIALLY DEFERRED " *//* *//* */
          ", Pathid INTEGER "                                        /* " REFERENCES " DUF_SQL_TABLES_PATHS " DEFERRABLE INITIALLY DEFERRED " *//*     *//* */
          ", " DUF_SQL_FILENAMEFIELD " TEXT NOT NULL"                /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE                    /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
        /* ", FOREIGN KEY(dataid) REFERENCES " DUF_SQL_TABLES_FILEDATAS "(" DUF_SQL_IDFIELD ") " (* *) */
        /* ", FOREIGN KEY(Pathid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDFIELD ") "         */
          " ) "                                                      /* */
          ,
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_filename ON " DUF_SQL_TABLES_FILENAMES " (" DUF_SQL_FILENAMEFIELD ")",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_dataid   ON " DUF_SQL_TABLES_FILENAMES " (dataid)",
          "CREATE        INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_Pathid   ON " DUF_SQL_TABLES_FILENAMES " (Pathid)",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_uniq     ON " DUF_SQL_TABLES_FILENAMES " (Pathid,"
          DUF_SQL_FILENAMEFIELD ")",

          "CREATE TRIGGER IF NOT EXISTS " DUF_SQL_TABLES_FILENAMES_FULL "_lastupdated " /* */
          " AFTER UPDATE OF dataid," DUF_SQL_FILENAMEFIELD ",Pathid ON " DUF_SQL_TABLES_FILENAMES /* */
          " FOR EACH ROW BEGIN "                                     /* */
          "   UPDATE " DUF_SQL_TABLES_FILENAMES " SET last_updated=DATETIME()  WHERE " DUF_SQL_IDFIELD "=OLD." DUF_SQL_IDFIELD " ; END",

          NULL}
};

duf_sql_sequence_t sql_beginning_create_four = {
  .name = "create (main tables)",
  .done = 0,
  .beginend = 1,
  .sql = {
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_TAGNAMES_FULL " ( "
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " name TEXT NOT NULL, inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TAGNAMES_FULL "_uniq ON " DUF_SQL_TABLES_TAGNAMES " (name)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_SQL_TABLES_TAGS_FULL " ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement,"
#endif
          " tagnameid INTEGER "                                      /* " REFERENCES tagnames DEFERRABLE INITIALLY DEFERRED " *//* */
          " , itemtype TEXT NOT NULL "                               /* */
          " , itemid INTEGER NOT NULL, inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')) " /* */
/*           " , FOREIGN KEY(itemid)  REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDFIELD ") "  (*   *) */
        /* " , FOREIGN KEY(tagnameid) REFERENCES tagnames(" DUF_SQL_IDFIELD ") " (* *) */
        /* " , FOREIGN KEY(itemid) REFERENCES " DUF_SQL_TABLES_FILENAMES "(" DUF_SQL_IDFIELD ") " (* *) */
        /* " , FOREIGN KEY(itemid) REFERENCES " DUF_SQL_TABLES_PATHS "(" DUF_SQL_IDFIELD ") " (* *)     */
          " )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TAGS_FULL "_uniq ON " DUF_SQL_TABLES_TAGS " (tagnameid, itemid)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "path_pairs ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " Pathid1 INTEGER NOT NULL "                               /* */
          ", Pathid2 INTEGER NOT NULL "                              /* */
          ", samefiles INTEGER, last_updated " DUF_SQL_DATETIME_TYPE /* */
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')) )",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_uniq      ON  path_pairs (Pathid1, Pathid2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid1   ON path_pairs (Pathid1)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_pathid2   ON path_pairs (Pathid2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "path_pairs_samefiles ON path_pairs (samefiles)",
/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE TABLE IF NOT EXISTS " DUF_DBPREF "mdpath ("
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "     /* */
#endif
          "  mdpathsum1 INTEGER NOT NULL, mdpathsum2 INTEGER NOT NULL " /* */
          ", duppcnt INTEGER"                                        /* */
          ", last_updated " DUF_SQL_DATETIME_TYPE ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')))",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_mdpathsum ON mdpath (mdpathsum1,mdpathsum2)",
          "CREATE        INDEX IF NOT EXISTS " DUF_DBPREF "mdpath_dup       ON mdpath (duppcnt)",

/******************************************************************************************************/
/***                                                                                             ******/
/******************************************************************************************************/
          "CREATE  VIEW  IF NOT EXISTS " DUF_DBPREF "v_selected_filenames AS " /* */
          " SELECT fn." DUF_SQL_IDFIELD " AS " DUF_SQL_IDFIELD ", fn." DUF_SQL_IDFIELD " AS nameid  " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "           /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
          " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
          " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL " AS mi ON( fd.mimeid = mi." DUF_SQL_IDFIELD " )",

          NULL}
};
