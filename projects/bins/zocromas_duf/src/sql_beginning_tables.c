#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_tables.h"
/* ###################################################################### */


duf_beginning_t sql_beginning_tables = {
  .done = 0,
  .sql = {
#ifndef DUF_SQL_TABLES_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_PATHTOT_FILES_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_TABLES_TEMPORARY_STRING " TABLE " DUF_SQL_TABLES_PATHTOT_FILES_FULL /* */
          " AS "                /* */
          " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
          " FROM " DUF_DBPREF " filenames AS fn " /* */
          " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
          " GROUP BY fn.Pathid " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHTOT_FILES_FULL "_rowid ON " DUF_SQL_TABLES_PATHTOT_FILES_FULL " (rowid) " /* */        ,
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_TABLES_PATHTOT_FILES_FULL " (Pathid)"
          /* */ ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_TABLES_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_TABLES_PATHTOT_FILES_FULL " (numfiles)" /* */
          ,
#ifndef DUF_SQL_TABLES_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_TABLES_PATHTOT_DIRS_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_TABLES_TEMPORARY_STRING " TABLE " DUF_SQL_TABLES_PATHTOT_DIRS_FULL /* */
          " AS "                /* */
          " SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT( * ) " /* */
          " AS numdirs "        /* */
          " FROM "              /* */
          DUF_SQL_TABLES_PATHS_FULL " AS pts " " LEFT JOIN " DUF_DBPREF " paths ON( pts.parentid = paths.rowid ) " /* */
          " JOIN " DUF_DBPREF " paths AS parents ON( parents." DUF_SQL_IDNAME " = paths.parentid ) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME " " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "_rowid ON " DUF_SQL_TABLES_PATHTOT_DIRS_FULL " (rowid) " /* */        ,
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_TABLES_PATHTOT_DIRS_FULL " (Pathid)"
          /* */ ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_TABLES_PATHTOT_DIRS_FULL " (numdirs)"
          /* */ ,
          /* TODO:
           * md5cnt
           * sd5cnt
           * crc32cnt
           * mimecnt
           * exifcnt
           * sizes
           * path_pairs
           * ...?
           *
           *  - see mod_integrity_uni.c
           * */
          NULL}
};
