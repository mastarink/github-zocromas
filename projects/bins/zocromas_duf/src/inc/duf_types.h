#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H

typedef struct
{
  unsigned long long md5id;
  unsigned long long size;
  unsigned long long dupcnt;
} md5_std_data_t;

typedef int ( *duf_str_cb_t ) ( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                                      void *str_cb_udata );
typedef int ( *duf_sql_select_cb_t ) ( int nrow, int nrows, char **presult, va_list args, void *sel_cb_udata, duf_str_cb_t fuscan );

#endif
