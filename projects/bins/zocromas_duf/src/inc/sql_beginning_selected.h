#ifndef SQL_BEGINNING_SELECTED_H
#  define SQL_BEGINNING_SELECTED_H


#  include "sql_tables_defs.h"
#  include "sql_selected_defs.h"



#  define DUF_SQL_UFILTER_BINDINGS \
           " (:minSize     IS NULL OR  fd.size     >=:minSize "                                        ") AND "  \
           " (:maxSize     IS NULL OR  fd.size     <=:maxSize "                                        ") AND "  \
\
           " (:minInode    IS NULL OR  fd.inode    >=:minInode "                                       ") AND "  \
           " (:maxInode    IS NULL OR  fd.inode    <=:maxInode "                                       ") AND "  \
\
           " (:minMd5ID    IS NULL OR  fd.md5id    >=:minMd5ID "                                       ") AND "  \
           " (:maxMd5ID    IS NULL OR  fd.md5id    <=:maxMd5ID "                                       ") AND "  \
\
           " (:minSha1ID   IS NULL OR  fd.sha1id   >=:minSha1ID "                                      ") AND "  \
           " (:maxSha1ID   IS NULL OR  fd.sha1id   <=:maxSha1ID "                                      ") AND "  \
\
           " (:minSd2ID    IS NULL OR  fd.sd5id    >=:minSd2ID "                                       ") AND "  \
           " (:maxSd2ID    IS NULL OR  fd.sd5id    <=:maxSd2ID "                                       ") AND "  \
\
           " (:minMimeID   IS NULL OR  fd.mimeid   >=:minMimeID "                                      ") AND "  \
           " (:maxMimeID   IS NULL OR  fd.mimeid   <=:maxMimeID "                                      ") AND "  \
           " (:MimeType    IS NULL OR  mi.mime     =:MimeType "                                        ") AND "  \
\
           " (:minDataID   IS NULL OR  fd." DUF_SQL_IDNAME ">=:minDataID "                             ") AND "  \
           " (:maxDataID   IS NULL OR  fd." DUF_SQL_IDNAME "<=:maxDataID "                             ") AND "  \
\
           " (:minExifID   IS NULL OR  fd.exifid   >=:minExifID "                                      ") AND "  \
           " (:maxExifID   IS NULL OR  fd.exifid   <=:maxExifID "                                      ") AND "  \
\
           " (:minMTime    IS NULL OR  fd.mtim     >=datetime(:minMTime,'unixepoch') "                 ") AND "  \
           " (:maxMTime    IS NULL OR  fd.mtim     <=datetime(:maxMTime,'unixepoch') "                 ") AND "  \
\
           " (:minExifDT   IS NULL OR  x.date_time >=datetime(:minExifDT,'unixepoch') "                ") AND "  \
           " (:maxExifDT   IS NULL OR  x.date_time <=datetime(:maxExifDT,'unixepoch') "                ") AND "  \
\
           " (:minNameID   IS NULL OR  fn." DUF_SQL_IDNAME ">=:minNameID "                             ") AND "  \
           " (:maxNameID   IS NULL OR  fn." DUF_SQL_IDNAME "<=:maxNameID "                             ") AND "  \
\
           " (:minDirID    IS NULL OR  fn.Pathid   >=:minDirID "                                       ") AND "  \
           " (:maxDirID    IS NULL OR  fn.Pathid   <=:maxDirID "                                       ") AND "  \
\
           " (:minMd5Same  IS NULL OR  md.dup5cnt  >=:minMd5Same "                                     ") AND "  \
           " (:maxMd5Same  IS NULL OR  md.dup5cnt  <=:maxMd5Same "                                     ") AND "  \
\
           " (:minSha1Same  IS NULL OR  sh.dupsha1cnt  >=:minSha1Same "                                ") AND "  \
           " (:maxSha1Same  IS NULL OR  sh.dupsha1cnt  <=:maxSha1Same "                                ") AND "  \
\
           " (:minSizeSame IS NULL OR  sz.dupzcnt  >=:minSizeSame "                                    ") AND "  \
           " (:maxSizeSame IS NULL OR  sz.dupzcnt  <=:maxSizeSame "                                    ") AND "  \
\
           " (:minDataSame IS NULL OR  fd.dupdatacnt  >=:minDataSame "                                 ") AND "  \
           " (:maxDataSame IS NULL OR  fd.dupdatacnt  <=:maxDataSame "                                 ") AND "  \
