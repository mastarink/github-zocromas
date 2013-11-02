#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H

#  define DUF_FALSE 0
#  define DUF_TRUE 1

#  define DUF_TRACE_NO DUF_FALSE
#  define DUF_TRACE_YES DUF_TRUE

#  define DUF_RECURSIVE_NO DUF_FALSE
#  define DUF_RECURSIVE_YES DUF_TRUE

#  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE
#  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE

#  define SEL_CB_DEF ((duf_sql_select_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_str_cb_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

typedef struct
{
  int recursive;
  int level;
} duf_dirinfo_t;
typedef struct
{
  unsigned long long md5id;
  unsigned long long size;
  unsigned long long dupcnt;
} md5_std_data_t;

typedef int ( *duf_str_cb_t ) ( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                                void *str_cb_udata, const char *const *all_args );
typedef int ( *duf_sql_select_cb_t ) ( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata,
                                       duf_str_cb_t str_cb, void *str_cb_udata );

#endif
