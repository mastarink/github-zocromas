#ifndef MAS_SQL_SELECTED_DEFS_H
#  define MAS_SQL_SELECTED_DEFS_H

#  include "sql_tables_global_defs.h"

#  define DUF_SQL_SELECTED_TMP_FILENAMES  "${PDI_NAME}_selected_filenames"
#  define DUF_SQL_SELECTED_TMP_PATHS "${PDI_NAME}_selected_paths"
#  ifndef DUF_NO_NUMS
#    define DUF_SQL_SELECTED_TMP_PATHTOT_FILES "${PDI_NAME}_selected_pathtot_files"
#    define DUF_SQL_SELECTED_TMP_PATHTOT_DIRS "${PDI_NAME}_selected_pathtot_dirs"
#  endif

#  ifdef DUF_SQL_SELECTED_TEMPORARY
#    define DUF_SQL_SELECTED_TMP_FILENAMES_FULL           DUF_SQL_SELECTED_TMP_FILENAMES
#    define DUF_SQL_SELECTED_TMP_PATHS_FULL               DUF_SQL_SELECTED_TMP_PATHS
#    ifndef DUF_NO_NUMS
#      define DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL       DUF_SQL_SELECTED_TMP_PATHTOT_FILES
#      define DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL        DUF_SQL_SELECTED_TMP_PATHTOT_DIRS
#    endif
#  else
#    define DUF_SQL_SELECTED_TMP_FILENAMES_FULL     DUF_DBSELECTEDPREF DUF_SQL_SELECTED_TMP_FILENAMES
#    define DUF_SQL_SELECTED_TMP_PATHS_FULL         DUF_DBSELECTEDPREF DUF_SQL_SELECTED_TMP_PATHS
#    ifndef DUF_NO_NUMS
#      define DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL DUF_DBSELECTEDPREF DUF_SQL_SELECTED_TMP_PATHTOT_FILES
#      define DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL  DUF_DBSELECTEDPREF DUF_SQL_SELECTED_TMP_PATHTOT_DIRS
#    endif
#  endif

#endif
