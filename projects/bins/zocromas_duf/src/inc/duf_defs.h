#ifndef MAS_DUF_DEFS_H
#  define MAS_DUF_DEFS_H

#  define DUF_FL __func__,__LINE__



#  define DUF_RECURSIVE_NO DUF_FALSE
#  define DUF_RECURSIVE_YES DUF_TRUE

#  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE
#  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE

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



/* ###################################################################### */

#  define DUF_ACTION_TITLE_FMT "-17s"
#  define DUF_ACTION_TITLE_PFMT "%" DUF_ACTION_TITLE_FMT
#  define DUF_DEPTH_FMT "-2d"
#  define DUF_DEPTH_PFMT "L%" DUF_DEPTH_FMT



/* ###################################################################### */

#  define DUF_UNUSED __attribute__(( unused ))
/* ###################################################################### */

#  define DUF_FALSE 0
#  define DUF_TRUE 1

/* ###################################################################### */

#  define SEL_CB_DEF ((duf_sel_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_str_cb_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

#  define DUF_STAT_DEF ((struct stat *) NULL)

/* ###################################################################### */


#endif
