#ifndef SQL_BEGINNING_TABLES_H
#  define SQL_BEGINNING_TABLES_H
/* #  define DUF_SQL_TTABLES_TEMPORARY "TEMPORARY" */

#  define DUF_SQL_TABLES_TEMPORARY "TEMPORARY"
#  ifdef DUF_SQL_TABLES_TEMPORARY
#    define DUF_SQL_TABLES_TEMPORARY_STRING DUF_SQL_TABLES_TEMPORARY
#  else
#    define DUF_SQL_TABLES_TEMPORARY_STRING
#  endif

#  ifdef DUF_SQL_TTABLES_TEMPORARY
#    define DUF_SQL_TTABLES_TEMPORARY_STRING DUF_SQL_TTABLES_TEMPORARY
#  else
#    define DUF_SQL_TTABLES_TEMPORARY_STRING
#  endif


#  define DUF_SQL_TABLES_NAME  "filenames"
#  define DUF_SQL_TABLES_PATHS "paths"
#  define DUF_SQL_TABLES_PATHTOT_FILES "common_pathtot_files"
#  define DUF_SQL_TABLES_PATHTOT_DIRS "common_pathtot_dirs"

#  define DUF_SQL_TABLES_NAME_FULL  DUF_SQL_TABLES_NAME
#  define DUF_SQL_TABLES_PATHS_FULL DUF_SQL_TABLES_PATHS
#  define DUF_SQL_TABLES_PATHTOT_FILES_FULL DUF_SQL_TABLES_PATHTOT_FILES
#  define DUF_SQL_TABLES_PATHTOT_DIRS_FULL DUF_SQL_TABLES_PATHTOT_DIRS

#include "sql_beginning_types.h"


extern duf_sql_sequence_t sql_beginning_tables;

#endif