\
           " (:minExifSame IS NULL OR  x.dupexifcnt>=:minExifSame "                                    ") AND "  \
           " (:maxExifSame IS NULL OR  x.dupexifcnt<=:maxExifSame "                                    ") AND "  \
\
           " (:minMimeSame IS NULL OR mi.dupmimecnt>=:minMimeSame "                                    ") AND "  \
           " (:maxMimeSame IS NULL OR mi.dupmimecnt<=:maxMimeSame "                                    ") AND "  \
\
           " (:Name "    " IS NULL OR fn.name      = :Name "                                           ") AND "  \
           " ( ( :GSameAs  IS NULL OR :GSamePathID IS NULL ) "  \
                                  " OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb "  \
                                           " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") "  \
                                             " WHERE fnb.name = :GSameAsMd5 AND fnb.Pathid=:GSameMd5PathID ) "  \
           " ) AND " \
           " ( ( :GSameAsSha1  IS NULL OR :GSameSha1PathID IS NULL ) "  \
                                  " OR sh." DUF_SQL_IDNAME "=(SELECT fdb.sha1id FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb "  \
                                           " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") "  \
                                             " WHERE fnb.name = :GSameAsSha1 AND fnb.Pathid=:GSameSha1PathID ) "  \
           " ) AND " \
           " ( ( :GSameAsExif  IS NULL OR :GSameExifPathID IS NULL ) "  \
                                 "  OR x." DUF_SQL_IDNAME "=(SELECT fdb.exifid FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb "  \
                                           " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") "  \
                                           "  WHERE fnb.name = :GSameAsExif AND fnb.Pathid=:GSameExifPathID ) "  \
           " ) AND " \
	   " ( " \
	   "  SELECT COUNT(tg.itemid) icnt " \
	        " FROM "      DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tbo " \
	        " LEFT JOIN " DUF_SQL_TABLES_TAGNAMES_FULL        " AS tn  ON (tbo.arg=tn.name) " \
	        " LEFT JOIN " DUF_SQL_TABLES_TAGS_FULL            " AS tg  ON (tg.tagnameid=tn.rowid) " \
	        " WHERE tg.itemid=fn.rowid AND tg.itemtype='filename' " \
	        " GROUP BY itemid " \
	        " HAVING icnt=(SELECT count(*) FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tboc " \
		" WHERE tbo.oval=:Option_Val_With_Tag_File) " \
	   " ) > 0 AND " \
           " ( :Camera "   " IS NULL OR xm.model     LIKE :Camera ) AND "  \
           " ( :GName "    " IS NULL OR fn.name      GLOB :GName  ) AND "  \
           " ( :GNameI "   " IS NULL OR fn.name      GLOB :GNameI ) AND "  \
           " ( :GNameX "   " IS NULL OR fn.name  NOT GLOB :GNameX ) AND "  \
	" 1 "

/* --- XXX "has one of tags" XXX --- replaced with "has both tags"
  	    " ( " \
  	           " (SELECT COUNT(*) AS C FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tbo WHERE tbo.oval= :Option_Val_With_Tag_File) == 0 " \
  	        " OR " \
  	           " fn.rowid IN (SELECT itemid FROM " DUF_SQL_TABLES_TAGS_FULL " AS t LEFT JOIN  tagnames AS tn ON (t.tagnameid=tn.rowid) " \
  	                        " WHERE itemtype='filename' AND tn.name IN " \
  	                                         " (SELECT arg FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tbo WHERE tbo.oval= :Option_Val_With_Tag_File ))  " \
  	    " ) AND " \
*/

        /* "         OR tgn.name IN (SELECT arg FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tbo WHERE tbo.oval= :Option_Val_With_Tag_File ))" \ */

        /* " AND (:TagDir  IS NULL OR tgn.name=:TagDir)" \  */
        /* " AND (:TagFile IS NULL OR tgn.name=:TagFile)" \ */
/* " AND ((SELECT COUNT(*) AS C FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL ") == 0 OR tgn.name IN (SELECT arg FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " AS tbo WHERE tbo.name='files-tagged-as' ))" \ */



#  include "sql_beginning_types.h"
extern duf_sql_sequence_t sql_create_selected;
extern duf_sql_sequence_t sql_update_selected;

#endif
