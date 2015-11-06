#ifndef MAS_SQL_TABLES_DEFS_H
#  define MAS_SQL_TABLES_DEFS_H

#  include "sql_tables_global_defs.h"

#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_DROP
#  else
#    define DUF_SQL_SELECTED_DROP
#  endif

#  define DUF_ATTACH_COMMON_PATTERN "${DB_PATH}${DB_NAME}_$_"

#  ifndef DUF_SQL_TTABLES_TEMPORARY
#    define DUF_ATTACH_TTABLES_PATTERN DUF_ATTACH_COMMON_PATTERN
#  endif

#  ifndef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_ATTACH_SELECTED_PATTERN DUF_ATTACH_COMMON_PATTERN "${PDI_NAME}_selected.db"
#  endif

#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_TEMPORARY_STRING DUF_SQL_SELECTED_TEMPORARY
#  else
#    define DUF_SQL_SELECTED_TEMPORARY_STRING
#  endif

/* #  ifdef DUF_SQL_TABLES_TEMPORARY                                    */
/* #    define DUF_SQL_TABLES_TEMPORARY_STRING DUF_SQL_TABLES_TEMPORARY */
/* #  else                                                              */
/* #    define DUF_SQL_TABLES_TEMPORARY_STRING                          */
/* #  endif                                                             */

#  ifdef DUF_SQL_TTABLES_TEMPORARY
#    define DUF_SQL_TTABLES_TEMPORARY_STRING DUF_SQL_TTABLES_TEMPORARY
#  else
#    define DUF_SQL_TTABLES_TEMPORARY_STRING
#  endif

#  define DUF_SQL_TABLES_TMP_TDB_OPTIONS       "t_tdb_options"
#  ifndef DUF_NO_NUMS
#    ifdef DUF_USE_TMP_PATHTOT_FILES_TABLE
#      define DUF_SQL_TABLES_TMP_PATHTOT_FILES     "t_common_pathtot_files"
#    endif
#    ifdef DUF_USE_TMP_PATHTOT_DIRS_TABLE
#      define DUF_SQL_TABLES_TMP_PATHTOT_DIRS      "t_common_pathtot_dirs"
#    endif

#    define DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES " ( SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " \
							" FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " \
								" LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDFIELD " ) " \
							" GROUP BY fn.Pathid ) "
#    define DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS  " ( SELECT parents." DUF_SQL_IDFIELD " AS Pathid, COUNT( * ) AS numdirs " \
							" FROM " DUF_SQL_TABLES_PATHS_FULL " AS pts " \
								" LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " \
								" JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD " = ptsp.parentid ) " \
							" GROUP BY parents." DUF_SQL_IDFIELD ") "
#  endif
       /* ifndef DUF_NO_NUMS */
#  ifndef DUF_NO_RNUMS
#    define DUF_SQL__RNUMDIRS(_paths_alias) "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS  ( " \
			      " SELECT dpt." DUF_SQL_IDFIELD ",dpt.parentid " \
			        " FROM " DUF_SQL_TABLES_PATHS_FULL "  AS dpt " \
			        " WHERE parentid=" #_paths_alias "." DUF_SQL_IDFIELD \
			        " UNION " \
			          " SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid " \
			            " FROM cte_paths " \
			            " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " \
			            " ) " \
			    " SELECT COUNT(*)   " \
			    "   FROM cte_paths  AS ptenud " \
			    "   LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS dpt ON (ptenud." DUF_SQL_IDFIELD "=dpt." DUF_SQL_IDFIELD ") "
#    define DUF_SQL_RNUMDIRS(_paths_alias) " ( " DUF_SQL__RNUMDIRS(_paths_alias) " ) "

#    define DUF_SQL__RNUMFILES(_paths_alias) "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS  ( " \
			      " SELECT fpt." DUF_SQL_IDFIELD ",fpt.parentid " \
			        " FROM " DUF_SQL_TABLES_PATHS_FULL "  AS fpt  " \
			        " WHERE fpt.parentid=" #_paths_alias "." DUF_SQL_IDFIELD " OR fpt." DUF_SQL_IDFIELD "=" #_paths_alias "." DUF_SQL_IDFIELD \
			        " UNION " \
			          " SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid " \
			            " FROM cte_paths " \
			            " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " \
			            " ) " \
			    " SELECT COUNT(*)  " \
			      " FROM cte_paths  AS ptenuf " \
			      " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (ptenuf." DUF_SQL_IDFIELD "=fn.Pathid) " \
			      " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " \
			      " JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)"

