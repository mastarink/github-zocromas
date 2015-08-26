#ifndef SQL_BEGINNING_SELECTED_H
#  define SQL_BEGINNING_SELECTED_H

/* #  define DUF_SQL_SELECTED_TEMPORARY "TEMPORARY" */
#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_TEMPORARY_STRING DUF_SQL_SELECTED_TEMPORARY
#  else
#    define DUF_SQL_SELECTED_TEMPORARY_STRING
#  endif

#  define DUF_SQL_SELECTED_NAME  "selected_filenames"
#  define DUF_SQL_SELECTED_PATHS "selected_paths"
#  define DUF_SQL_SELECTED_PATHTOT_FILES "selected_pathtot_files"
#  define DUF_SQL_SELECTED_PATHTOT_DIRS "selected_pathtot_dirs"

#  if 0
#    define DUF_SQL_SELECTED_NAME_FULL  DUF_DBPREF DUF_SQL_SELECTED_NAME
#    define DUF_SQL_SELECTED_PATHS_FULL DUF_DBPREF DUF_SQL_SELECTED_PATHS
#    define DUF_SQL_SELECTED_PATHTOT_FILES_FULL DUF_DBPREF DUF_SQL_SELECTED_PATHTOT_FILES
#    define DUF_SQL_SELECTED_PATHTOT_DIRS_FULL DUF_DBPREF DUF_SQL_SELECTED_PATHTOT_DIRS
#  else
#    define DUF_SQL_SELECTED_NAME_FULL  DUF_SQL_SELECTED_NAME
#    define DUF_SQL_SELECTED_PATHS_FULL DUF_SQL_SELECTED_PATHS
#    define DUF_SQL_SELECTED_PATHTOT_FILES_FULL DUF_SQL_SELECTED_PATHTOT_FILES
#    define DUF_SQL_SELECTED_PATHTOT_DIRS_FULL DUF_SQL_SELECTED_PATHTOT_DIRS
#  endif

#include "sql_beginning_types.h"

#define DUF_SQL_UFILTER_BINDINGS \
        "    (:minSize     IS NULL OR  fd.size     >=:minSize                             ) AND"  \
        "    (:maxSize     IS NULL OR  fd.size     <=:maxSize                             ) AND"  \
        "    (:minSame     IS NULL OR  md.dup5cnt  >=:minSame                             ) AND"  \
        "    (:maxSame     IS NULL OR  md.dup5cnt  <=:maxSame                             ) AND"  \
        "    (:minInode    IS NULL OR  fd.inode    >=:minInode                            ) AND"  \
        "    (:maxInode    IS NULL OR  fd.inode    <=:maxInode                            ) AND"  \
        "    (:minMd5ID    IS NULL OR  fd.md5id    >=:minMd5ID                            ) AND"  \
        "    (:maxMd5ID    IS NULL OR  fd.md5id    <=:maxMd5ID                            ) AND"  \
        "    (:minSd2ID    IS NULL OR  fd.sd5id    >=:minSd2ID                            ) AND"  \
        "    (:maxSd2ID    IS NULL OR  fd.sd5id    <=:maxSd2ID                            ) AND"  \
        "    (:minMimeID   IS NULL OR  fd.mimeid   >=:minMimeID                           ) AND"  \
        "    (:maxMimeID   IS NULL OR  fd.mimeid   <=:maxMimeID                           ) AND"  \
        "    (:MimeType    IS NULL OR  mi.mime     =:MimeType                             ) AND"  \
        "    (:minExifID   IS NULL OR  fd.exifid   >=:minExifID                           ) AND"  \
        "    (:maxExifID   IS NULL OR  fd.exifid   <=:maxExifID                           ) AND"  \
        "    (:minMTime    IS NULL OR  fd.mtim     >=datetime(:minMTime,'unixepoch')      ) AND"  \
        "    (:maxMTime    IS NULL OR  fd.mtim     <=datetime(:maxMTime,'unixepoch')      ) AND"  \
        "    (:minExifDT   IS NULL OR  x.date_time >=datetime(:minExifDT,'unixepoch')     ) AND"  \
        "    (:maxExifDT   IS NULL OR  x.date_time <=datetime(:maxExifDT,'unixepoch')     ) AND"  \
        "    (:minNameID   IS NULL OR  fn." DUF_SQL_IDNAME ">=:minNameID                  ) AND"  \
        "    (:maxNameID   IS NULL OR  fn." DUF_SQL_IDNAME "<=:maxNameID                  ) AND"  \
        "    (:minDirID    IS NULL OR  fn.Pathid   >=:minDirID                            ) AND"  \
        "    (:maxDirID    IS NULL OR  fn.Pathid   <=:maxDirID                            ) AND"  \
        "    (:minExifSame IS NULL OR  x.dupexifcnt>=:minExifSame                         ) AND"  \
        "    (:maxExifSame IS NULL OR  md.dup5cnt  <=:maxExifSame                         ) AND"  \
        "    ( ( :GSameAs  IS NULL OR :GSamePathID IS NULL ) "  \
        "                          OR md." DUF_SQL_IDNAME "=(SELECT fdb.md5id FROM filenames AS fnb "  \
        "                                   JOIN " DUF_DBPREF "filedatas AS fdb ON (fnb.dataid=fdb." DUF_SQL_IDNAME ") "  \
        "                                     WHERE fnb.name = :GSameAs AND fnb.Pathid=:GSamePathID ) "  \
        "                  ) " \
	" AND ((SELECT COUNT(*) AS C FROM tdb_options) == 0 OR tgn.name IN (SELECT arg FROM tdb_options AS tbo WHERE tbo.oval= :DUF_OPTION_VAL_WITH_TAG_FILE ))" \
	" AND " \
        "  ( :GName        IS NULL OR fn.name      GLOB :GName                             ) AND "  \
        "  ( :GNameI       IS NULL OR fn.name      GLOB :GNameI                            ) AND "  \
        "  ( :GNameX       IS NULL OR fn.name  NOT GLOB :GNameX                            ) AND "  \
	" 1 "

        /* " AND (:TagDir  IS NULL OR tgn.name=:TagDir)" \  */
        /* " AND (:TagFile IS NULL OR tgn.name=:TagFile)" \ */
        /* " AND ((SELECT COUNT(*) AS C FROM tdb_options) == 0 OR tgn.name IN (SELECT arg FROM tdb_options AS tbo WHERE tbo.name='files-tagged-as' ))" \ */

extern duf_sql_sequence_t sql_beginning_selected;

#endif
