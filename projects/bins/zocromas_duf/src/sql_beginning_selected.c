#include <string.h>

#include "duf_defs.h"

#include "sql_tables_defs.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

/* ###################################################################### */
#include "sql_beginning_selected.h"
/* ###################################################################### */

#ifdef DUF_ATTACH_SELECTED_PATTERN
# ifdef DUF_SQL_SELECTED_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#else
# ifndef DUF_SQL_SELECTED_TEMPORARY
#  error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#endif

/* changed: DROP + CREATE AS => CREATE + DELETE + INSERT SELECT */
duf_sql_sequence_t sql_create_selected = {
  .name = "selected (create selected tables)",
  .done = 0,
  .beginend = 0,
  .set_selected_db = 1,
  .sql = {
#ifdef DUF_SQL_SELECTED_DROP
        /* "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL (* *) , (* XXX *) */
#endif
          "CREATE  " DUF_SQL_SELECTED_TEMPORARY_STRING "  TABLE IF NOT EXISTS  " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */
          " AS "                                                     /* */
          " SELECT fn." DUF_SQL_IDFIELD ", fn." DUF_SQL_IDFIELD " AS nameid "
        /* ",NULL AS last_updated,NULL AS inow" */
          "   FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn LEFT "    /* */
          "        JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "  AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL "  AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "  AS x ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_MIME_FULL " AS mi ON( fd.mimeid = mi." DUF_SQL_IDFIELD " ) " /* */
          "      WHERE "                                             /* */
          DUF_SQL_UFILTER_BINDINGS                                   /* */
        /* " GROUP BY nameid " (* *) */
          ,                                                          /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL "_fnid ON " DUF_SQL_SELECTED_TMP_FILENAMES " (nameid) " /* */ , /* XXX */
          "DELETE FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */ , /* XXX */

#ifdef DUF_SQL_SELECTED_DROP
        /* "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL (* *) , (* XXX *) */
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS  " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS " /* */
          " WITH RECURSIVE parents_cte(fid, did, parentid) AS "      /* */
          "   ( SELECT sel.nameid as fid, fn.dataid AS did, p." DUF_SQL_IDFIELD " as parentid " /* */
          "      FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.nameid=fn." DUF_SQL_IDFIELD ") " /* */
          "         LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS p ON (p." DUF_SQL_IDFIELD "=fn.Pathid) " /* */
          " UNION "                                                  /* */
          " SELECT fid, did, pp.parentid as parentid "               /* */
          " FROM parents_cte "                                       /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " as pp ON( pp." DUF_SQL_IDFIELD " = parents_cte.parentid ) " /* */
          " ) "                                                      /* */
          " SELECT fid, did, parentid FROM parents_cte WHERE parentid IS NOT NULL GROUP BY ParentId " /* */ , /* XXX */

          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_fid ON " DUF_SQL_SELECTED_TMP_PATHS " (fid)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_did ON " DUF_SQL_SELECTED_TMP_PATHS " (did)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_parentid ON " DUF_SQL_SELECTED_TMP_PATHS " (parentid)" /* */ , /* XXX */
          "DELETE FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL /* */ ,     /* XXX */

#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
# ifdef DUF_SQL_SELECTED_DROP
        /* "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL (* *) , */
# endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */
          " AS "                                                     /* */
          " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
# if 0
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "           /* */
# else
          " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel "    /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.nameid=fn." DUF_SQL_IDFIELD ") " /* */
# endif
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
          " GROUP BY fn.Pathid " /* */ ,                             /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES " (Pathid)"
        /* */ ,
        /* XXX */
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES " (numfiles)" /* */ , /* XXX */
          "DELETE FROM " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */ , /* XXX */
        /* XXX */

# ifdef DUF_SQL_SELECTED_DROP
        /* "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL (* *) , */
# endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL /* */
          " AS "                                                     /* */
          " SELECT parents." DUF_SQL_IDFIELD " AS Pathid, COUNT( * ) AS numdirs " /* */
          " FROM "                                                   /* */
# if 0
          DUF_SQL_TABLES_PATHS_FULL " "                              /* */
# else
          DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts "                 /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
# endif
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD " = ptsp.parentid ) " /* */
          " GROUP BY parents." DUF_SQL_IDFIELD " ",                  /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (Pathid)", /* XXX */
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (numdirs)", /* XXX */
          "DELETE FROM " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL,     /* XXX */
#endif

          NULL}
};
