#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H

#  define DUF_TRACE_NO 0
#  define DUF_TRACE_YES 1

#  define SEL_CB_DEF ((duf_sql_select_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_str_cb_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

typedef struct
{
  unsigned long long md5id;
  unsigned long long size;
  unsigned long long dupcnt;
} md5_std_data_t;

typedef int ( *duf_str_cb_t ) ( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                                void *str_cb_udata );
typedef int ( *duf_sql_select_cb_t ) ( int nrow, int nrows, char **presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                                       void *str_cb_udata );

#endif
