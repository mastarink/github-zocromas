#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H

typedef struct
{
  unsigned long long md5id;
  unsigned long long size;
  unsigned long long dupcnt;
} md5_std_data_t;

typedef int ( *duf_str_callback_t ) ( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                                      void *udata
                                      /* unsigned long long md5id, unsigned long long size */
       );
typedef int ( *duf_sql_select_callback_t ) ( int nrow, int rows, char **presult, va_list args, void *udata, duf_str_callback_t f );

#endif