#    define DUF_SQL_RNUMFILES(_paths_alias, _wmore)  " ( " DUF_SQL__RNUMFILES(_paths_alias) # _wmore " ) "
#  endif
       /* ifndef DUF_NO_RNUMS */

#  if 1

#    define DUF_SQL_TABLES_FILENAMES_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL          DUF_DBPREF  DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SHA1_FULL               DUF_DBPREF  DUF_SQL_TABLES_SHA1
#    define DUF_SQL_TABLES_SD5_FULL                DUF_DBPREF  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL              DUF_DBPREF  DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL              DUF_DBPREF  DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL               DUF_DBPREF  DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL         DUF_DBPREF  DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL               DUF_DBPREF  DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL              DUF_DBPREF  DUF_SQL_TABLES_PATHS
#    define DUF_SQL_TABLES_TAGS_FULL               DUF_DBPREF  DUF_SQL_TABLES_TAGS
#    define DUF_SQL_TABLES_TAGNAMES_FULL           DUF_DBPREF  DUF_SQL_TABLES_TAGNAMES

#    ifdef DUF_SQL_TTABLES_TEMPORARY
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL     DUF_SQL_TABLES_TMP_TDB_OPTIONS
#      ifndef DUF_NO_NUMS
#        ifdef DUF_USE_TMP_PATHTOT_FILES_TABLE
#          define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL   DUF_SQL_TABLES_TMP_PATHTOT_FILES
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL  DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL
#        else
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES
#        endif
#        ifdef DUF_USE_TMP_PATHTOT_DIRS_TABLE
#          define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL    DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#        else
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS
#        endif
#      endif
#    else
#      define DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL    DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_TDB_OPTIONS
#      ifndef DUF_NO_NUMS
#        ifdef DUF_USE_TMP_PATHTOT_FILES_TABLE
#          define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL  DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_PATHTOT_FILES
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL  DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL
#        else
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES
#        endif
#        ifdef DUF_USE_TMP_PATHTOT_DIRS_TABLE
#          define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL   DUF_DBTEMPPREF  DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL   DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL
#        else
#          define DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS
#        endif
#      endif
#    endif
#  else
#    define DUF_SQL_TABLES_FILENAMES_FULL            DUF_SQL_TABLES_FILENAMES
#    define DUF_SQL_TABLES_FILEDATAS_FULL            DUF_SQL_TABLES_FILEDATAS
#    define DUF_SQL_TABLES_MD5_FULL                  DUF_SQL_TABLES_MD5
#    define DUF_SQL_TABLES_SHA1_FULL                 DUF_SQL_TABLES_SHA1
#    define DUF_SQL_TABLES_SD5_FULL                  DUF_SQL_TABLES_SD5
#    define DUF_SQL_TABLES_CRC32_FULL                DUF_SQL_TABLES_CRC32
#    define DUF_SQL_TABLES_SIZES_FULL                DUF_SQL_TABLES_SIZES
#    define DUF_SQL_TABLES_EXIF_FULL                 DUF_SQL_TABLES_EXIF
#    define DUF_SQL_TABLES_EXIF_MODEL_FULL           DUF_SQL_TABLES_EXIF_MODEL
#    define DUF_SQL_TABLES_MIME_FULL                 DUF_SQL_TABLES_MIME
#    define DUF_SQL_TABLES_PATHS_FULL                DUF_SQL_TABLES_PATHS
#    define DUF_SQL_TABLES_TAGS_FULL                 DUF_SQL_TABLES_TAGS
#    define DUF_SQL_TABLES_TAGNAMES_FULL             DUF_SQL_TABLES_TAGNAMES
#    define DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL    DUF_SQL_TABLES_TMP_PATHTOT_FILES
#    define DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL     DUF_SQL_TABLES_TMP_PATHTOT_DIRS
#  endif

#endif
