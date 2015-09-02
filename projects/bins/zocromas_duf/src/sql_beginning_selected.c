#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_selected.h"
/* ###################################################################### */



duf_sql_sequence_t sql_beginning_selected = {
  .done = 0,
  .sql = {
          "BEGIN",
#ifndef DUF_SQL_SELECTED_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */ ,
#endif
          "CREATE  " DUF_SQL_SELECTED_TEMPORARY_STRING "  TABLE  " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */
          " AS "                /* */
          " SELECT fn." DUF_SQL_IDNAME " AS rowid, fn." DUF_SQL_IDNAME " AS nameid "
          /* ",NULL AS last_updated,NULL AS inow" */
          "   FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn LEFT " /* */
          "        JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
          "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
          "   LEFT JOIN " DUF_DBPREF "exif_model AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
          "   LEFT JOIN " DUF_DBPREF "mime AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
          "      WHERE "        /* */
          DUF_SQL_UFILTER_BINDINGS /* */
          /* " GROUP BY nameid " (* *) */
          ,
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL "_rowid ON " DUF_SQL_SELECTED_TMP_FILENAMES " (rowid) " /* */
          ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL "_nameid ON " DUF_SQL_SELECTED_TMP_FILENAMES " (nameid) " /* */        ,

#ifndef DUF_SQL_SELECTED_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE  " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS " /* */
          " WITH RECURSIVE parents_cte(fid, did, parentid) AS " /* */
          "   ( SELECT sel.rowid as fid, fn.dataid AS did, p.rowid as parentid " /* */
          "      FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.rowid=fn.rowid) " /* */
          "         LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS p ON (p.rowid=fn.Pathid) " /* */
          " UNION "             /* */
          " SELECT fid, did, pp.parentid as parentid " /* */
          " FROM parents_cte "  /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " as pp ON( pp.rowid = parents_cte.parentid ) " /* */
          " ) "                 /* */
          " SELECT fid, did, parentid FROM parents_cte WHERE parentid IS NOT NULL GROUP BY ParentId " /* */ ,

//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_rowid ON " DUF_SQL_SELECTED_TMP_PATHS " (rowid) " /* */        ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_fid ON " DUF_SQL_SELECTED_TMP_PATHS " (fid)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_did ON " DUF_SQL_SELECTED_TMP_PATHS " (did)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_parentid ON " DUF_SQL_SELECTED_TMP_PATHS " (parentid)" /* */ ,


#ifndef DUF_SQL_SELECTED_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */
          " AS "                /* */
          " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
#if 0
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
#else
          " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.rowid=fn.rowid) " /* */
#endif
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
          " GROUP BY fn.Pathid " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_rowid ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES " (rowid) " /* */        ,
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES
          " (Pathid)"
          /* */ ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES
          " (numfiles)"
          /* */ ,

#ifndef DUF_SQL_SELECTED_TEMPORARY
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL /* */
          " AS "                /* */
          " SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT( * ) " /* */
          " AS numdirs "        /* */
          " FROM "              /* */
#if 0
          DUF_SQL_TABLES_PATHS_FULL " "  /* */
#else
          DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp.rowid ) " /* */
#endif
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDNAME " = ptsp.parentid ) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME " " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_rowid ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (rowid) " /* */        ,
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (Pathid)" /* */
          ,
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS
          " (numdirs)",

          "END",
          NULL}
};
