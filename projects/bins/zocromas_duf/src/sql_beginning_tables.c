#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

/* ###################################################################### */
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */
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
          ", inow " DUF_SQL_DATETIME_TYPE " DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW'))" /* */
          " )",                                                      /* */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL "_option ON " DUF_SQL_TABLES_TMP_TDB_OPTIONS " (oval, name, arg)", /* */
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL "_names ON " DUF_SQL_TABLES_TMP_TDB_OPTIONS " (name)", /* */

          "DELETE FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL,        /* */

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
