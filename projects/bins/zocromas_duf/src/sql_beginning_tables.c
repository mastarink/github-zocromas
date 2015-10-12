#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_tables.h"
/* ###################################################################### */


duf_sql_sequence_t sql_beginning_tables = {
  .name = "tables (create tmp tables)",
  .done = 0,
  .beginend = 1,
  .sql = {

#ifndef DUF_SQL_TTABLES_TEMPORARY
          /* "DROP TABLE IF EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL (* *) , */
#endif
          "CREATE " DUF_SQL_TTABLES_TEMPORARY_STRING " TABLE  IF NOT EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " ( "
#ifdef DUF_USE_IDCOL
          DUF_SQL_IDFIELD " INTEGER PRIMARY KEY autoincrement, "
#endif
          " oval INTEGER NOT NULL, name TEXT NOT NULL, arg TEXT NOT NULL" /* */
          ", inow REAL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          " )",                 /* */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL "_option ON " DUF_SQL_TABLES_TMP_TDB_OPTIONS " (oval, name, arg)", /* */
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL "_names ON " DUF_SQL_TABLES_TMP_TDB_OPTIONS " (name)", /* */

          "DELETE FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL, /* */



#ifndef DUF_SQL_TTABLES_TEMPORARY
          /* "DROP TABLE IF EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL (* *) , */
#endif
          "CREATE " DUF_SQL_TTABLES_TEMPORARY_STRING " TABLE IF NOT EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL /* */
          " AS "                /* */
          " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
          " GROUP BY fn.Pathid " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "_" DUF_SQL_IDFIELD " ON " DUF_SQL_TABLES_TMP_PATHTOT_FILES " (" DUF_SQL_IDFIELD ") ",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_TABLES_TMP_PATHTOT_FILES " (Pathid)"
          /* */ ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_TABLES_TMP_PATHTOT_FILES " (numfiles)", /* */
          "DELETE FROM " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL, /* */
#ifndef DUF_SQL_TTABLES_TEMPORARY
          /* "DROP TABLE IF EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL (* *) , */
#endif
          "CREATE " DUF_SQL_TTABLES_TEMPORARY_STRING " TABLE IF NOT EXISTS  " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL /* */
          " AS "                /* */
          " SELECT parents." DUF_SQL_IDFIELD " AS Pathid, COUNT( * ) " /* */
          " AS numdirs "        /* */
          " FROM "              /* */
          DUF_SQL_TABLES_PATHS_FULL " AS pts " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD " = ptsp.parentid ) " /* */
          " GROUP BY parents." DUF_SQL_IDFIELD " " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "_" DUF_SQL_IDFIELD " ON " DUF_SQL_TABLES_TMP_PATHTOT_DIRS " (" DUF_SQL_IDFIELD ") ",
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_TABLES_TMP_PATHTOT_DIRS " (Pathid)"
          /* */ ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_TABLES_TMP_PATHTOT_DIRS " (numdirs)", /* */
          "DELETE FROM " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL, /* */
          /* */
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
           * */
          NULL}
};
