#ifndef MAS_DUF_DB_DEFS_H
#  define MAS_DUF_DB_DEFS_H


/* #  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE */
/* #  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE */

#  define DUF_DBADMALIAS "dufadm"

#  define DUF_DBPREF "main."
#  ifdef MAS_SPLIT_DB
#    define DUF_DBADMPREF DUF_DBADMALIAS "."
#  else
#    define DUF_DBADMPREF DUF_DBPREF
#  endif

#  define DUF_DBTEMPALIAS "duftemp"

#  ifdef MAS_SPLIT_DB
#    define DUF_DBTEMPPREF DUF_DBTEMPALIAS "."
#  else
#    define DUF_DBTEMPPREF DUF_DBPREF
#  endif

#  define DUF_DBSELECTEDALIAS "duf${PDI_NAME}_selected"

#  ifdef MAS_SPLIT_DB
#    define DUF_DBSELECTEDPREF DUF_DBSELECTEDALIAS "."
#  else
#    define DUF_DBSELECTEDPREF DUF_DBPREF
#  endif

#  if 0
#    define DUF_DBTOSETALIAS "duf${SELECTED_DB}"

#    ifdef MAS_SPLIT_DB
#      define DUF_DBTOSETPREF DUF_DBTOSETALIAS "."
#    else
#      define DUF_DBTOSETPREF DUF_DBPREF
#    endif
#  endif


#endif

