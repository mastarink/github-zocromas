#include <string.h>

#include "duf_defs.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "sql_beginning_selected.h"
/* ###################################################################### */



duf_sql_sequence_t sql_beginning_selected = {
  .done = 0,
  .sql = {
//"DELETE FROM " DUF_SQL_SELECTED_NAME /* */
//      ,
//"INSERT INTO " DUF_SQL_SELECTED_NAME /* */
//      " SELECT fn." DUF_SQL_IDNAME ",NULL,NULL" /* */
//      "   FROM filenames AS fn LEFT " /* */
//      "        JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
//      "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
//      "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
//      "      WHERE "          /* */
//      "  ( (:minSize     IS NULL OR      fd.size>=:minSize)   AND (:maxSize   IS NULL OR fd.size<=:maxSize      ))  AND" /* */
//      "  ( (:minSame     IS NULL OR   md.dup5cnt>=:minSame)   AND (:maxSame   IS NULL OR md.dup5cnt<=:maxSame   ))  AND" /* */
//      "  ( (:minInode    IS NULL OR     fd.inode>=:minInode)  AND (:maxInode  IS NULL OR fd.inode<=:maxInode    ))  AND" /* */
//      "  ( (:minMd5ID    IS NULL OR     fd.md5id>=:minMd5ID)  AND (:maxMd5ID    IS NULL OR fd.md5id<=:maxMd5ID  ))  AND" /* */
//      "  ( (:minSd2ID    IS NULL OR     fd.sd5id>=:minSd2ID)  AND (:maxSd2ID    IS NULL OR fd.sd5id<=:maxSd2ID  ))  AND" /* */
//      "  ( (:minMimeID   IS NULL OR    fd.mimeid>=:minMimeID) AND (:maxMimeID IS NULL OR fd.mimeid<=:maxMimeID  ))  AND" /* */
//      "  ( (:minExifID   IS NULL OR    fd.exifid>=:minExifID) AND (:maxExifID IS NULL OR fd.exifid<=:maxExifID  ))  AND" /* */
//      "  ( (:minMTime    IS NULL OR      fd.mtim>=datetime(:minMTime,'unixepoch')) AND " /* */
//      "    (:maxMTime    IS NULL OR      fd.mtim<=datetime(:maxMTime,'unixepoch') )) AND" /* */
//      "  ( (:minExifDT   IS NULL OR  x.date_time>=datetime(:minExifDT,'unixepoch')) AND" /* */
//      "    (:maxExifDT   IS NULL OR  x.date_time<=datetime(:maxExifDT,'unixepoch'))  ) AND" /* */
//      "  ( (:minNameID   IS NULL OR fn." DUF_SQL_IDNAME ">=:minNameID) AND (:maxNameID IS NULL OR fn." DUF_SQL_IDNAME "<=:maxNameID )) AND" /* */
//      "  ( (:minDirID    IS NULL OR    fn.Pathid>=:minDirID)  AND (:maxDirID IS NULL OR fn.Pathid<=:maxDirID )) AND" /* */
//      "  ( (:minExifSame IS NULL OR x.dupexifcnt>=:minExifSame)  AND (:maxExifSame   IS NULL OR md.dup5cnt<=:maxExifSame   )) AND " /* */
//      "  ( :GName        IS NULL OR fn.name GLOB :GName ) " /* */
//      " AND "                 /* */
//      "  ( ( :GSameAs    IS NULL OR :GSamePathID IS NULL ) " /* */
//      " OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM filenames AS fnb " /* */
//      "   JOIN " DUF_DBPREF "filedatas AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") " /* */
//      "          WHERE fnb.name GLOB :GSameAs AND fnb.Pathid=:GSamePathID ) ) " /* */
//      , 
#ifndef DUF_SQL_SELECTED_TEMPORARY
            "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_NAME_FULL /* */ ,
#endif
            "CREATE  " DUF_SQL_SELECTED_TEMPORARY_STRING "  TABLE  " DUF_SQL_SELECTED_NAME_FULL /* */
            " AS "              /* */
            " SELECT fn." DUF_SQL_IDNAME ", fn." DUF_SQL_IDNAME " AS nameid "
            /* ",NULL AS last_updated,NULL AS inow" */
            "   FROM filenames AS fn LEFT " /* */
            "        JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
            "   LEFT JOIN " DUF_DBPREF "md5  AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
            "   LEFT JOIN " DUF_DBPREF "exif  AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
            "      WHERE "      /* */
	    DUF_SQL_UFILTER_BINDINGS
            ,
            "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_NAME_FULL "_rowid ON " DUF_SQL_SELECTED_NAME_FULL " (rowid) " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_NAME_FULL "_nameid ON " DUF_SQL_SELECTED_NAME_FULL " (nameid) " /* */        ,

#ifndef DUF_SQL_SELECTED_TEMPORARY
            "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_PATHS_FULL /* */ ,
#endif
            "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE  " DUF_SQL_SELECTED_PATHS_FULL " AS " /* */
            " WITH RECURSIVE parents_cte(fid, did, parentid) AS " /* */
            "   ( SELECT sel.rowid as fid, fn.dataid AS did, p.rowid as parentid " /* */
            "      FROM " DUF_SQL_SELECTED_NAME_FULL " AS sel LEFT JOIN filenames AS fn ON (sel.rowid=fn.rowid) " /* */
            "         LEFT JOIN paths AS p ON (p.rowid=fn.Pathid) " /* */
            " UNION "           /* */
            " SELECT fid, did, pp.parentid as parentid " /* */
            " FROM parents_cte " /* */
            " LEFT JOIN paths as pp ON( pp.rowid = parents_cte.parentid ) " /* */
            " ) "               /* */
            " SELECT fid, did, parentid FROM parents_cte WHERE parentid IS NOT NULL GROUP BY ParentId " /* */ ,

//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHS_FULL "_rowid ON " DUF_SQL_SELECTED_PATHS_FULL " (rowid) " /* */        ,
            "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHS_FULL "_fid ON " DUF_SQL_SELECTED_PATHS_FULL " (fid)" /* */ ,
            "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHS_FULL "_did ON " DUF_SQL_SELECTED_PATHS_FULL " (did)" /* */ ,
            "CREATE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHS_FULL "_parentid ON " DUF_SQL_SELECTED_PATHS_FULL " (parentid)" /* */ ,


#ifndef DUF_SQL_SELECTED_TEMPORARY
            "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_PATHTOT_FILES_FULL /* */ ,
#endif
            "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE " DUF_SQL_SELECTED_PATHTOT_FILES_FULL /* */
            " AS "              /* */
            " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
#if 0
            " FROM " DUF_DBPREF " filenames AS fn " /* */
#else
            " FROM " DUF_SQL_SELECTED_NAME_FULL " AS sel LEFT JOIN filenames AS fn ON (sel.rowid=fn.rowid) " /* */
#endif
            " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
            " GROUP BY fn.Pathid " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHTOT_FILES_FULL "_rowid ON " DUF_SQL_SELECTED_PATHTOT_FILES_FULL " (rowid) " /* */        ,
            "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHTOT_FILES_FULL "_Pathid ON " DUF_SQL_SELECTED_PATHTOT_FILES_FULL " (Pathid)"
            /* */ ,
            "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_PATHTOT_FILES_FULL "_numfiles ON " DUF_SQL_SELECTED_PATHTOT_FILES_FULL " (numfiles)"
            /* */ ,

#ifndef DUF_SQL_SELECTED_TEMPORARY
            "DROP TABLE IF EXISTS " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL /* */ ,
#endif
            "CREATE " DUF_SQL_SELECTED_TEMPORARY_STRING " TABLE " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL /* */
            " AS "              /* */
            " SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT( * ) " /* */
            " AS numdirs "      /* */
            " FROM "            /* */
#if 0
            DUF_DBPREF " paths " /* */
#else
            DUF_SQL_SELECTED_PATHS_FULL " AS pts " /* */
	    " LEFT JOIN " DUF_DBPREF " paths ON( pts.parentid = paths.rowid ) " /* */
#endif
            " JOIN " DUF_DBPREF " paths AS parents ON( parents." DUF_SQL_IDNAME " = paths.parentid ) " /* */
            " GROUP BY parents." DUF_SQL_IDNAME " " /* */ ,
//"CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL "_rowid ON " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL " (rowid) " /* */        ,
            "CREATE UNIQUE INDEX IF NOT EXISTS " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL "_Pathid ON " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL " (Pathid)" /* */
            ,
            "CREATE INDEX IF NOT EXISTS        " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL "_numdirs ON " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL " (numdirs)"
            /* */ ,
            NULL}
};
