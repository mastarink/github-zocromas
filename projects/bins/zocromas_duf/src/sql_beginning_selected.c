#include <string.h>

#include "duf_defs.h"
#include "sql_tables_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_selected.h"
/* ###################################################################### */

#ifdef DUF_ATTACH_SELECTED_PATTERN
#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#else
#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    error Wrong "DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#endif

/* TODO :
 * DROP + CREATE AS => CREATE + DELETE + INSERT SELECT 
 * */
duf_sql_sequence_t sql_create_selected = {
  .name = "selected (create selected tables)",
  .done = 0,
  .beginend = 0,
  .set_selected_db = 1,
  .sql = {
#ifdef DUF_SQL_SELECTED_DROP
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */ , /* XXX */
#endif
          "CREATE  " DUF_SQL_SELECTED_TEMPORARY_STRING "  TABLE IF NOT EXISTS  " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */
          " AS "                /* */
          " SELECT fn." DUF_SQL_IDNAME ", fn." DUF_SQL_IDNAME " AS nameid "
          /* ",NULL AS last_updated,NULL AS inow" */
          "   FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn LEFT " /* */
          "        JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_SHA_FULL "  AS sh ON (sh." DUF_SQL_IDNAME "=fd.shaid) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
          "   LEFT JOIN " DUF_SQL_TABLES_MIME_FULL " AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
          "      WHERE "        /* */
          DUF_SQL_UFILTER_BINDINGS /* */
          /* " GROUP BY nameid " (* *) */
          ,                     /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_FILENAMES_FULL "_fnid ON " DUF_SQL_SELECTED_TMP_FILENAMES " (nameid) " /* */ , /* XXX */

#ifdef DUF_SQL_SELECTED_DROP
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL /* */ , /* XXX */
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS  " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS " /* */
          " WITH RECURSIVE parents_cte(fid, did, parentid) AS " /* */
          "   ( SELECT sel.nameid as fid, fn.dataid AS did, p." DUF_SQL_IDNAME " as parentid " /* */
          "      FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.nameid=fn." DUF_SQL_IDNAME ") " /* */
          "         LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS p ON (p." DUF_SQL_IDNAME "=fn.Pathid) " /* */
          " UNION "             /* */
          " SELECT fid, did, pp.parentid as parentid " /* */
          " FROM parents_cte "  /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " as pp ON( pp." DUF_SQL_IDNAME " = parents_cte.parentid ) " /* */
          " ) "                 /* */
          " SELECT fid, did, parentid FROM parents_cte WHERE parentid IS NOT NULL GROUP BY ParentId " /* */ , /* XXX */

          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_fid ON " DUF_SQL_SELECTED_TMP_PATHS " (fid)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_did ON " DUF_SQL_SELECTED_TMP_PATHS " (did)" /* */ ,
          "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHS_FULL "_parentid ON " DUF_SQL_SELECTED_TMP_PATHS " (parentid)" /* */ , /* XXX */


#ifdef DUF_SQL_SELECTED_DROP
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL /* */
          " AS "                /* */
          " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
#if 0
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
#else
          " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS sel " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (sel.nameid=fn." DUF_SQL_IDNAME ") " /* */
#endif
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
          " GROUP BY fn.Pathid " /* */ , /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES " (Pathid)"
          /* */ ,
          /* XXX */
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_SELECTED_TMP_PATHTOT_FILES
          " (numfiles)"
          /* */ ,
          /* XXX */

#ifdef DUF_SQL_SELECTED_DROP
          "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL /* */ ,
#endif
          "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL /* */
          " AS "                /* */
          " SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT( * ) AS numdirs " /* */
          " FROM "              /* */
#if 0
          DUF_SQL_TABLES_PATHS_FULL " " /* */
#else
          DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDNAME " ) " /* */
#endif
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDNAME " = ptsp.parentid ) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME " ", /* XXX */
          "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (Pathid)", /* XXX */
          "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS " (numdirs)", /* XXX */

          NULL}
};
