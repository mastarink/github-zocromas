#ifndef MAS_SQL_TABLES_GLOBAL_DEFS_H
# define MAS_SQL_TABLES_GLOBAL_DEFS_H

/* see duf_defs.h : */
/* #  define DUF_USE_CTE yes */

/* # define DUF_SQL_DATETIME_TYPE             "REAL" */
/* # define DUF_SQL_DATETIME_TYPE             "DATETIME" */
# define DUF_SQL_DATETIME_TYPE             "INTEGER"

/* #  define DUF_USE_TMP_PATHTOT yes */
# ifdef DUF_USE_TMP_PATHTOT
#  define DUF_USE_TMP_PATHTOT_DIRS_TABLE yes
#  define DUF_USE_TMP_PATHTOT_FILES_TABLE yes
# endif

# define DUF_SQL_SELECTED_TEMPORARY "TEMPORARY"
# define DUF_SQL_TTABLES_TEMPORARY "TEMPORARY"
/* #  define DUF_SQL_TABLES_TEMPORARY "TEMPORARY" */

# define DUF_SQL_TABLES_FILENAMES             "filenames"
# define DUF_SQL_TABLES_FILEDATAS             "filedatas"
# define DUF_SQL_TABLES_SHA1                  "sha1"
# define DUF_SQL_TABLES_MD5                   "md5"
# define DUF_SQL_TABLES_SD5                   "sd5"
# define DUF_SQL_TABLES_CRC32                 "crc32"
# define DUF_SQL_TABLES_SIZES                 "sizes"
# define DUF_SQL_TABLES_EXIF                  "exif"
# define DUF_SQL_TABLES_EXIF_MODEL            "exif_model"
# define DUF_SQL_TABLES_MIME                  "mime"
# define DUF_SQL_TABLES_PATHS                 "paths"
# define DUF_SQL_TABLES_TAGS                  "tags"
# define DUF_SQL_TABLES_TAGNAMES              "tagnames"

#endif
