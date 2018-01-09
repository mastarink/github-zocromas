#ifndef MAS_QSTD_DEFS_H
# define MAS_QSTD_DEFS_H

/* # define QSTD_TABPREF "${QSTD_PREFIX}" */
# define QSTD_TABPREF "${PFS_PREFIX}"
/* # define QSTD_TABPREF "qstd_" */

// \<\(file\(sizes\|datas\|props\|names\)parents\|\(all\|dir\|file\)full\)\> //
# define QSTD_TABLE_SIZES     QSTD_TABPREF "filesizes"
# define QSTD_TABLE_DATAS     QSTD_TABPREF "filedatas"
# define QSTD_TABLE_PROPS     QSTD_TABPREF "fileprops"
# define QSTD_TABLE_PARENTS   QSTD_TABPREF "parents"
# define QSTD_TABLE_NAMES     QSTD_TABPREF "filenames"
# define QSTD_TABLE_MD5       QSTD_TABPREF "md5"
# define QSTD_TABLE_MD5DREF   QSTD_TABPREF "md5dref"
# define QSTD_TABLE_SHA1      QSTD_TABPREF "sha1"
# define QSTD_TABLE_SHA1DREF  QSTD_TABPREF "sha1dref"

# define QSTD_VIEW_ALL        QSTD_TABPREF "allfull"
# define QSTD_VIEW_DIRS       QSTD_TABPREF "dirfull"
# define QSTD_VIEW_FILES      QSTD_TABPREF "filefull"
# define QSTD_VIEW_FILES_OLD     QSTD_TABPREF "filefull1"

#endif
